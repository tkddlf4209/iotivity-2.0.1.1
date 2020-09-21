/******************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
#ifdef TC_DISABLE
#include "CSCsdkCloudHelper.h"
#include "CSCsdkUtilityHelper.h"

class CSCsdkGroupTest_stc: public ::testing::Test
{
protected:

    CSCsdkCloudHelper m_CloudAclHelper;
    string m_hostAddress = COAP_HOST_ADDRESS;
    string m_authProvider = "github";
    string m_authCode = "";
    string m_uidControlee = "";
    string m_authTokenControlee = "";
    string m_groupId = "";
    string m_aclId = "";
    OCAccountManager::Ptr m_accountMgrControlee = nullptr;
    std::vector< std::string > m_deviceIdAddded;
    std::vector< std::string > m_deviceIdRemoving;
    uint16_t m_credId = 0;
    ByteArray_t m_trustCertChainArray =
    { 0, 0 };

    OCDevAddr m_endPoint;

    stringArray_t *m_memberArray = NULL;
    stringArray_t *m_deviceArray = NULL;
    stringArray_t *m_groupArray = NULL;

    virtual void SetUp()
    {
        CommonUtil::copyFile(CLOUD_ACL_CONTROLLER_DAT_BACKUP, CLOUD_ACL_CONTROLLER_DAT);
        CommonUtil::copyFile(ROOT_CERT_FILE_BACKUP, ROOT_CERT_FILE);
        CommonTestUtil::runCommonTCSetUpPart();

        if (!m_CloudAclHelper.initCloudACLClient())
        {
            SET_FAILURE(m_CloudAclHelper.getFailureMessage());
            return;
        }

        m_hostAddress = CloudCommonUtil::getDefaultHostAddess();
        m_endPoint = CloudCommonUtil::getDefaultEndPoint();

#ifdef __TLS_ON__
        setCoapPrefix(true);
        CASelectCipherSuite(MBEDTLS_TLS_RSA_WITH_AES_128_GCM_SHA256, CA_ADAPTER_TCP);
#endif

        CSCsdkCloudHelper::readFile(ROOT_CERT_FILE, (OCByteString *) &m_trustCertChainArray);
        m_accountMgrControlee = OCPlatform::constructAccountManagerObject(m_hostAddress,
                CT_ADAPTER_TCP);

        if (!m_CloudAclHelper.saveTrustCertChain(m_trustCertChainArray.data, m_trustCertChainArray.len,
                        OIC_ENCODING_PEM, &m_credId, OC_STACK_OK))
        {
            SET_FAILURE(m_CloudAclHelper.getFailureMessage());
            return;
        }

        if (!CloudCommonUtil::signIn(m_accountMgrControlee))
        {
            SET_FAILURE(ERROR_SIGN_IN);
            return;
        }

        m_memberArray = (stringArray_t*) OICCalloc(memberNumber, sizeof(stringArray_t*));
        m_memberArray->array = memberIDs;
        m_memberArray->length = memberNumber;

        m_deviceArray = (stringArray_t*) OICCalloc(deviceNumber, sizeof(stringArray_t*));
        m_deviceArray->array = deviceIDs;
        m_deviceArray->length = deviceNumber;

        m_groupArray = (stringArray_t*) OICCalloc(1, sizeof(stringArray_t*));
        m_groupArray->array = groupIds;
        m_groupArray->length = groupNumber;
    }

    virtual void TearDown()
    {
        CommonTestUtil::runCommonTCTearDownPart();
    }
};

/**
 * @since           2017-02-23
 * @see             OCStackResult OCRegisterPersistentStorageHandler(OCPersistentStorage* persistentStorageHandler)
 * @see             OCStackResult OCInit(const char *ipAddr, uint16_t port, OCMode mode)
 * @see             void setCoapPrefix(bool secured)
 * @see             CAResult_t CASelectCipherSuite(const uint16_t cipher, CATransportAdapter_t adapter)
 * @see             OCStackResult OCSaveTrustCertChain(uint8_t *trustCertChain, size_t chainSize, OicEncodingType_t encodingType, uint16_t *credId)
 * @see             OCStackResult signIn(const std::string& userUuid, const std::string& accessToken, PostCallback cloudConnectHandler)
 * @objective       Test OCCloudAclCreateGroup positively with regular data
 * @target          OCStackResult OCCloudAclCreateGroup(void* ctx, const char *groupType,const char *groupMasterId, const OCDevAddr *endPoint,OCCloudResponseCB callback)
 * @test_data       regular data for target API
 * @pre_condition   Run iotivity_cs_server
 * @procedure       1. call OCRegisterPersistentStorageHandler
 *                  2. call OCInit
 *                  3. call setCoapPrefix wtih true param
 *                  4. call CASelectCipherSuite with MBEDTLS_TLS_RSA_WITH_AES_256_CBC_SHA and CA_ADAPTER_TCP
 *                  5. call signIn
 *                  6. call OCCloudAclCreateGroup
 * @post_condition  none
 * @expected        OCCloudAclCreateGroup will return OC_STACK_OK
 */
