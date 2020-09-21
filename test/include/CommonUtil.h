/******************************************************************
*
* Copyright 2016 Samsung Electronics All Rights Reserved.
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

#ifndef __COMMON_UTIL_H__
#define __COMMON_UTIL_H__
#include "iotivity_config.h"
#include <malloc.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <string>
#include <fstream>
#ifdef __GNUC__
#include <ifaddrs.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_WINDOWS_H
#undef _WINSOCKAPI_
#define _WINSOCKAPI_
#include <windows.h>
#include <direct.h>
#include <process.h>
#include <io.h>
#define F_OK    0
#define popen    _popen
#define pclose    _pclose
#define getpid    _getpid
#define fseeko    _fseeki64
#define ftello    _ftelli64
#endif
#include <ctime>
#include <iostream>
#include <sstream>
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#undef BOOST_NO_CXX11_SCOPED_ENUMS
#include "boost/assign.hpp"
#include "octypes.h"
#include "platform_features.h"
//#include "gtest_custom.h"
//#include "gtest/gtest.h"

#define APP_START_STOP_TIMEOUT 2
#define WAIT_TIME_MIN 1
using namespace std;
static const string KILL_ALL_COMMAND = "kill -9 ";
static const string PID_FIND_COMMAND = "/usr/bin/lsof -t -i:";

typedef enum
{
    IPv4 = 0, IPv6
} InternetProtocolVersion;

class CommonUtil
{
    static long s_setUpDynamicMemoryUsage;
    static long s_tearDownDynamicMemoryUsage;
    static long s_memoryDiffSum;


public:

    static map<OCStackResult, string> s_OCStackResultString;

    /**
     * API for Get OCSTACK result in char*
     *
     * @return char*
     */
    static char* getOCStackResult(OCStackResult ocstackresult);

    /**
     * API for get time stamp
     *
     * @return const char*
     *
     */
    static const char* GetTimeStampString();

    /**
     * API for launch App
     *
     *
     * @param app - App binary path.
     * @param withGnome - Open in gnome terminal
     *
     * @return none
     *
     * NOTE: This API will be support other platforms
     */
//    static void launchApp(std::string app);
    static void launchApp(std::string app, bool withGnome = false);

    /**
     * API for kill App
     *
     *
     * @param app - App process name.
     *
     * @return none
     *
     * NOTE: This API will be support other platforms
     */
    static void killApp(std::string app);

    /**
     * API for make directory
     *
     *
     * @param app - App binary path.
     *
     * @return none
     *
     * NOTE: This API will be support other platforms
     */
    static void mkDir(std::string dir);

    /**
     * API for Launch App
     *
     *
     * @param app - App binary path.
     *
     * @return none
     *
     * NOTE: This API will be support other platforms
     */
    static void rmDir(std::string dir);

    /**
     * API for copyfile
     *
     *
     * @param app - App binary path.
     *
     * @return none
     *
     * NOTE: This API will be support other platforms
     */
    static void copyFile(std::string source, std::string destination);

    /**
     * API for remve file
     *
     *
     * @param app - App binary path.
     *
     * @return none
     *
     * NOTE: This API will be support other platforms
     */
    static void rmFile(std::string file);

    /**
     * API for check existing
     *
     *
     * @param app - App binary path.
     *
     * @return bool
     *
     * NOTE: This API will be support other platforms
     */
    static bool isExists(std::string file);

    /**
     * API for freezing current thread from execute next code for a given time
     *
     * @param seconds no of seconds to wait for
     *
     * @return bool
     *
     * NOTE: This API will be support other platforms
     */
    static void waitInSecond(unsigned int seconds);

    /**
     * API for creating file in the specified path with specified file content
     *
     * @param filePath - File path
     *
     * @param fileContent - The Content that will be written in the file
     *
     * @return bool - if Successfully writes the file, returns true, otherwise false
     *
     * NOTE: This API will be supported in other platforms
     */
    static bool writeFile(std::string filePath, std::string fileName, std::string fileContent);

    /**
     * API for Getting Local IP
     *
     * @param ipVersion - InternetProtocolVersion enum
     *
     * @return string - ip of the local machine
     *
     * NOTE: This API will be supported in other platforms
     */
    static std::string getLocalIP(InternetProtocolVersion ipVersion);

    /**
     * API for Getting Local IP
     *
     * @param ipVersion - InternetProtocolVersion enum
     *
     * @return string - ip of the local machine
     *
     * NOTE: This API will be supported in other platforms
     */
    static std::string getLocalIPAll(InternetProtocolVersion ipVersion, std::string token);

    /**
     * API for Getting current system time
     *
     * @param [out] currentTime - struct tm type object
     *
     */
    static void getCurrentTime(struct tm& currentTime);

    /**
     * API for Getting Process Id from Port
     *
     * @param [in] port - Port Number of a process
     *
     */
    static string getPIDFromPort(string port);

    /**
     * API for Kiling a Process
     *
     * @param [in] pid - pid of a process
     *
     */
    static void killPID(string pid);
};
#endif
