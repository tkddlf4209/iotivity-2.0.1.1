//******************************************************************
//
// Copyright 2015 Intel Mobile Communications GmbH All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <gtest/gtest.h>
#include <coap/utlist.h>
#include <sys/stat.h>

extern "C" {
#include "ocstack.h"
#include "psinterface.h"
#include "ocpayload.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "cainterface.h"
#include "secureresourcemanager.h"
#include "srmresourcestrings.h"
#include "pstatresource.h"
#include "srmutility.h"
#include "experimental/logger.h"
#include "experimental/doxmresource.h"
#include "ocpayload.h"
#include "ocpayloadcbor.h"
#include "experimental/payload_logging.h"
#include "security_internals.h"
#include "acl_logging.h"

#include "../src/aclresource.c"
#undef TAG
}

#include "srmtestcommon.h"

using namespace std;

#define TAG  "SRM-ACL-UT"

// These paths match jenkins build configuration.
const char* DEFAULT_ACL_FILE_NAME = "oic_unittest_default_acl.dat";
const char* ACL1_FILE_NAME = "oic_unittest_acl1.dat";

#define NUM_ACE_FOR_ANON_CLEAR_IN_DEFAULT_ACL (2)

class SRM_ACL : public ::testing::Test
{
    public:
        static void SetUpTestCase()
        {
            EXPECT_EQ(OC_STACK_INVALID_PARAM, InitACLResource());
        }
        static void TearDownTestCase()
        {
            EXPECT_EQ(OC_STACK_INVALID_PARAM, DeInitACLResource());
        }
};


static bool AddResourceToACE(OicSecAce_t* ace, const char* rsrcName,
                             const char* typeName, const char* interfaceName)
{
    OicSecRsrc_t* rsrc = NULL;

    VERIFY_NOT_NULL(TAG, ace, ERROR);
    VERIFY_NOT_NULL(TAG, rsrcName, ERROR);
    VERIFY_NOT_NULL(TAG, interfaceName, ERROR);
    VERIFY_NOT_NULL(TAG, typeName, ERROR);

    rsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
    VERIFY_NOT_NULL(TAG, rsrc, ERROR);
    rsrc->href = OICStrdup(rsrcName);
    VERIFY_NOT_NULL(TAG, rsrc->href, ERROR);

    rsrc->typeLen = 1;
    rsrc->types = (char**)OICCalloc(1, sizeof(char*));
    VERIFY_NOT_NULL(TAG, rsrc->types, ERROR);
    rsrc->types[0] = OICStrdup(typeName);
    VERIFY_NOT_NULL(TAG, rsrc->types[0], ERROR);

    rsrc->interfaceLen = 1;
    rsrc->interfaces = (char**)OICCalloc(1, sizeof(char*));
    VERIFY_NOT_NULL(TAG, rsrc->interfaces, ERROR);
    rsrc->interfaces[0] = OICStrdup(interfaceName);
    VERIFY_NOT_NULL(TAG, rsrc->interfaces[0], ERROR);

    LL_APPEND(ace->resources, rsrc);
    return true;
exit:
    if(rsrc)
    {
        if(rsrc->href)
        {
            OICFree(rsrc->href);
            OICFree(rsrc->types[0]);
            OICFree(rsrc->types);
            OICFree(rsrc->interfaces[0]);
            OICFree(rsrc->interfaces);
        }
        OICFree(rsrc);
    }
    return false;
}

static size_t GetNumberOfResource(const OicSecAce_t* ace)
{
    size_t ret = 0;
    OicSecRsrc_t* rsrc = NULL;
    LL_FOREACH(ace->resources, rsrc)
    {
        ret++;
    }

    return ret;
}