#if defined(__LINUX__) || defined(__TIZEN__)
TEST_F(CSCsdkGroupTest_stc, OCCloudAclCreateGroup_SRC_P)
{
    if (!m_CloudAclHelper.cloudAclCreateGroup((void*)CTX_CREATE_GROUP, GROUP_TYPE_PUBLIC, GROUP_MASTER_ID,
                    &m_endPoint, CSCsdkCloudHelper::createGroupResponseCB, m_groupId, OC_STACK_OK))
    {
        SET_FAILURE(m_CloudAclHelper.getFailureMessage());
    }
}
#endif

/**
 * @since           2017-02-23
 * @see             OCStackResult OCRegisterPersistentStorageHandler(OCPersistentStorage* persistentStorageHandler)
 * @see             OCStackResult OCInit(const char *ipAddr, uint16_t port, OCMode mode)
 * @see             void setCoapPrefix(bool secured)
 * @see             CAResult_t CASelectCipherSuite(const uint16_t cipher, CATransportAdapter_t adapter)
 * @see             OCStackResult OCSaveTrustCertChain(uint8_t *trustCertChain, size_t chainSize, OicEncodingType_t encodingType, uint16_t *credId)
 * @see             OCStackResult signIn(const std::string& userUuid, const std::string& accessToken, PostCallback cloudConnectHandler)
 * @see             OCStackResult OCCloudAclCreateGroup(void* ctx, const char *groupType,const char *groupMasterId, const OCDevAddr *endPoint,OCCloudResponseCB callback)
 * @objective       Test OCCloudAclFindMyGroup positively with regular data
 * @target          OCStackResult OCCloudAclFindMyGroup(void* ctx, const char *memberId, const OCDevAddr *endPoint, OCCloudResponseCB callback)
 * @test_data       regular data for target API
 * @pre_condition   Run iotivity_cs_server
 * @procedure       1. call OCRegisterPersistentStorageHandler
 *                  2. call OCInit
 *                  3. call setCoapPrefix wtih true param
 *                  4. call CASelectCipherSuite with MBEDTLS_TLS_RSA_WITH_AES_256_CBC_SHA and CA_ADAPTER_TCP
 *                  5. call signIn
 *                  6. call OCCloudAclCreateGroup
 *                  7. call OCCloudAclFindMyGroup
 * @post_condition  none
 * @expected        OCCloudAclFindMyGroup will return OC_STACK_OK
 */
#if defined(__LINUX__) || defined(__TIZEN__)
TEST_F(CSCsdkGroupTest_stc, OCCloudAclFindMyGroup_SRC_P)
{
    if (!m_CloudAclHelper.cloudAclCreateGroup((void*)CTX_CREATE_GROUP, GROUP_TYPE_PUBLIC, GROUP_MASTER_ID,
                    &m_endPoint, CSCsdkCloudHelper::createGroupResponseCB, m_groupId, OC_STACK_OK))
    {
        SET_FAILURE(m_CloudAclHelper.getFailureMessage());
        return;
    }

    if (!m_CloudAclHelper.cloudAclFindMyGroup((void*)CTX_FIND_GROUP, GROUP_MASTER_ID,
                    &m_endPoint, CSCsdkCloudHelper::cloudResponseCB, OC_STACK_OK))
    {
        SET_FAILURE(m_CloudAclHelper.getFailureMessage());
    }
}
#endif

