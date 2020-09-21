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
 *
 ******************************************************************/

#ifndef CSCsdkUtilityHelper_H_
#define CSCsdkUtilityHelper_H_

#ifdef __GNUC__
#pragma GCC system_header
#endif

#include <stddef.h>
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#ifdef __LINUX__
#include <execinfo.h>
#endif
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "SampleResource.h"
#include "ResourceHelper.h"
#include "OCPlatform.h"
#include "OCApi.h"

#include "casecurityinterface.h"
#include "cathreadpool.h"
#include "logger.h"
#include "occloudprovisioning.h"
#include "ocpayload.h"
#include "ocprovisioningmanager.h"
#include "ocstack.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "oxmjustworks.h"
#include "oxmrandompin.h"
#include "OCAccountManager.h"
#include "OCApi.h"
#include "OCPlatform.h"
#include "OCPlatform_impl.h"
#include "payload_logging.h"
#include "pmtypes.h"
#include "RDClient.h"
#include "rd_client.h"
#include "srmutility.h"
#include "ssl_ciphersuites.h"
#include "utils.h"
#include "utlist.h"

#include "Configuration.h"
#include "CommonUtil.h"
#include "CommonTestUtil.h"
#include "CloudCommonUtil.h"
#include "CommonProperties.h"
#include "IotivityTest_Logger.h"

using namespace std;
using namespace OC;

#define TAG "CS C"

#define CTX_CERT_REQ_ISSUE "Cert Request Context"
#define CTX_CREATE_ACL "Create Acl"
#define CTX_DELETE_ACL "Delete Acl"
#define CTX_ACES_DELETE "Delete Aces"
#define CTX_INDIVIDUAL_ACE_DELETE "Delete Individual Ace"
#define CTX_GET_ACL_ID_BY_DEV "Get Acl Id By Dev Context"
#define CTX_INDIVIDUAL_GET_INFO "Individual Get Info"
#define CTX_INDIVIDUAL_ACL_UPDATE "Individual ACL update"
#define CTX_INDIVIDUAL_UPDATE_ACE "Individual Update Ace"
#define CTX_INDIVIDUAL_UPDATE "Individual Update"
#define CTX_PROV_TRUST_CERT "Provision Trust Cert"
#define CTX_GET_CRL "Get CRL"
#define CTX_POST_CRL "Post CRL"
#define CTX_CREATE_GROUP "Create Group"
#define CTX_FIND_GROUP "Find Group"
#define CTX_DELETE_GROUP "Delete Group"
#define CTX_GET_GROUP_INFO "Get Group Info"
#define CTX_INVITE_USER "Invite User in Group"
#define CTX_SHARE_DEVICE "Share Device"
#define CTX_DELETE_SHARED_DEVICE "Delete Shared Device"
#define CTX_GET_GROUP_INVITATION "Get Group Invitation"
#define CTX_DELETE_INVITATION "Delete Invitation"
#define CTX_JOIN_GROUP "Join Group"
#define CTX_OBSERVER_GROUP "Observe Group"
#define ERROR_SIGN_IN "Sign In Fail"

#define DEFAULT_GROUP_ID "77777777-5069-6E44-6576-557569643030"
#define GROUP_MASTER_ID "11111111-5069-6E44-6576-557569643030"
#define GROUP_MEMBER_ID_01 "22222222-5069-6E44-6576-111111111111"
#define GROUP_MEMBER_ID_02 "22222222-5069-6E44-6576-222222222222"
#define GROUP_DEVICE_ID_01 "88888888-5069-6E44-6576-111111111111"
#define GROUP_DEVICE_ID_02 "88888888-5069-6E44-6576-222222222222"
#define GROUP_TYPE_PUBLIC "public"
#define DEFAULT_GROUP_ID "66665555-5069-6E44-6576-222222222222"

#define ACL_DELETE_DEVICE_ID "22222222-5069-6E44-6576-00000000000"
#define DEFAULT_DEV_ID_APP "99999999-5069-6E44-6576-00000000000"
/*
 * Aces
 */
#define MAX_ID_LENGTH       (64)
#define SUBJECT_ID_EXAMPLE "72616E64-5069-6E44-6576-557569643030"
#define ACE_ID_EXAMPLE "55556666-5069-6E44-6576-557569643030"
#define RESOURCE_URI_EXAMPLE "/a/light/0"
#define RESOURCE_TYPE_EXAMPLE "core.light"
#define INTERFACE_EXAMPLE "oic.if.baseline"

#define DEFAULT_DEV_ID_ACL_DELETE "99999999-0000-0000-0000-0001301"
#define DEFAULT_DEV_ID_ACES_DELETE "99999999-0000-0000-0000-0002301"
#define DEFAULT_DEV_ID_INDIVIDUAL_ACE_DELETE "99999999-0000-0000-0000-0003301"

static int memberNumber = 2;
static char* memberIDs[2] =
{ GROUP_MEMBER_ID_01, GROUP_MEMBER_ID_02 };

static int deviceNumber = 2;
static char* deviceIDs[2] =
{ GROUP_DEVICE_ID_01, GROUP_DEVICE_ID_02 };

static int groupNumber = 1;
static char* groupIds[1] =
{ DEFAULT_GROUP_ID };

class CSCsdkUtilityHelper
{

public:

    static OCDevAddr getOCDevAddrEndPoint();

    static cloudAce_t* createCloudAces();

    static char *getOCStackResult(OCStackResult ocstackresult);

    static OCProvisionDev_t* getDevInst(OCProvisionDev_t* dev_lst, const int dev_num);

    static int printDevList(OCProvisionDev_t*);

    static int printResultList(const OCProvisionResult_t*, const int);

    static int waitCallbackRet(void);

    static size_t printUuidList(const OCUuidList_t*);

    static void printUuid(const OicUuid_t*);

    static std::string readfile(std::string filename);

    static std::string setFailureMessage(OCStackResult actualResult, OCStackResult expectedResult);

    static std::string setFailureMessage(std::string errorMessage);
};

#endif /* CSCsdkUtilityHelper_H_ */