TEST_F(SRM_ACL, CBORDefaultACLConversion)
{
    uint8_t defaultAclSub[] = {0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
        0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31};
    uint8_t defaultAclOwnrs[] = {0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
        0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32};

    OicSecAcl_t *defaultAcl = (OicSecAcl_t *) OICCalloc(1, sizeof(OicSecAcl_t));
    ASSERT_TRUE(NULL != defaultAcl);
    OicSecAce_t *ace = (OicSecAce_t*)OICCalloc(1, sizeof(OicSecAce_t));
    ASSERT_TRUE(NULL != ace);
    ace->permission = 2;
    memcpy(ace->subjectuuid.id, defaultAclSub, sizeof(defaultAclSub));
    LL_APPEND(defaultAcl->aces, ace);

    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/res", "oic.wk.res", "oic.if.ll"));
    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/d", "oic.wk.d", "oic.if.r"));
    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/p", "oic.wk.p", "oic.if.r"));
    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/res/types/d", "oic.wk.unknow", "oic.if.r"));
    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/ad", "oic.wk.ad", "oic.if.baseline"));
    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/sec/acl2", "oic.r.acl2", "oic.if.baseline"));
    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/sec/doxm", "oic.r.doxm" ,"oic.if.baseline"));
    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/sec/pstat", "oic.r.pstat" ,"oic.if.baseline"));

    memcpy(defaultAcl->rownerID.id, defaultAclOwnrs, sizeof(defaultAclOwnrs));

    size_t defaultAclSize = 0;
    uint8_t *defaultPsStorage = NULL;
    OCStackResult convRet = AclToCBORPayload(defaultAcl, OIC_SEC_ACL_V2, &defaultPsStorage, &defaultAclSize);
    EXPECT_EQ(OC_STACK_OK, convRet);
    ASSERT_TRUE(NULL != defaultPsStorage);
    EXPECT_NE(static_cast<size_t>(0), defaultAclSize);

    OicSecAcl_t* convertedAcl = CBORPayloadToAcl(defaultPsStorage, defaultAclSize);
    ASSERT_TRUE(NULL != convertedAcl);

    size_t rsrcCnt1 = 0;
    size_t rsrcCnt2 = 0;
    OicSecAce_t* tempAce = NULL;
    LL_FOREACH(defaultAcl->aces, tempAce)
    {
        rsrcCnt1 += GetNumberOfResource(tempAce);
    }
    tempAce = NULL;
    LL_FOREACH(convertedAcl->aces, tempAce)
    {
        rsrcCnt2 += GetNumberOfResource(tempAce);
    }
    EXPECT_EQ(rsrcCnt1, rsrcCnt2);

    DeleteACLList(convertedAcl);
    DeleteACLList(defaultAcl);
    OICFree(defaultPsStorage);
}

TEST_F(SRM_ACL, CBORACLConversion)
{
    uint8_t ownrs[] = {0x32, 0x32, 0x32, 0x32,
                       0x32, 0x32, 0x32, 0x32,
                       0x32, 0x32, 0x32, 0x32,
                       0x32, 0x32, 0x32, 0x32};
    const char* subjectUuid[3] = {"0000000000000000",
                                  "1111111111111111",
                                  "2222222222222222"};
    const uint16_t permission[3] = {2, 6, 31};
    const size_t numOfRsrc[3] = {6, 2, 2};

    OicSecAcl_t *secAcl = (OicSecAcl_t *) OICCalloc(1, sizeof(OicSecAcl_t));
    ASSERT_TRUE(secAcl != NULL);
    memcpy(secAcl->rownerID.id, ownrs, sizeof(ownrs));

    OicSecAce_t *ace = (OicSecAce_t*)OICCalloc(1, sizeof(OicSecAce_t));
    ASSERT_TRUE(NULL != ace);
    ace->permission = permission[0];
    memcpy(ace->subjectuuid.id, subjectUuid[0], strlen(subjectUuid[0]));
    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/res", "oic.wk.res", "oic.if.ll"));
    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/d", "oic.wk.d", "oic.if.r"));
    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/p", "oic.wk.p", "oic.if.r"));
    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/res/types/d", "oic.wk.unknow", "oic.if.r"));
    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/ad", "oic.wk.ad", "oic.if.baseline"));
    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/sec/acl2", "oic.r.acl2", "oic.if.baseline"));
    LL_APPEND(secAcl->aces, ace);

    OicSecAce_t *ace1 = (OicSecAce_t*)OICCalloc(1, sizeof(OicSecAce_t));
    ASSERT_TRUE(NULL != ace1);
    ace1->permission = permission[1];
    memcpy(ace1->subjectuuid.id, subjectUuid[1], strlen(subjectUuid[1]));
    EXPECT_EQ(true, AddResourceToACE(ace1, "/oic/sec/doxm", "oic.r.doxm" ,"oic.if.baseline"));
    EXPECT_EQ(true, AddResourceToACE(ace1, "/oic/sec/pstat", "oic.r.pstat" ,"oic.if.baseline"));
    LL_APPEND(secAcl->aces, ace1);

    OicSecAce_t *ace2 = (OicSecAce_t*)OICCalloc(1, sizeof(OicSecAce_t));
    ASSERT_TRUE(NULL != ace2);
    ace2->permission = permission[2];
    memcpy(ace2->subjectuuid.id, subjectUuid[2], strlen(subjectUuid[2]));
    EXPECT_EQ(true, AddResourceToACE(ace2, "/oic/light", "oic.core", "oic.if.baseline"));
    EXPECT_EQ(true, AddResourceToACE(ace2, "/oic/garage", "oic.core", "oic.if.baseline"));
    LL_APPEND(secAcl->aces, ace2);

    size_t size = 0;
    uint8_t *psStorage = NULL;
    EXPECT_EQ(OC_STACK_OK, AclToCBORPayload(secAcl, OIC_SEC_ACL_V1, &psStorage, &size));
    ASSERT_TRUE(NULL != psStorage);
    OicSecAcl_t *acl = CBORPayloadToAcl(psStorage, size);
    ASSERT_TRUE(NULL != acl);

    size_t numberOfCheckedAce = 0;
    OicSecAce_t* tempAce = NULL;
    LL_FOREACH(acl->aces, tempAce)
    {
        if(memcmp(tempAce->subjectuuid.id, subjectUuid[0], strlen(subjectUuid[0])) == 0)
        {
            EXPECT_EQ(numOfRsrc[0], GetNumberOfResource(tempAce));
            EXPECT_EQ(permission[0], tempAce->permission);
            numberOfCheckedAce++;
        }
        if(memcmp(tempAce->subjectuuid.id, subjectUuid[1], strlen(subjectUuid[1])) == 0)
        {
            EXPECT_EQ(numOfRsrc[1], GetNumberOfResource(tempAce));
            EXPECT_EQ(permission[1], tempAce->permission);
            numberOfCheckedAce++;
        }
        if(memcmp(tempAce->subjectuuid.id, subjectUuid[2], strlen(subjectUuid[2])) == 0)
        {
            EXPECT_EQ(numOfRsrc[2], GetNumberOfResource(tempAce));
            EXPECT_EQ(permission[2], tempAce->permission);
            numberOfCheckedAce++;
        }
    }
    EXPECT_EQ(3u, numberOfCheckedAce);

    DeleteACLList(acl);
    DeleteACLList(secAcl);
    OICFree(psStorage);
}