/**
 * @since           2017-02-23
 * @see             OCStackResult OCRegisterPersistentStorageHandler(OCPersistentStorage* persistentStorageHandler)
 * @see             OCStackResult OCInit(const char *ipAddr, uint16_t port, OCMode mode)
 * @see             void setCoapPrefix(bool secured)
 * @see             CAResult_t CASelectCipherSuite(const uint16_t cipher, CATransportAdapter_t adapter)
 * @see             OCStackResult OCSaveTrustCertChain(uint8_t *trustCertChain, size_t chainSize, OicEncodingType_t encodingType, uint16_t *credId)
 * @see             OCStackResult signIn(const std::string& userUuid, const std::string& accessToken, PostCallback cloudConnectHandler)
 * @see             OCStackResult OCCloudAclCreateGroup(void* ctx, const char *groupType,const char *groupMasterId, const OCDevAddr *endPoint,OCCloudResponseCB callback)
 * @objective       Test OCCloudAclFindMyGroup positively with regular data
 * @target          OCStackResult OCCloudAclDeleteGroup(void* ctx, const char *groupId, const char *groupMasterId, const OCDevAddr *endPoint, OCCloudResponseCB callback)
 * @test_data       regular data for target API
 * @pre_condition   Run iotivity_cs_server
 * @procedure       1. call OCRegisterPersistentStorageHandler
 *                  2. call OCInit
 *                  3. call setCoapPrefix wtih true param
 *                  4. call CASelectCipherSuite with MBEDTLS_TLS_RSA_WITH_AES_256_CBC_SHA and CA_ADAPTER_TCP
 *                  5. call signIn
 *                  6. call OCCloudAclCreateGroup
 *                  7. call OCCloudAclDeleteGroup
 * @post_condition  none
 * @expected        OCCloudAclDeleteGroup will return OC_STACK_OK
 */
#if defined(__LINUX__) || defined(__TIZEN__)
TEST_F(CSCsdkGroupTest_stc, OCCloudAclDeleteGroup_SRC_P)
{
    if (!m_CloudAclHelper.cloudAclCreateGroup((void*)CTX_CREATE_GROUP, GROUP_TYPE_PUBLIC, GROUP_MASTER_ID,
                    &m_endPoint, CSCsdkCloudHelper::createGroupResponseCB, m_groupId, OC_STACK_OK))
    {
        SET_FAILURE(m_CloudAclHelper.getFailureMessage());
        return;
    }

    if (!m_CloudAclHelper.cloudAclDeleteGroup((void*)CTX_DELETE_GROUP, m_groupId.c_str(), GROUP_MASTER_ID,
                    &m_endPoint, CSCsdkCloudHelper::cloudResponseCB, OC_STACK_OK))
    {
        SET_FAILURE(m_CloudAclHelper.getFailureMessage());
    }
}
#endif

