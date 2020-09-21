'''
/******************************************************************
*
* Copyright 2018 Samsung Electronics All Rights Reserved.
*
*
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
******************************************************************/
'''

import re
import operator
import os
from itertools import takewhile

from ite.config import *
from ite.util import *

class TCAnalyzer:
    GTC_PATTERN = re.compile(r'(TEST_F|TEST)\s*\(\s*(?P<testcasename>\w+)\s*,\s*(?P<testname>\w+)\s*\)', re.DOTALL)

    JUNIT_TS_PATTERN = re.compile(r'class\s*(?P<testsuite>\w+)\s*', re.DOTALL)
    #JUNIT_TC_PATTERN = re.compile(r'\@Test\s*\w+\s+\w+\s+(?P<testcase>\w+)\(\s*\)\s*\{', re.DOTALL)
    JUNIT_TC_PATTERN = re.compile(r'public\s+void\s+(?P<testcase>(Test|test)\w+)\s*\(\s*\)\s*\{', re.DOTALL)
    TRANSPORT_PATTERN = re.compile(r'@transport\s+(?P<transport>[\w+[\s,]*]*)', re.DOTALL)
    BUILD_TYPE_PATTERN = re.compile(r'@build_type\s+(?P<build_type>[\w+[ \t,-]*]*)', re.DOTALL)

    ARDUINOUNIT_TS_PATTERN = re.compile(r'(?P<testsuite>\w+\.\w+)', re.DOTALL)
    ARDUINOUNIT_TC_PATTERN = re.compile(r'test\((?P<testcase>\w+)\)\s*\{', re.DOTALL)

    COMMENT_PATTERN = re.compile(r'\/\*\*\s*(?P<comments>.+)\s*\*\/', re.DOTALL)
    PLATFORM_OR_TRANSPORT_DEFINE_PATTERN = re.compile(r'#if\s+\(*\s*defined\s*\(\w*\)*\s*((\|\|\s*)|(&&\s*)*)+')
    PLATFORM_OR_TRANSPORT_PATTERN = re.compile(r'defined\s*\(\s*(?P<platform_or_transport>\w+)\)\s*')

    def __init__(self, testfw):
        if testfw == TESTFW_TYPES.GTEST:
            self.analyzer = self.get_gtest_tc_info
        elif testfw == TESTFW_TYPES.JUNIT:
            self.analyzer = self.get_junit_tc_info
        elif testfw == TESTFW_TYPES.ARDUINOUNIT:
            self.analyzer = self.get_arduinounit_tc_info
        else:
            self.analyzer = None

    def count_file_bytes(self, iterable):
        file_bytes = 0
        for item in iterable:
            file_bytes += len(item)
            yield file_bytes

    def get_line_number(self, path, pos):
        file = open_read_file(path);
        if file == None:
            return 0

        line = 1+len(list(takewhile(lambda x: x <= pos, self.count_file_bytes(file))))

        file.close()

        return line

    def get_build_types(self, comments, module):

        build_types = []
        for build_type_match in self.BUILD_TYPE_PATTERN.finditer(comments):
            for build_type in re.split(', ', build_type_match.group('build_type').strip()):
                build_types.append(build_type.upper())

        if not build_types:
            for build_type in MODULES_BUILD_TYPES[module.upper()]:
                build_types.append(build_type)

        return build_types

    def get_arduinounit_tc_info(self, path, module):
        source = read_file(path)
        if source == '':
            return

        # Analyze Test Suite
        match = self.ARDUINOUNIT_TS_PATTERN.match(os.path.basename(path))
        if match == None:
            return
        testsuite = match.group('testsuite')

        prev_pos = 0
        for match in self.ARDUINOUNIT_TC_PATTERN.finditer(source):
            pos = match.start()
            line = self.get_line_number(path, pos)
            testcase = match.group('testcase')

            comments = ''
            for match in self.COMMENT_PATTERN.finditer(source[prev_pos:pos]):
                comments += match.group('comments')

            build_types = self.get_build_types(comments, module)

            platforms = dict()
            platforms[TEST_PLATFORM.ARDUINO] = dict()

            for build_type in build_types:
                platforms[TEST_PLATFORM.ARDUINO][build_type] = 1

            yield line, testsuite, testcase, comments, platforms

            prev_pos = pos

    def get_junit_tc_info(self, path, module):
        print("###### Junit Path: " + path + " ######")
        source = read_file(path)
        if source == '':
            return

        prev_pos = 0

        match = self.JUNIT_TS_PATTERN.search(source)
        if match == None:
            return

        prev_pos = match.start()
        testsuite = match.group('testsuite')

        for match in self.JUNIT_TC_PATTERN.finditer(source):
            pos = match.start()
            line = self.get_line_number(path, pos)
            testcase = match.group('testcase')

            platforms = dict()

            comments = ''
            for match in self.COMMENT_PATTERN.finditer(source[prev_pos:pos]):
                comment = match.group('comments')
                transport_value = 0

                build_types = self.get_build_types(comment, module)

                for transport_match in self.TRANSPORT_PATTERN.finditer(comment):
                    for transport_name in re.split(', ', transport_match.group('transport').strip()):
                        transport_name = transport_name.upper()
                        if transport_name in TRANSPORT_VALUES:
                            transport_value = transport_value + TRANSPORT_VALUES[transport_name]
                        elif not transport_name == 'NONE':
                            print ('invalid transport name in junit')

                if transport_value == 0:
                    transport_value = 1

                if TEST_PLATFORM.ANDROID not in platforms:
                    platforms[TEST_PLATFORM.ANDROID] = dict()

                for build_type in build_types:
                    platforms[TEST_PLATFORM.ANDROID][build_type] = transport_value

                comments += comment

            yield line, testsuite, testcase, comments, platforms

            prev_pos = pos

    def get_gtest_tc_info(self, path, module):
        source = read_file(path)
        if source == '':
            return

        if not '::testing::Test' in source:
            print ('Not GTest TC file: ' + path)
            return

        prev_pos = 0
        for match in self.GTC_PATTERN.finditer(source):
            pos = match.start()
            line = self.get_line_number(path, pos)
            testsuite = match.group('testcasename')
            testcase = match.group('testname')
            platforms = dict()
            platform_found = False

            comments = ''
            for match in self.COMMENT_PATTERN.finditer(source[prev_pos:pos]):
                comments += match.group('comments')

            build_types = self.get_build_types(comments, module)

            platforms_or_transports_matches = self.PLATFORM_OR_TRANSPORT_DEFINE_PATTERN.finditer(source[prev_pos:pos])

            for platforms_or_transports_match in platforms_or_transports_matches:
                platform_found = True

            if platform_found:
                begin_index = platforms_or_transports_match.start()
                tc_guard_text = platforms_or_transports_match.string[begin_index:pos]

                tc_guard_text = tc_guard_text.replace('__', '')
                tc_guard_text = tc_guard_text.replace(' ', '')
                tc_guard_text = tc_guard_text.replace('\n', '')
                tc_guard_text = tc_guard_text.replace('\r', '')
                tc_guard_text = tc_guard_text.replace('#if', '')
                tc_guard_text = tc_guard_text.replace('defined', '')
                tc_guard_text = tc_guard_text.replace('||', '+')
                tc_guard_text = tc_guard_text.replace('&&', '*')
                tc_guard_text = tc_guard_text.replace('WINDOWS', '0')

                for platform in TEST_PLATFORM:
                    platform_transport_value = tc_guard_text
                    platform_transport_value = platform_transport_value.replace(platform, '1')

                    for other_platform in TEST_PLATFORM:
                        platform_transport_value = platform_transport_value.replace(other_platform, '0')

                    platform_transport_value = platform_transport_value.replace(DEFINE_TRANSPORT.ALL_TRANSPORT, ALL_TRANSPORT_VALUES[platform])

                    for transport in TRANSPORT_VALUES:
                        platform_transport_value = platform_transport_value.replace(transport, str(TRANSPORT_VALUES[transport]))

                    try:
                        platform_transport_value = eval(platform_transport_value)
                    except:
                        if 'CA' in path:
                            print ('something wrong with defined tag')
                            print (path)

                    if platform_transport_value != 0:
                        if platform not in platforms:
                            platforms[platform] = dict()
                        for build_type in build_types:
                            platforms[platform][build_type] = platform_transport_value
            else:
                if TEST_PLATFORM.LINUX not in platforms:
                    platforms[TEST_PLATFORM.LINUX] = dict()
                for build_type in build_types:
                    platforms[TEST_PLATFORM.LINUX][build_type] = 1

            yield line, testsuite, testcase, comments, platforms

            prev_pos = pos

    def analyze_tc_file(self, path, module):
        if (os.path.isfile(path) == False):
            return

        print("### Start to analyze test case file: " + path)
        test_spec = dict()
        invalid_tc = list()

        for line, suite, name, comments, platforms in self.analyzer(path, module):
            #print (line, suite, name, comments, platforms)
            spec = TestSpec(line, suite, name)
            #print (spec.suite, spec.name)
            success, message = spec.check_tc_naming_rule()
            if (success == False):
                invalid_tc.append((line, suite, name, message))
                continue

            if (len(comments) == 0):
                invalid_tc.append((line, suite, name, 'Cannot Find TC Comments'))
                continue

            success, message = spec.parse_comment(comments)
            if (success == False):
                invalid_tc.append((line, suite, name, message))
                continue
            for platform in platforms:
                if not platform in TEST_PLATFORM:
                    invalid_tc.append((line, suite, name, 'Invalid Platform Definition: ' + platform))
                    continue
                #print ('start%s' % test_spec)
                if not platform in test_spec:
                    test_spec[platform] = dict()
                #print ('end%s' % test_spec)
                for build_type in platforms[platform]:

                    if build_type not in test_spec[platform]:
                        test_spec[platform][build_type] = dict()

                    transport_value = platforms[platform][build_type]

                    if transport_value == 1:
                        if not NO_TRANSPORT in test_spec[platform][build_type]:
                            test_spec[platform][build_type][NO_TRANSPORT] = dict()
                            if not NO_NETWORK in test_spec[platform][build_type][NO_TRANSPORT]:
                                test_spec[platform][build_type][NO_TRANSPORT][NO_NETWORK] = list()
                        test_spec[platform][build_type][NO_TRANSPORT][NO_NETWORK].append(spec)
                   
                    valid_transport_value = True
                    for t_name in TRANSPORT_VALUES:
                        t_value = TRANSPORT_VALUES[t_name]
                        if type(transport_value) is int:
                            if (transport_value & t_value) == t_value:
                                if not t_name in test_spec[platform][build_type]:
                                    test_spec[platform][build_type][t_name] = dict()
                                if t_name == TEST_TRANSPORT.IP or t_name == TEST_TRANSPORT.TCP:
                                    if not TEST_NETWORK.WIFI in test_spec[platform][build_type][t_name]:
                                        test_spec[platform][build_type][t_name][TEST_NETWORK.WIFI] = list()
                                    test_spec[platform][build_type][t_name][TEST_NETWORK.WIFI].append(spec)
                                    if platform == TEST_PLATFORM.LINUX:
                                        if not TEST_NETWORK.ETH in test_spec[platform][build_type][t_name]:
                                            test_spec[platform][build_type][t_name][TEST_NETWORK.ETH] = list()
                                        test_spec[platform][build_type][t_name][TEST_NETWORK.ETH].append(spec)
                                else:
                                    if not NO_NETWORK in test_spec[platform][build_type][t_name]:
                                        test_spec[platform][build_type][t_name][NO_NETWORK] = list()
                                    test_spec[platform][build_type][t_name][NO_NETWORK].append(spec)
                        else:
                            valid_transport_value = False

                    if not valid_transport_value:
                        print ('check transport_value: ' + str(transport_value))
                        print (path)

        return test_spec, invalid_tc

    def analyze_tc_file_without_validation(self, path):
        if (os.path.isfile(path) == False):
            return

        test_spec = list()
        for line, suite, name, comments, platforms in self.analyzer(path):
            spec = TestSpec(line, suite, name)

            test_spec.append(spec)

        return test_spec