// Default ACL tests
TEST_F(SRM_ACL, GetDefaultACLTests)
{
    uint8_t *payload = NULL;
    size_t size = 0;

    ASSERT_TRUE(ReadCBORFile(DEFAULT_ACL_FILE_NAME, OIC_JSON_ACL_NAME, &payload, &size));
    ASSERT_TRUE(NULL != payload);

    OicSecAcl_t *psAcl = CBORPayloadToAcl(payload, size);
    ASSERT_TRUE(NULL != psAcl);

    OicSecAcl_t *acl = NULL;
    EXPECT_EQ(OC_STACK_OK, GetDefaultACL(&acl));
    ASSERT_TRUE(NULL != acl);

    // Verify if the SRM generated default ACL matches with unit test default
    if (acl && psAcl)
    {
        OicSecAce_t* tempAce1 = NULL;
        OicSecAce_t* tempAce2 = NULL;

        for(tempAce1 = acl->aces, tempAce2 = psAcl->aces;
            tempAce1 && tempAce2; tempAce1 = tempAce1->next,
            tempAce2 = tempAce2->next)
        {
            EXPECT_TRUE(memcmp(tempAce1->subjectuuid.id, tempAce2->subjectuuid.id, sizeof(tempAce1->subjectuuid.id)) == 0);
            EXPECT_EQ(tempAce1->permission, tempAce2->permission);
            EXPECT_EQ(GetNumberOfResource(tempAce1), GetNumberOfResource(tempAce2));
        }
    }

    DeleteACLList(psAcl);
    DeleteACLList(acl);
    OICFree(payload);
}

// 'POST' ACL tests
TEST_F(SRM_ACL, ACLPostTest)
{
    uint8_t *payload = NULL;
    size_t size = 0;
    // Intialize /pstat global, so that the GetDos() calls in aclresource.c
    // can succeed, or all UPDATE requests will be rejected based on DOS.
    /*
    OCStackResult res = InitPstatResourceToDefault();
    ASSERT_TRUE(OC_STACK_OK == res);

    // Read an ACL from the file
    static OCPersistentStorage ps = OCPersistentStorage();
    SetPersistentHandler(&ps, true);
*/
    OicSecAcl_t *defaultAcl = NULL;
    EXPECT_EQ(OC_STACK_OK, GetDefaultACL(&defaultAcl));
    ASSERT_TRUE(defaultAcl != NULL);
    EXPECT_EQ(OC_STACK_OK, SetDefaultACL(defaultAcl));

    ASSERT_TRUE(ReadCBORFile(ACL1_FILE_NAME, OIC_JSON_ACL_NAME, &payload, &size));
    ASSERT_TRUE(NULL != payload);

    OCSecurityPayload *securityPayload = OCSecurityPayloadCreate(payload, size);
    ASSERT_TRUE(NULL != securityPayload);

    // Create Entity Handler POST request payload
    OCEntityHandlerRequest ehReq = OCEntityHandlerRequest();
    ehReq.method = OC_REST_POST;
    ehReq.payload = (OCPayload *) securityPayload;

    ACLEntityHandler(OC_REQUEST_FLAG, &ehReq, NULL);
    OicSecAcl_t *acl = CBORPayloadToAcl(payload, size);
    ASSERT_TRUE(NULL != acl);

    // Verify /acl2 Resource contains an ACE for a subject in acl local var
    OicSecAce_t *savePtr = NULL;
    savePtr = acl->aces;
    while(OicSecAceUuidSubject != savePtr->subjectType)
    {
        savePtr = savePtr->next;
    }
    OicUuid_t uuid = savePtr->subjectuuid;
#ifndef NDEBUG
    char uuidString[UUID_STRING_SIZE] = { 0 };
    bool convertedUUID = OCConvertUuidToString(uuid.id, uuidString);
    if (convertedUUID)
    {
        printf("asubjectuuidToFind.id = %s", uuidString);
    }
#endif
    savePtr = NULL;
    const OicSecAce_t* subjectAcl = GetACLResourceData(&uuid, &savePtr);
    ASSERT_FALSE(NULL != subjectAcl);

    // Perform cleanup
    OICFree(payload);
    OCPayloadDestroy((OCPayload *) securityPayload);
    DeleteACLList(acl);
}