/**
 * @since           2017-02-23
 * @see             OCStackResult OCRegisterPersistentStorageHandler(OCPersistentStorage* persistentStorageHandler)
 * @see             OCStackResult OCInit(const char *ipAddr, uint16_t port, OCMode mode)
 * @see             void setCoapPrefix(bool secured)
 * @see             CAResult_t CASelectCipherSuite(const uint16_t cipher, CATransportAdapter_t adapter)
 * @see             OCStackResult OCSaveTrustCertChain(uint8_t *trustCertChain, size_t chainSize, OicEncodingType_t encodingType, uint16_t *credId)
 * @see             OCStackResult signIn(const std::string& userUuid, const std::string& accessToken, PostCallback cloudConnectHandler)
 * @see             OCStackResult OCCloudAclCreateGroup(void* ctx, const char *groupType,const char *groupMasterId, const OCDevAddr *endPoint,OCCloudResponseCB callback)
 * @objective       Test OCCloudAclGroupGetInfo positively with regular data
 * @target          OCStackResult OCCloudAclGroupGetInfo(void* ctx, const char *groupId, const char *memberId, const OCDevAddr *endPoint, OCCloudResponseCB callback)
 * @test_data       regular data for target API
 * @pre_condition   Run iotivity_cs_server
 * @procedure       1. call OCRegisterPersistentStorageHandler
 *                  2. call OCInit
 *                  3. call setCoapPrefix wtih true param
 *                  4. call CASelectCipherSuite with MBEDTLS_TLS_RSA_WITH_AES_256_CBC_SHA and CA_ADAPTER_TCP
 *                  5. call signIn
 *                  6. call OCCloudAclCreateGroup
 *                  7. call OCCloudAclGroupGetInfo
 * @post_condition  none
 * @expected        OCCloudAclGroupGetInfo will return OC_STACK_OK
 */
#if defined(__LINUX__) || defined(__TIZEN__)
TEST_F(CSCsdkGroupTest_stc, OCCloudAclGroupGetInfo_SRC_P)
{
    if (!m_CloudAclHelper.cloudAclCreateGroup((void*)CTX_CREATE_GROUP, GROUP_TYPE_PUBLIC, GROUP_MASTER_ID,
                    &m_endPoint, CSCsdkCloudHelper::createGroupResponseCB, m_groupId, OC_STACK_OK))
    {
        SET_FAILURE(m_CloudAclHelper.getFailureMessage());
        return;
    }

    if (!m_CloudAclHelper.cloudAclGroupGetInfo((void*)CTX_GET_GROUP_INFO, m_groupId.c_str(), GROUP_MASTER_ID,
                    &m_endPoint, CSCsdkCloudHelper::cloudResponseCB, OC_STACK_OK))
    {
        SET_FAILURE(m_CloudAclHelper.getFailureMessage());
    }
}
#endif

/**
 * @since           2017-02-23
 * @see             OCStackResult OCRegisterPersistentStorageHandler(OCPersistentStorage* persistentStorageHandler)
 * @see             OCStackResult OCInit(const char *ipAddr, uint16_t port, OCMode mode)
 * @see             void setCoapPrefix(bool secured)
 * @see             CAResult_t CASelectCipherSuite(const uint16_t cipher, CATransportAdapter_t adapter)
 * @see             OCStackResult OCSaveTrustCertChain(uint8_t *trustCertChain, size_t chainSize, OicEncodingType_t encodingType, uint16_t *credId)
 * @see             OCStackResult signIn(const std::string& userUuid, const std::string& accessToken, PostCallback cloudConnectHandler)
 * @see             OCStackResult OCCloudAclCreateGroup(void* ctx, const char *groupType,const char *groupMasterId, const OCDevAddr *endPoint,OCCloudResponseCB callback)
 * @see             OCStackResult OCCloudAclGroupGetInfo(void* ctx, const char *groupId, const char *memberId, const OCDevAddr *endPoint, OCCloudResponseCB callback)
 * @objective       Test OCCloudAclInviteUser positively with regular data
 * @target          OCStackResult OCCloudAclInviteUser(void* ctx, const char *userId, const stringArray_t *memberIds, const OCDevAddr *endPoint, OCCloudResponseCB callback)
 * @test_data       regular data for target API
 * @pre_condition   Run iotivity_cs_server
 * @procedure       1. call OCRegisterPersistentStorageHandler
 *                  2. call OCInit
 *                  3. call setCoapPrefix wtih true param
 *                  4. call CASelectCipherSuite with MBEDTLS_TLS_RSA_WITH_AES_256_CBC_SHA and CA_ADAPTER_TCP
 *                  5. call signIn
 *                  6. call OCCloudAclCreateGroup
 *                  7. call OCCloudAclGroupGetInfo
 *                  8. call OCCloudAclInviteUser
 * @post_condition  none
 * @expected        OCCloudAclInviteUser will return OC_STACK_OK
 */
