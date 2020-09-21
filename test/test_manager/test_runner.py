#!/usr/bin/python3
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
import sys
import os
import optparse
import subprocess
import re
import platform
import configuration
from configuration import *
from ite.exec.runner import TestRunner, TestRunnerOption
from ite.reporter.tc_reporter import TestSpecReporter
from ite.webreporter.tc_list_reporter import TCListReporter
from ite.tc.container import TestSpecContainer

oparser = optparse.OptionParser()
oparser.add_option("-f", action="store", dest="file_filter")
oparser.add_option("--filter", action="store", dest="file_filter")
oparser.add_option("-m", action="store", dest="module")
oparser.add_option("--module", action="store", dest="module")
oparser.add_option("-p", action="store", dest="platform")
oparser.add_option("--platform", action="store", dest="platform")
oparser.add_option("-x", action="store", dest="transport", default="")
oparser.add_option("--transport", action="store", dest="transport", default="")
oparser.add_option("-n", action="store", dest="network", default="")
oparser.add_option("--network", action="store", dest="network", default="")
oparser.add_option("-t", action="store", dest="target")
oparser.add_option("--target", action="store", dest="target")
oparser.add_option("-l", action="store", dest="testlist")
oparser.add_option("--list", action="store", dest="testlist")
oparser.add_option("-s", action="store", dest="testprogress")
oparser.add_option("--progress", action="store", dest="testprogress")
oparser.add_option("-r", action="store", dest="testresult")
oparser.add_option("--result", action="store", dest="testresult")
oparser.add_option("-a", action="store_true", dest="standalone")
oparser.add_option("--standalone", action="store_true", dest="standalone")
oparser.add_option("-o", action="store_true", dest="runonce")
oparser.add_option("--runonce", action="store_true", dest="runonce")
oparser.add_option("-u", action="store", dest="testsuite")
oparser.add_option("--testsuite", action="store", dest="testsuite")
oparser.add_option("-c", action="store", dest="testcase")
oparser.add_option("--testcase", action="store", dest="testcase")


oparser.add_option("--memcheck", action = "store", dest = "memcheck", default= "0")
oparser.add_option("-v", action= "store", dest= "memcheck", default="0")
oparser.add_option("--memcheckLocation", action="store", dest ="memcheckLocation", default = "")
oparser.add_option("-g", action="store", dest ="memcheckLocation", default = "")

oparser.set_defaults(file_filter='', platform=platform.system().lower(), target='', testlist='', testprogress='', testresult='',
                     standalone=TEST_STANDALONE, runonce=False)

opts, args = oparser.parse_args()
file_filter = opts.file_filter
module = opts.module
platform = opts.platform
transport = opts.transport
network = opts.network
target = opts.target
testlist = opts.testlist
testprogress = opts.testprogress
testresult = opts.testresult
standalone = opts.standalone
runonce = opts.runonce
given_testsuite = opts.testsuite
memcheckOn = opts.memcheck
memcheckLocation = opts.memcheckLocation

if testresult == '':
    testresult = TEST_RESULT_RUN_DIR

if not os.path.exists(testresult):
    os.makedirs(testresult)

if not testlist == '':
    list_analyzer = TCListReporter()
    testgroup = list_analyzer.analyze(testlist)

if memcheckOn == "1":
    if memcheckLocation =='':
        memcheckLocation = MEMCHECK_RESULT_RUN_DIR
    if not os.path.exists(memcheckLocation):
        os.makedirs(memcheckLocation)

testspec_path = os.path.join(testresult, TEST_SPEC_XML_FOR_RESULT)
if not os.path.exists(testspec_path) and os.path.exists(API_TC_SRC_DIR):
    container = TestSpecContainer()
    container.extract_api_testspec(API_TC_SRC_DIR, '')

    reporter = TestSpecReporter()
    reporter.generate_testspec_report(container.data)
    reporter.report('XML', testspec_path)

configuration.EXECUTION_LOG_FP = open(testresult + os.sep + EXECUTION_LOG_FILE, "w")

file_filter = file_filter.replace(TC_BIN_PREFIX, '')
total_found_tc = 0
runner = TestRunner()
for fname in os.listdir(TC_BIN_DIR):
    if "." in fname:
        if fname.endswith('.exe'):
            fname = fname.split('.')[0]
        else:
            continue

    if not fname.startswith(TC_BIN_PREFIX):
        continue

    if not fname.endswith(TC_BIN_SUFFIX):
        continue

    if module and module != fname.split('_')[1]:
        continue

    if file_filter and file_filter not in fname:
        continue

    tname = fname.replace(TC_BIN_PREFIX, '')

    option = TestRunnerOption()

    current_filter = ''
    if not testlist == '':
        for tcfile_filter in list(testgroup):
            if tcfile_filter in fname:
                current_filter = tcfile_filter
                break

        if current_filter == '':
            continue

        option.testset = set(testgroup[current_filter].keys())
        option.testkey = testgroup[current_filter]

    elif (file_filter != '') and (not file_filter in tname):
        continue

    filepath = os.path.join(TC_BIN_DIR, fname)

    print("### Start to run : " + filepath)

    if given_testsuite:
        given_testsuites = opts.testsuite.split(',')
    else:
        given_testsuites = []

    if opts.testcase:
        given_testcases = opts.testcase.split(',')
    else:
        given_testcases = []

    if option.testset == None:
        if platform == 'linux':
            command = ["./%s" % os.path.basename(filepath), '--gtest_list_tests']
            process = subprocess.Popen(command, cwd=os.path.dirname(filepath), stdout=subprocess.PIPE)
        if platform == 'windows':
            filepath = filepath+'.exe'
            command = ["%s" % os.path.basename(filepath), '--gtest_list_tests']
            process = subprocess.Popen(command, cwd=os.path.dirname(filepath), stdout=subprocess.PIPE, shell= True)
        testset = set()
        testsuite = ''

        for line in process.stdout.readlines():
            line = re.sub(r'(b\'|\')', '', str(line.strip()))
            if line.endswith('.'):
                testsuite = line
                continue
            if given_testsuites:
                for ts in given_testsuites:

                    if testsuite != (ts+'.'):
                        continue

                    if (testsuite != '' and line != ''):
                        if not given_testcases:
                            testset.add(testsuite +line)
                        else:
                            for tc in given_testcases:
                                if ( line == tc ):
                                    testset.add(testsuite +line)
            elif given_testcases:
                for tc in given_testcases:
                    if ( line == tc):
                        testset.add(testsuite +line)
            else:
                testset.add(testsuite +line)

    if not testset:
        continue

    total_found_tc = total_found_tc + len(testset)

    option.testset = testset
    option.exe_path = filepath
    option.platform = platform
    option.target = target
    option.transport = transport
    option.network = network
    option.result_dir = testresult
    option.runtime = TEST_REPEAT
    option.run_standalone = standalone
    option.testprogress_path = testprogress
    option.repeat_failed = TEST_REPEAT_FAILED
    option.repeat_crashed = TEST_REPEAT_CRASHED
    if runonce == True:
        option.repeat_failed = False
        option.repeat_crashed = False

    runner.run_test_executable(option)

if total_found_tc == 0:
    print ('No testcase Found')
    exit(0)

print("### Test Is Done!!")