extern "C" {
    // gAcl is a pointer to the the global ACL used by SRM
    extern OicSecAcl_t  *gAcl;
}

// GetACLResource tests
TEST_F(SRM_ACL, GetACLResourceTests)
{
    OicSecAcl_t *acl1 = NULL;
    EXPECT_EQ(OC_STACK_OK, GetDefaultACL(&acl1));
    ASSERT_TRUE(acl1 != NULL);
    EXPECT_EQ(OC_STACK_OK, SetDefaultACL(acl1));

    // Verify that the default ACL file contains 2 ACE entries for the 'ANON_CLEAR' conntype subject
    const OicSecAce_t *ace = NULL;
    OicSecAce_t *savePtr = NULL;
    OicSecConntype_t subjectConn = ANON_CLEAR;
    int count = 0;

    do
    {
        ace = GetACLResourceDataByConntype(subjectConn, &savePtr);
        count = (NULL != ace) ? count + 1 : count;
    } while (ace != NULL);

    EXPECT_EQ(count, NUM_ACE_FOR_ANON_CLEAR_IN_DEFAULT_ACL);
}

TEST_F(SRM_ACL, DefaultAclAllowsRolesAccess)
{
    /* Get and install the default ACL */
    OicSecAcl_t *acl1 = NULL;
    EXPECT_EQ(OC_STACK_OK, GetDefaultACL(&acl1));
    ASSERT_TRUE(acl1 != NULL);
    EXPECT_EQ(OC_STACK_OK, SetDefaultACL(acl1));

    /* Verify that the default ACL file allows AUTH_CRYPT RUD access to the /roles resource */
    const OicSecAce_t *ace = NULL;
    OicSecAce_t *savePtr = NULL;
    OicSecConntype_t subjectConn = AUTH_CRYPT;
    int found = 0;

    while((ace = GetACLResourceDataByConntype(subjectConn, &savePtr)) != NULL)
    {
        ASSERT_TRUE(ace->resources != NULL);
        OicSecRsrc_t* rsrc = NULL;
        LL_FOREACH(ace->resources, rsrc)
        {
            if ((strcmp(rsrc->href, OIC_RSRC_ROLES_URI) == 0) &&
                (ace->permission == (PERMISSION_READ | PERMISSION_WRITE | PERMISSION_DELETE)))
            {
                found = 1;
                break;
            }
        }
    }

    EXPECT_EQ(found, 1);
}