#if defined(__LINUX__) || defined(__TIZEN__)
TEST_F(CSCsdkGroupTest_stc, OCCloudAclInviteUser_SRC_P)
{
    if (!m_CloudAclHelper.cloudAclCreateGroup((void*)CTX_CREATE_GROUP, GROUP_TYPE_PUBLIC, GROUP_MASTER_ID,
                    &m_endPoint, CSCsdkCloudHelper::createGroupResponseCB, m_groupId, OC_STACK_OK))
    {
        SET_FAILURE(m_CloudAclHelper.getFailureMessage());
        return;
    }

    if (!m_CloudAclHelper.cloudAclGroupGetInfo((void*)CTX_GET_GROUP_INFO, m_groupId.c_str(), GROUP_MASTER_ID,
                    &m_endPoint, CSCsdkCloudHelper::cloudResponseCB, OC_STACK_OK))
    {
        SET_FAILURE(m_CloudAclHelper.getFailureMessage());
        return;
    }

    if (!m_CloudAclHelper.cloudAclInviteUser((void*)CTX_INVITE_USER, NULL, m_groupArray, m_memberArray,
                    &m_endPoint, CSCsdkCloudHelper::cloudResponseCB, OC_STACK_OK))
    {
        SET_FAILURE(m_CloudAclHelper.getFailureMessage());
    }
}
#endif

/**
 * @since           2017-02-23
 * @see             OCStackResult OCRegisterPersistentStorageHandler(OCPersistentStorage* persistentStorageHandler)
 * @see             OCStackResult OCInit(const char *ipAddr, uint16_t port, OCMode mode)
 * @see             void setCoapPrefix(bool secured)
 * @see             CAResult_t CASelectCipherSuite(const uint16_t cipher, CATransportAdapter_t adapter)
 * @see             OCStackResult OCSaveTrustCertChain(uint8_t *trustCertChain, size_t chainSize, OicEncodingType_t encodingType, uint16_t *credId)
 * @see             OCStackResult signIn(const std::string& userUuid, const std::string& accessToken, PostCallback cloudConnectHandler)
 * @see             OCStackResult OCCloudAclCreateGroup(void* ctx, const char *groupType,const char *groupMasterId, const OCDevAddr *endPoint,OCCloudResponseCB callback)
 * @see             OCStackResult OCCloudAclGroupGetInfo(void* ctx, const char *groupId, const char *memberId, const OCDevAddr *endPoint, OCCloudResponseCB callback)
 * @see             OCStackResult OCCloudAclInviteUser(void* ctx, const char *userId, const stringArray_t *memberIds, const OCDevAddr *endPoint, OCCloudResponseCB callback)
 * @objective       Test OCCloudAclCancelInvitation positively with regular data
 * @target          OCStackResult OCCloudAclCancelInvitation(void* ctx, const char *userId, const char *groupId, const char *memberId, const OCDevAddr *endPoint, OCCloudResponseCB callback)
 * @test_data       regular data for target API
 * @pre_condition   Run iotivity_cs_server
 * @procedure       1. call OCRegisterPersistentStorageHandler
 *                  2. call OCInit
 *                  3. call setCoapPrefix wtih true param
 *                  4. call CASelectCipherSuite with MBEDTLS_TLS_RSA_WITH_AES_256_CBC_SHA and CA_ADAPTER_TCP
 *                  5. call signIn
 *                  6. call OCCloudAclCreateGroup
 *                  7. call OCCloudAclGroupGetInfo
 *                  8. call OCCloudAclInviteUser
 *                  9. call OCCloudAclCancelInvitation
 * @post_condition  none
 * @expected        OCCloudAclCancelInvitation will return OC_STACK_OK
 */