class TestSpec:


    COMMENT_PATTERN = re.compile(r'\s+@(?P<tag>\w+)\s+', re.DOTALL)

    def __init__(self, line, suite, name):
        self.line = line
        self.suite = suite
        self.name = name

        for tag in list(TAG_DIC):
            self.__dict__[tag] = ''

    def to_string(self):
        result = "%s.%s(line: %d)\n" %(self.suite, self.name, self.line)
        for key, title in sorted(TAG_DIC.items(), key=operator.itemgetter(1)):
            if (self.__dict__[key] == None or title[1] == ''):
                continue
            result += "\t%-15s : %s\n" %(title[1], self.__dict__[key].replace("\n", "\t\n                      "))

        return result

    def check_tc_naming_rule(self):
        if (self.suite == '' or self.name == ''):
            return False, 'Empty Test Suite Name or Empty Test Case Name'

        if (self.suite.startswith(TESTSUITE_MODULES) == False):
            return False, 'Invalid Test Suite Module Name: ' + self.suite

        if (self.name.find('_') >= 0):
            if (self.name.endswith(TESTCASE_CATEGORY) == False):
                return False, 'Invalid Test Case Module Name: ' + self.name

        return True, '';

    def parse_comment_tag(self, comments, tag, start, end):
        description = comments[start : end].strip()
        description = re.sub(r'\n\s+', '\n', description).strip()
        description = re.sub(r'(^\*+\s*|\n\*+\s*)', '\n', description).strip()
        description = re.sub(r'(\n/)', '', description).strip()

        if (tag in self.__dict__):
            self.__dict__[tag] = '\n'.join([self.__dict__[tag], description]).strip()
        else :
            return False

        #print('tag: ' + tag + ', description: ' + self.__dict__[tag])

        return True

    def parse_comment(self, comments):
        tag = ''
        description_start = 0

        for match in self.COMMENT_PATTERN.finditer(comments):
            if (tag != ''):
                description_end = match.start()
                self.parse_comment_tag(comments, tag, description_start, description_end)

            tag = match.group('tag')
            description_start = match.end()

        description_end = len(comments)
        self.parse_comment_tag(comments, tag, description_start, description_end)

        # Check empty tags...
        empty_tags = list()
        for tag in list(TAG_DIC):
            if (not tag in self.__dict__):
                continue

            if tag == 'reviewer':
                continue

            #if (tag != 'reviewer' and self.__dict__[tag] == ''):
            if (self.__dict__[tag] == ''):
                empty_tags.append(tag)

        if (len(empty_tags) != 0):
            return False, 'There are empty tags : ' + ', '.join(empty_tags)

        return True, ''