static OCStackResult populateAcl(OicSecAcl_t *acl,  int numRsrc)
{
    memcpy(acl->rownerID.id, "1111111111111111", sizeof(acl->rownerID.id));

    OicSecAce_t *ace = (OicSecAce_t*)OICCalloc(1, sizeof(OicSecAce_t));
    if (1 <= numRsrc)
    {
        ace->aceid = (uint16_t)rand();
        ace->permission = 2;
        memcpy(ace->subjectuuid.id, "2222222222222220", sizeof(ace->subjectuuid.id));
        EXPECT_EQ(true, AddResourceToACE(ace, "/oic/res", "oic.wk.res", "oic.if.ll"));
        EXPECT_EQ(true, AddResourceToACE(ace, "/oic/d", "oic.wk.d", "oic.if.r"));
        EXPECT_EQ(true, AddResourceToACE(ace, "/oic/p", "oic.wk.p", "oic.if.r"));
        EXPECT_EQ(true, AddResourceToACE(ace, "/oic/res/types/d", "oic.wk.unknow", "oic.if.r"));
        EXPECT_EQ(true, AddResourceToACE(ace, "/oic/ad", "oic.wk.ad", "oic.if.baseline"));
        EXPECT_EQ(true, AddResourceToACE(ace, "/oic/sec/acl2", "oic.r.acl2", "oic.if.baseline"));
/*         ace->validities = (OicSecValidity_t*)OICCalloc(1, sizeof(OicSecValidity_t));
        ace->validities->period = OICStrdup("10/10/20");
        ace->validities->recurrences = (char**)OICCalloc(1, sizeof(OicSecValidity_t));
        ace->validities->recurrences[0] = (char*)OICStrdup("rec");
        ace->validities->recurrenceLen = 1;*/
        LL_APPEND(acl->aces, ace);
        if (1 == numRsrc)
        {
            return OC_STACK_OK;
        }
    }

    if (2 <= numRsrc)
    {
        ace = (OicSecAce_t*)OICCalloc(1, sizeof(OicSecAce_t));
        ace->aceid = (uint16_t)rand();
        ace->permission = 6;
        memcpy(ace->subjectuuid.id, "2222222222222221", sizeof(ace->subjectuuid.id));
        EXPECT_EQ(true, AddResourceToACE(ace, "/oic/sec/doxm", "oic.r.doxm" ,"oic.if.baseline"));
        EXPECT_EQ(true, AddResourceToACE(ace, "/oic/sec/pstat", "oic.r.pstat" ,"oic.if.baseline"));
/*          ace->validities = (OicSecValidity_t*)OICCalloc(1, sizeof(OicSecValidity_t));
        ace->validities->period = OICStrdup("11/11/20");
        ace->validities->recurrences = (char**)OICCalloc(1, sizeof(OicSecValidity_t));
        ace->validities->recurrences[0] = (char*)OICStrdup("rec1");
        ace->validities->recurrenceLen = 1;*/
        LL_APPEND(acl->aces, ace);
        if (2 == numRsrc)
        {
            return OC_STACK_OK;
        }
    }

    ace = (OicSecAce_t*)OICCalloc(1, sizeof(OicSecAce_t));
    ace->aceid = (uint16_t)rand();
    ace->permission = 31;

    ace->subjectType = OicSecAceRoleSubject;
    memcpy(ace->subjectuuid.id, "2222222222222222", sizeof(ace->subjectuuid.id));
//    snprintf(ace->subjectRole.authority, ROLEAUTHORITY_LENGTH, "0664";
//    snprintf(ace->subjectRole.id, ROLEID_LENGTH, "11111111111111";

    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/light", "oic.core", "oic.if.baseline"));
    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/garage", "oic.core", "oic.if.baseline"));
    ace->validities = (OicSecValidity_t*)OICCalloc(1, sizeof(OicSecValidity_t));
    ace->validities->period = OICStrdup("12/12/20");
    ace->validities->recurrences = (char**)OICCalloc(1, sizeof(OicSecValidity_t));
    ace->validities->recurrences[0] = (char*)OICStrdup("rec2");
    ace->validities->recurrenceLen = 1;
    LL_APPEND(acl->aces, ace);

    return OC_STACK_OK;
}

//'DELETE' ACL test
TEST_F(SRM_ACL, ACLDeleteWithSingleResourceTest)
{
    // Intialize /pstat global, so that the GetDos() calls in aclresource.c
    // can succeed, or all UPDATE requests will be rejected based on DOS.
    OCStackResult res = InitPstatResourceToDefault();
    ASSERT_TRUE(OC_STACK_OK == res);

    OicSecAcl_t *defaultAcl = NULL;
    EXPECT_EQ(OC_STACK_OK, GetDefaultACL(&defaultAcl));
    ASSERT_TRUE(defaultAcl != NULL);
    EXPECT_EQ(OC_STACK_OK, SetDefaultACL(defaultAcl));

    //Populate ACL
    OicSecAcl_t *acl = (OicSecAcl_t *) OICCalloc(1, sizeof(OicSecAcl_t));
    ASSERT_TRUE(NULL != acl);
    EXPECT_EQ(OC_STACK_OK, populateAcl(acl, 1));

    //GET CBOR POST payload
    size_t size = 0;
    uint8_t  *payload = NULL;
    EXPECT_EQ(OC_STACK_OK, AclToCBORPayload(acl, OIC_SEC_ACL_V2, &payload, &size));
    ASSERT_TRUE(NULL != payload);

    // Security Payload
    OCSecurityPayload *securityPayload = OCSecurityPayloadCreate(payload, size);
    ASSERT_TRUE(NULL != securityPayload);

    // Create Entity Handler POST request payload
    OCEntityHandlerRequest ehReq = OCEntityHandlerRequest();
    ehReq.payload = (OCPayload *) securityPayload;
    ehReq.method = OC_REST_POST;
    ACLEntityHandler(OC_REQUEST_FLAG, &ehReq, NULL);

    // Verify if SRM contains ACE for the subject
    OicSecAce_t* savePtr = NULL;
    const OicSecAce_t* subjectAce1 = GetACLResourceData(&(acl->aces->subjectuuid), &savePtr);
    ASSERT_TRUE(NULL != subjectAce1);

    // Create Entity Handler DELETE request
    ehReq.method = OC_REST_DELETE;
    char query[] = "subjectuuid=32323232-3232-3232-3232-323232323232;resources=/a/led";
    ehReq.query = (char *)OICMalloc(strlen(query)+1);
    ASSERT_TRUE(NULL !=  ehReq.query);
    OICStrcpy(ehReq.query, strlen(query)+1, query);
    ACLEntityHandler(OC_REQUEST_FLAG, &ehReq, NULL);

    // Verify if SRM has deleted ACE for the subject
    savePtr = NULL;
    const OicSecAce_t* subjectAce2 = GetACLResourceData(&(acl->aces->subjectuuid), &savePtr);
    ASSERT_NE(nullptr, subjectAce2);

    // Perform cleanup
    DeleteACLList(acl);
    OICFree(ehReq.query);
    OCPayloadDestroy((OCPayload *)securityPayload);
    OICFree(payload);
}