#if defined(__LINUX__) || defined(__TIZEN__)
TEST_F(CSCsdkGroupTest_stc, OCCloudCancelInvitation_SRC_P)
{
    if (!m_CloudAclHelper.cloudAclCreateGroup((void*)CTX_CREATE_GROUP, GROUP_TYPE_PUBLIC, GROUP_MASTER_ID,
                    &m_endPoint, CSCsdkCloudHelper::createGroupResponseCB, m_groupId, OC_STACK_OK))
    {
        SET_FAILURE(m_CloudAclHelper.getFailureMessage());
        return;
    }

    if (!m_CloudAclHelper.cloudAclGroupGetInfo((void*)CTX_GET_GROUP_INFO, m_groupId.c_str(), GROUP_MASTER_ID,
                    &m_endPoint, CSCsdkCloudHelper::cloudResponseCB, OC_STACK_OK))
    {
        SET_FAILURE(m_CloudAclHelper.getFailureMessage());
        return;
    }

    if (!m_CloudAclHelper.cloudAclInviteUser((void*)CTX_INVITE_USER, NULL, m_groupArray, m_memberArray,
                    &m_endPoint, CSCsdkCloudHelper::cloudResponseCB, OC_STACK_OK))
    {
        SET_FAILURE(m_CloudAclHelper.getFailureMessage());
        return;
    }

    if (!m_CloudAclHelper.cloudAclCancelInvitation((void*)CTX_INVITE_USER, NULL, m_groupId.c_str(), GROUP_MEMBER_ID_01,
                    &m_endPoint, CSCsdkCloudHelper::cloudResponseCB, OC_STACK_OK))
    {
        SET_FAILURE(m_CloudAclHelper.getFailureMessage());
    }
}
#endif

/**
 * @since           2017-02-23
 * @see             OCStackResult OCRegisterPersistentStorageHandler(OCPersistentStorage* persistentStorageHandler)
 * @see             OCStackResult OCInit(const char *ipAddr, uint16_t port, OCMode mode)
 * @see             void setCoapPrefix(bool secured)
 * @see             CAResult_t CASelectCipherSuite(const uint16_t cipher, CATransportAdapter_t adapter)
 * @see             OCStackResult OCSaveTrustCertChain(uint8_t *trustCertChain, size_t chainSize, OicEncodingType_t encodingType, uint16_t *credId)
 * @see             OCStackResult signIn(const std::string& userUuid, const std::string& accessToken, PostCallback cloudConnectHandler)
 * @see             OCStackResult OCCloudAclCreateGroup(void* ctx, const char *groupType,const char *groupMasterId, const OCDevAddr *endPoint,OCCloudResponseCB callback)
 * @see             OCStackResult OCCloudAclGroupGetInfo(void* ctx, const char *groupId, const char *memberId, const OCDevAddr *endPoint, OCCloudResponseCB callback)
 * @see             OCStackResult OCCloudAclCancelInvitation(void* ctx, const char *userId, const char *groupId, const char *memberId, const OCDevAddr *endPoint, OCCloudResponseCB callback)
 * @see             OCStackResult OCCloudAclInviteUser(void* ctx, const char *userId, const stringArray_t *memberIds, const OCDevAddr *endPoint, OCCloudResponseCB callback)
 * @objective       Test OCCloudAclShareDeviceIntoGroup positively with regular data
 * @target          OCStackResult OCCloudAclShareDeviceIntoGroup(void* ctx, const char *groupId, const stringArray_t *memberIds, const stringArray_t *deviceIds, const OCDevAddr *endPoint, OCCloudResponseCB callback)
 * @test_data       regular data for target API
 * @pre_condition   Run iotivity_cs_server
 * @procedure       1. call OCRegisterPersistentStorageHandler
 *                  2. call OCInit
 *                  3. call setCoapPrefix wtih true param
 *                  4. call CASelectCipherSuite with MBEDTLS_TLS_RSA_WITH_AES_256_CBC_SHA and CA_ADAPTER_TCP
 *                  5. call signIn
 *                  6. call OCCloudAclCreateGroup
 *                  7. call OCCloudAclGroupGetInfo
 *                  8. call OCCloudAclInviteUser
 *                  9. call OCCloudAclCancelInvitation
 *                  10. call OCCloudAclShareDeviceIntoGroup
 * @post_condition  none
 * @expected        OCCloudAclShareDeviceIntoGroup will return OC_STACK_OK
 */
#if defined(__LINUX__) || defined(__TIZEN__)
TEST_F(CSCsdkGroupTest_stc, OCCloudAclShareDeviceIntoGroup_SRC_P)
{
    if (!m_CloudAclHelper.cloudAclCreateGroup((void*)CTX_CREATE_GROUP, GROUP_TYPE_PUBLIC, GROUP_MASTER_ID,
                    &m_endPoint, CSCsdkCloudHelper::createGroupResponseCB, m_groupId, OC_STACK_OK))
    {
        SET_FAILURE(m_CloudAclHelper.getFailureMessage());
        return;
    }

    if (!m_CloudAclHelper.cloudAclGroupGetInfo((void*)CTX_GET_GROUP_INFO, m_groupId.c_str(), GROUP_MASTER_ID,
                    &m_endPoint, CSCsdkCloudHelper::cloudResponseCB, OC_STACK_OK))
    {
        SET_FAILURE(m_CloudAclHelper.getFailureMessage());
        return;
    }

    if (!m_CloudAclHelper.cloudAclInviteUser((void*)CTX_INVITE_USER, NULL, m_groupArray, m_memberArray,
                    &m_endPoint, CSCsdkCloudHelper::cloudResponseCB, OC_STACK_OK))
    {
        SET_FAILURE(m_CloudAclHelper.getFailureMessage());
        return;
    }

    if (!m_CloudAclHelper.cloudAclCancelInvitation((void*)CTX_INVITE_USER, NULL, m_groupId.c_str(), GROUP_MEMBER_ID_01,
                    &m_endPoint, CSCsdkCloudHelper::cloudResponseCB, OC_STACK_OK))
    {
        SET_FAILURE(m_CloudAclHelper.getFailureMessage());
        return;
    }

    if (!m_CloudAclHelper.cloudAclShareDeviceIntoGroup((void*)CTX_SHARE_DEVICE, m_groupId.c_str(), m_memberArray, m_deviceArray,
                    &m_endPoint, CSCsdkCloudHelper::cloudResponseCB, OC_STACK_OK))
    {
        SET_FAILURE(m_CloudAclHelper.getFailureMessage());
    }
}
#endif

/**
 * @since           2017-02-23
 * @see             OCStackResult OCRegisterPersistentStorageHandler(OCPersistentStorage* persistentStorageHandler)
 * @see             OCStackResult OCInit(const char *ipAddr, uint16_t port, OCMode mode)
 * @see             void setCoapPrefix(bool secured)
 * @see             CAResult_t CASelectCipherSuite(const uint16_t cipher, CATransportAdapter_t adapter)
 * @see             OCStackResult OCSaveTrustCertChain(uint8_t *trustCertChain, size_t chainSize, OicEncodingType_t encodingType, uint16_t *credId)
 * @see             OCStackResult signIn(const std::string& userUuid, const std::string& accessToken, PostCallback cloudConnectHandler)
 * @see             OCStackResult OCCloudAclCreateGroup(void* ctx, const char *groupType,const char *groupMasterId, const OCDevAddr *endPoint,OCCloudResponseCB callback)
 * @see             OCStackResult OCCloudAclGroupGetInfo(void* ctx, const char *groupId, const char *memberId, const OCDevAddr *endPoint, OCCloudResponseCB callback)
 * @see             OCStackResult OCCloudAclCancelInvitation(void* ctx, const char *userId, const char *groupId, const char *memberId, const OCDevAddr *endPoint, OCCloudResponseCB callback)
 * @see             OCStackResult OCCloudAclInviteUser(void* ctx, const char *userId, const stringArray_t *memberIds, const OCDevAddr *endPoint, OCCloudResponseCB callback)
 * @see             OCStackResult OCCloudAclShareDeviceIntoGroup(void* ctx, const char *groupId, const stringArray_t *memberIds, const stringArray_t *deviceIds, const OCDevAddr *endPoint, OCCloudResponseCB callback)
 * @objective       Test OCCloudAclDeleteDeviceFromGroup positively with regular data
 * @target          OCStackResult OCCloudAclDeleteDeviceFromGroup(void* ctx, const char *groupId, const stringArray_t *memberIds, const stringArray_t *deviceIds, const OCDevAddr *endPoint, OCCloudResponseCB callback)
 * @test_data       regular data for target API
 * @pre_condition   Run iotivity_cs_server
 * @procedure       1. call OCRegisterPersistentStorageHandler
 *                  2. call OCInit
 *                  3. call setCoapPrefix wtih true param
 *                  4. call CASelectCipherSuite with MBEDTLS_TLS_RSA_WITH_AES_256_CBC_SHA and CA_ADAPTER_TCP
 *                  5. call signIn
 *                  6. call OCCloudAclCreateGroup
 *                  7. call OCCloudAclGroupGetInfo
 *                  8. call OCCloudAclInviteUser
 *                  9. call OCCloudAclCancelInvitation
 *                  10. call OCCloudAclShareDeviceIntoGroup
 *                  11. call OCCloudAclDeleteDeviceFromGroup
 * @post_condition  none
 * @expected        OCCloudAclDeleteDeviceFromGroup will return OC_STACK_OK
 */