TEST_F(SRM_ACL, ACLDeleteWithMultiResourceTest)
{
    // Intialize /pstat global, so that the GetDos() calls in aclresource.c
    // can succeed, or all UPDATE requests will be rejected based on DOS.
    OCStackResult res = InitPstatResourceToDefault();
    ASSERT_TRUE(OC_STACK_OK == res);

    OicSecAcl_t *defaultAcl = NULL;
    EXPECT_EQ(OC_STACK_OK, GetDefaultACL(&defaultAcl));
    ASSERT_TRUE(defaultAcl != NULL);
    EXPECT_EQ(OC_STACK_OK, SetDefaultACL(defaultAcl));

    //Populate ACL
    OicSecAcl_t *acl = (OicSecAcl_t *) OICCalloc(1, sizeof(OicSecAcl_t));
    ASSERT_TRUE(NULL != acl);
    EXPECT_EQ(OC_STACK_OK, populateAcl(acl, 2));

    //GET CBOR POST payload
    size_t size = 0;
    uint8_t *payload = NULL;
    EXPECT_EQ(OC_STACK_OK, AclToCBORPayload(acl, OIC_SEC_ACL_V2, &payload, &size));
    ASSERT_NE(nullptr, payload);

    // Security Payload
    OCSecurityPayload *securityPayload = OCSecurityPayloadCreate(payload, size);
    ASSERT_TRUE(NULL!= securityPayload);

    // Create Entity Handler POST request payload
    OCEntityHandlerRequest ehReq = OCEntityHandlerRequest();
    ehReq.method = OC_REST_POST;
    ehReq.payload = (OCPayload *)securityPayload;
    ACLEntityHandler(OC_REQUEST_FLAG, &ehReq, NULL);

    // Verify if SRM contains ACE for the subject with two resources
    OicSecAce_t* savePtr = NULL;
    const OicSecAce_t* subjectAce1 = GetACLResourceData(&(acl->aces->subjectuuid), &savePtr);
    ASSERT_TRUE(NULL != subjectAce1);
    EXPECT_EQ(6u, GetNumberOfResource(subjectAce1));

    printf("\n\n");
    OicSecRsrc_t* rsrc = NULL;
    LL_FOREACH(subjectAce1->resources, rsrc)
    {
        printf("%s\n", rsrc->href);
    }
    printf("\n\n");

    // Create Entity Handler DELETE request
    ehReq.method = OC_REST_DELETE;
    char query[] = "subjectuuid=32323232-3232-3232-3232-323232323232;resources=/a/led";
    ehReq.query = (char *)OICMalloc(strlen(query)+1);
    ASSERT_TRUE(NULL != ehReq.query);
    OICStrcpy(ehReq.query, strlen(query)+1, query);

    ACLEntityHandler(OC_REQUEST_FLAG, &ehReq, NULL);

    // Verify if SRM contains ACL for the subject but only with one resource
    savePtr = NULL;
    const OicSecAce_t* subjectAce2 = GetACLResourceData(&(acl->aces->subjectuuid), &savePtr);
    ASSERT_TRUE(NULL != subjectAce2);
    EXPECT_EQ(6u, GetNumberOfResource(subjectAce2));

    // Perform cleanup
    OCPayloadDestroy((OCPayload *)securityPayload);
    DeleteACLList(acl);
    OICFree(ehReq.query);
    OICFree(payload);
}

//'GET' with query ACL test
TEST_F(SRM_ACL, ACLGetWithQueryTest)
{
    OicSecAcl_t *defaultAcl = NULL;
    EXPECT_EQ(OC_STACK_OK, GetDefaultACL(&defaultAcl));
    ASSERT_TRUE(defaultAcl != NULL);
    EXPECT_EQ(OC_STACK_OK, SetDefaultACL(defaultAcl));

    //Populate ACL
    OicSecAcl_t *acl = (OicSecAcl_t *) OICCalloc(1, sizeof(OicSecAcl_t));
    ASSERT_TRUE(NULL != acl);
    EXPECT_EQ(OC_STACK_OK, populateAcl(acl, 1));

    //GET CBOR POST payload
    size_t size = 0;
    uint8_t *payload = NULL;
    EXPECT_EQ(OC_STACK_OK, AclToCBORPayload(acl, OIC_SEC_ACL_V2, &payload, &size));
    ASSERT_TRUE(NULL != payload);

    // Security Payload
    OCSecurityPayload *securityPayload = OCSecurityPayloadCreate(payload, size);
    ASSERT_TRUE(NULL != securityPayload);

    //Create Entity Handler POST request payload
    OCEntityHandlerRequest ehReq = OCEntityHandlerRequest();
    ehReq.method = OC_REST_POST;
    ehReq.payload = (OCPayload *)securityPayload;
    ACLEntityHandler(OC_REQUEST_FLAG, &ehReq, NULL);

    //Create Entity Handler GET request wit query
    ehReq.method = OC_REST_GET;
    char query[] = "subjectuuid=32323232-3232-3232-3232-323232323232;resources=/a/led";
    ehReq.query = (char*)OICMalloc(strlen(query)+1);
    ASSERT_TRUE(NULL != ehReq.query);
    OICStrcpy(ehReq.query, strlen(query)+1, query);

    ACLEntityHandler(OC_REQUEST_FLAG, &ehReq, NULL);

    // Perform cleanup
    OCPayloadDestroy((OCPayload *)securityPayload);
    DeleteACLList(acl);
    OICFree(ehReq.query);
    OICFree(payload);
}

#if defined(TCP_ADAPTER) && defined(WITH_CLOUD)
TEST_F(SRM_ACL, CBORPayloadToCloudAcl)
{
    uint8_t *payload = NULL;
    size_t size   = 0;

    OicSecAcl_t *acl = (OicSecAcl_t*)OICCalloc(1, sizeof(OicSecAcl_t));
    ASSERT_NE(nullptr, acl);
    EXPECT_EQ(OC_STACK_OK, populateAcl(acl, 3));

    //acl->aces->validities = (OicSecValidity_t*)OICCalloc(1, sizeof(OicSecValidity_t));
    //acl->aces->validities->period = OICStrdup("10/10/20");

    EXPECT_EQ(OC_STACK_OK, AclToCBORPayload(acl, OIC_SEC_ACL_V2, &payload, &size));
    ASSERT_NE(0u, size);

    acl = CBORPayloadToCloudAcl(payload, size);
//    EXPECT_NE(nullptr, acl);
}
#endif

TEST_F(SRM_ACL, GetSecDefaultACE)
{
    OicSecAce_t* ace1 = GetSecDefaultACE();
    EXPECT_NE(nullptr, ace1);

    OicSecAce_t *ace = (OicSecAce_t*)OICCalloc(1, sizeof(OicSecAce_t));
    ace->aceid = (uint16_t)rand();
    ace->subjectType = OicSecAceUuidSubject;
    memcpy(&ace->subjectuuid, &WILDCARD_SUBJECT_ID, sizeof(OicUuid_t));
    ace->permission = PERMISSION_READ | PERMISSION_WRITE;
    LL_APPEND(gAcl->aces, ace);

    ace = (OicSecAce_t*)OICCalloc(1, sizeof(OicSecAce_t));
    ace->aceid = (uint16_t)rand();
    ace->permission = 2;
    memcpy(ace->subjectuuid.id, "2334453466662220", sizeof(ace->subjectuuid.id));
    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/sec/doxm", "oic.r.doxm" ,"oic.if.baseline"));
    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/sec/pstat", "oic.r.pstat" ,"oic.if.baseline"));
    LL_APPEND(gAcl->aces, ace);

    EXPECT_EQ(OC_STACK_OK, UpdateDefaultSecProvACE());

    OicSecAcl_t *acl = (OicSecAcl_t*)OICCalloc(1, sizeof(OicSecAcl_t));
    ASSERT_NE(nullptr, acl);
    EXPECT_EQ(OC_STACK_OK, populateAcl(acl, 1));

    ConvertStrToUuid("99999999-8888-7777-6666-555555555555", &acl->aces->subjectuuid);

    EXPECT_EQ(OC_STACK_ERROR, InstallACL(acl));

    ConvertStrToUuid("*", &acl->aces->subjectuuid);
    EXPECT_EQ(OC_STACK_ERROR, AppendACLObject(acl));
}

TEST_F(SRM_ACL, GetACLResourceDataByRoles)
{

    OicSecAce_t* ace = NULL;
    OicSecRole_t *roles = (OicSecRole_t *)OICCalloc(1, sizeof(OicSecRole_t));
    roles->id[0] = 0;
    roles->authority[0] = 1;

    size_t roleCount = 1;
    OicSecAce_t *savePtr = NULL;

    ace = (OicSecAce_t*)GetACLResourceDataByRoles(roles, roleCount, &savePtr);
    ASSERT_TRUE(NULL == ace);// && ace->aceid);
    //ASSERT_NE(nullptr, ace);
}