#if defined(__LINUX__) || defined(__TIZEN__)
TEST_F(CSCsdkGroupTest_stc, OCCloudAclDeleteDeviceFromGroup_SRC_P)
{
    if (!m_CloudAclHelper.cloudAclCreateGroup((void*)CTX_CREATE_GROUP, GROUP_TYPE_PUBLIC, GROUP_MASTER_ID,
                    &m_endPoint, CSCsdkCloudHelper::createGroupResponseCB, m_groupId, OC_STACK_OK))
    {
        SET_FAILURE(m_CloudAclHelper.getFailureMessage());
        return;
    }

    if (!m_CloudAclHelper.cloudAclGroupGetInfo((void*)CTX_GET_GROUP_INFO, m_groupId.c_str(), GROUP_MASTER_ID,
                    &m_endPoint, CSCsdkCloudHelper::cloudResponseCB, OC_STACK_OK))
    {
        SET_FAILURE(m_CloudAclHelper.getFailureMessage());
        return;
    }

    if (!m_CloudAclHelper.cloudAclInviteUser((void*)CTX_INVITE_USER, NULL, m_groupArray, m_memberArray,
                    &m_endPoint, CSCsdkCloudHelper::cloudResponseCB, OC_STACK_OK))
    {
        SET_FAILURE(m_CloudAclHelper.getFailureMessage());
        return;
    }

    if (!m_CloudAclHelper.cloudAclCancelInvitation((void*)CTX_INVITE_USER, NULL, m_groupId.c_str(), GROUP_MEMBER_ID_01,
                    &m_endPoint, CSCsdkCloudHelper::cloudResponseCB, OC_STACK_OK))
    {
        SET_FAILURE(m_CloudAclHelper.getFailureMessage());
        return;
    }

    if (!m_CloudAclHelper.cloudAclShareDeviceIntoGroup((void*)CTX_SHARE_DEVICE, m_groupId.c_str(), m_memberArray, m_deviceArray,
                    &m_endPoint, CSCsdkCloudHelper::cloudResponseCB, OC_STACK_OK))
    {
        SET_FAILURE(m_CloudAclHelper.getFailureMessage());
        return;
    }

    if (!m_CloudAclHelper.cloudAclDeleteDeviceFromGroup((void*)CTX_DELETE_SHARED_DEVICE, m_groupId.c_str(), m_memberArray, m_deviceArray,
                    &m_endPoint, CSCsdkCloudHelper::cloudResponseCB, OC_STACK_OK))
    {
        SET_FAILURE(m_CloudAclHelper.getFailureMessage());
    }
}
#endif
#endif