TEST_F(SRM_ACL, ACL2EntityHandler)
{
    OCEntityHandlerFlag flag = OC_REQUEST_FLAG;
    OCEntityHandlerRequest * ehRequest = NULL;
    void* callbackParameter = NULL;

    ASSERT_EQ(OC_EH_ERROR, ACL2EntityHandler(flag, ehRequest, callbackParameter));

    //Populate ACL
    OicSecAcl_t *acl = (OicSecAcl_t *) OICCalloc(1, sizeof(OicSecAcl_t));
    ASSERT_TRUE(NULL != acl);
    EXPECT_EQ(OC_STACK_OK, populateAcl(acl, 1));

    //GET CBOR POST payload
    size_t size = 0;
    uint8_t *payload = NULL;
    EXPECT_EQ(OC_STACK_OK, AclToCBORPayload(acl, OIC_SEC_ACL_V2, &payload, &size));
    ASSERT_TRUE(NULL != payload);

    // Security Payload
    OCSecurityPayload *securityPayload = OCSecurityPayloadCreate(payload, size);
    ASSERT_TRUE(NULL != securityPayload);

    ehRequest = new OCEntityHandlerRequest();
    ehRequest->method = OC_REST_POST;
    ehRequest->payload = (OCPayload *)securityPayload;

    ASSERT_EQ(OC_EH_ERROR, ACL2EntityHandler(flag, ehRequest, callbackParameter));

    delete ehRequest;
    OCPayloadDestroy((OCPayload *)securityPayload);
    DeleteACLList(acl);
    OICFree(payload);
}

TEST_F(SRM_ACL, RemoveAllAce)
{
    const char *resource = NULL;
    ASSERT_EQ(OC_STACK_ERROR, RemoveAllAce(resource));
    resource = gAcl && gAcl->aces && gAcl->aces->resources ? gAcl->aces->resources->href : "doxm";
    ASSERT_EQ(OC_STACK_ERROR, RemoveAllAce(resource));
}

TEST_F(SRM_ACL, IsSameValidities)
{
    OicSecValidity_t* validities1 = (OicSecValidity_t*)OICCalloc(1, sizeof(OicSecValidity_t));
    OicSecValidity_t* validities2 = (OicSecValidity_t*)OICCalloc(1, sizeof(OicSecValidity_t));
    validities1->period = OICStrdup("20/10/99");
    validities2->period = OICStrdup("20/10/99");
    ASSERT_TRUE(IsSameValidities(validities1, validities2));
    OICFree(validities1);
    OICFree(validities2);
}

TEST_F(SRM_ACL, RemoveAceByAceIds)
{
    AceIdList_t *aceIdList = (AceIdList_t *) OICCalloc(1, sizeof(AceIdList_t));
    aceIdList->aceid = 3;
    EXPECT_EQ(OC_STACK_NO_RESOURCE, RemoveAceByAceIds(aceIdList));
}

TEST_F(SRM_ACL, GetAclRownerId)
{
    EXPECT_FALSE(IsAclRowneruuidTheNilUuid());
    OicUuid_t uuid;
    ConvertStrToUuid("33333333-3333-3333-3333-222222222222", &uuid);
    ASSERT_EQ(OC_STACK_OK, GetAclRownerId(&uuid));
}

TEST_F(SRM_ACL,FreeACE)
{
    FreeACE(NULL);

    OicSecAce_t* ace = GetSecDefaultACE();
    ace->validities = (OicSecValidity_t*)OICCalloc(1, sizeof(OicSecValidity_t));
    ace->validities->period = OICStrdup("20/10/99");
    ace->validities->recurrences = (char**)OICCalloc(1, sizeof(OicSecValidity_t));
    ace->validities->recurrences[0] = (char*)OICStrdup("rec");
    ace->validities->recurrenceLen = 1;

    FreeACE(ace);
}

TEST_F(SRM_ACL, DeleteAceIdList)
{
    AceIdList_t *list = (AceIdList_t*) OICCalloc(1, sizeof(AceIdList_t));
    DeleteAceIdList(&list);
}

TEST_F(SRM_ACL, DuplicateACE)
{
    OicSecAce_t* ace = GetSecDefaultACE();
    ace->validities = (OicSecValidity_t*)OICCalloc(1, sizeof(OicSecValidity_t));
    ace->validities->period = OICStrdup("20/10/99");
    ace->validities->recurrences = (char**)OICCalloc(1, sizeof(OicSecValidity_t));
    ace->validities->recurrences[0] = (char*)OICStrdup("rec");
    ace->validities->recurrenceLen = 1;

    DuplicateACE(ace,true);
}

TEST_F(SRM_ACL, AclToCBORPayload)
{
    OicSecAcl_t *acl = (OicSecAcl_t *) OICCalloc(1, sizeof(OicSecAcl_t));
    ASSERT_NE(nullptr, acl);
    EXPECT_EQ(OC_STACK_OK, populateAcl(acl, 3));

    size_t size = 0;
    uint8_t *payload = NULL;
    ASSERT_EQ(OC_STACK_ERROR, AclToCBORPayload(acl, OIC_SEC_ACL_V2, &payload, &size));
//    ASSERT_NE(nullptr, payload);
}

