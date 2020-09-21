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

#include "CAHelper.h"

class CANetworkTest_btc: public ::testing::Test
{
protected:
    CAHelper m_caHelper;

    virtual void SetUp()
    {
        CommonTestUtil::runCommonTCSetUpPart();
    }

    virtual void TearDown()
    {
        CommonTestUtil::runCommonTCTearDownPart();
    }
};

/**
 * @since 2014-12-01
 * @see CAResult_t CAInitialize()
 * @see void CATerminate()
 * @objective Test 'CAStartListeningServer' positively to start the listening service
 * @target CAResult_t CAStartListeningServer()
 * @test_data none
 * @pre_condition 1. Initialize CA using CAInitialize
 *                2. Select IP as network
 * @procedure 1. Call the CAStartListeningServer API
 *            2. Check it's return value
 * @post_condition Terminate CA using CATerminate
 * @expected It will start the listening server and will return CA_STATUS_OK
 */
#if (defined(__LINUX__) || defined(__TIZEN__) || defined(__ANDROID__)) && defined(__ALL_TRANSPORT__)
TEST_F(CANetworkTest_btc, CAStartListeningServer_P)
{
    if (!m_caHelper.initServerNetwork())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
        return;
    }

    CATerminate();
}
#endif

/**
 * @since 2015-02-16
 * @see CAResult_t CAInitialize()
 * @see CAResult_t CASelectNetwork(CATransportAdapter_t interestedNetwork)
 * @see CAResult_t CAStartListeningServer()
 * @see void CATerminate()
 * @objective Test 'CAStopListeningServer' positively to stop the listening service [SRC]
 * @target CAResult_t CAStopListeningServer()
 * @test_data none
 * @pre_condition  1. Initialize CA using CAInitialize
 *                 2. Call CASelectNetwork to select network
 *                 3. Call CAStartListeningServer to start server
 * @procedure 1. Call the CAStopListeningServer API
 *            2. Check it's return value
 * @post_condition Terminate CA using CATerminate
 * @expected It will stop the listening server and will return CA_STATUS_OK
 */
#if (defined(__LINUX__) || defined(__TIZEN__) || defined(__ANDROID__) || defined(__WINDOWS__)) && defined(__ALL_TRANSPORT__)
TEST_F(CANetworkTest_btc, CAStopListeningServer_SRC_P)
{
    if (!m_caHelper.initServerNetwork())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
        return;
    }

    if (!m_caHelper.stopListeningServer())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
    }

    CATerminate();
}
#endif

/**
 * @since 2014-12-01
 * @see CAResult_t CAInitialize()
 * @see void CATerminate()
 * @objective Test 'CAStartDiscoveryServer' positively to start the discovery service
 * @target CAResult_t CAStartDiscoveryServer()
 * @test_data none
 * @pre_condition  1. Initialize CA using CAInitialize
 *                 2. Select IP as network
 * @procedure  1. Call the CAStartDiscoveryServer API
 *             2. Check it's return value
 * @post_condition Terminate CA using CATerminate function
 * @expected It will start the discovery server and will return CA_STATUS_OK
 */
#if (defined(__LINUX__) || defined(__TIZEN__) || defined(__ANDROID__) || defined(__WINDOWS__)) && defined(__ALL_TRANSPORT__)
TEST_F(CANetworkTest_btc, CAStartDiscoveryServer_P)
{
    if (!m_caHelper.initClientNetwork())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
        return;
    }

    CATerminate();
}
#endif

/**
 * @since 2014-11-28
 * @see void CADestroyEndpoint(CAEndpoint_t *object)
 * @objective Test 'CACreateEndpoint' positively to create a remote endpoint
 * @target CAResult_t CACreateEndpoint(CATransportFlags_t flags, CATransportAdapter_t adapter, const char *addr,
 *         uint16_t port, CAEndpoint_t **object)
 * @test_data  none
 * @pre_condition none
 * @procedure call the CACreateEndpoint API and pass the valid flags, adapter, addr, port & endpoint reference as arguments
 * @post_condition call CADestroyEndpoint to destroy the endpoint
 * @expected 1. It will create the endpoint object and return CA_STATUS_OK
 *           2. CAEndpoint must not be NULL.
 */
#if (defined(__LINUX__) || defined(__TIZEN__) || defined(__ANDROID__) || defined(__WINDOWS__)) && defined(__ALL_TRANSPORT__)
TEST_F(CANetworkTest_btc, CACreateEndpoint_P)
{
    if (!m_caHelper.createEndpoint(false, false))
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
        return;
    }

    m_caHelper.destroyEndpoint();
}
#endif

/**
 * @since 2016-02-24
 * @see none
 * @objective Test 'CACreateEndpoint' to check against negative value in flags [flags-ECRC]
 * @target CAResult_t CACreateEndpoint(CATransportFlags_t flags, CATransportAdapter_t adapter, const char *addr,
 *         uint16_t port, CAEndpoint_t **object)
 * @test_data  Invalid flags
 * @pre_condition none
 * @procedure  1. call the CACreateEndpoint API and pass the invalid value as flags arguments
 *             2. Check it's return value
 * @post_condition none
 * @expected It won't return CA_STATUS_OK
 */
#if (defined(__LINUX__) || defined(__TIZEN__) || defined(__ANDROID__) || defined(__WINDOWS__)) && defined(__ALL_TRANSPORT__)
TEST_F(CANetworkTest_btc, CACreateEndpoint_Flag_ECRC_N)
{
    CAEndpoint_t* endpoint = NULL;

    CAResult_t result = CACreateEndpoint(CA_INVALID_FLAG, (CATransportAdapter_t)m_caHelper.m_availableNetwork, ENDPOINT_IP, ENDPOINT_PORT, &endpoint);
    if (result != CA_STATUS_OK)
    {
        SET_FAILURE(m_caHelper.getFailureMessage("CACreateEndpoint", result, CA_STATUS_OK));
    }

    if(endpoint != NULL)
    {
        CADestroyEndpoint(endpoint);
    }
}
#endif

/**
 * @since 2016-02-24
 * @see void CADestroyEndpoint(CAEndpoint_t *object)
 * @objective Test 'CACreateEndpoint' negatively to check against NULL value in address [address-NV]
 * @target CAResult_t CACreateEndpoint(CATransportFlags_t flags, CATransportAdapter_t adapter, const char *addr,
 *         uint16_t port, CAEndpoint_t **object)
 * @test_data  NULL address
 * @pre_condition none
 * @procedure  1. call the CACreateEndpoint API and pass the NULL as address arguments
 *             2. Check it's return value
 * @post_condition call CADestroyEndpoint to destroy the endpoint
 * @expected It will create end point and return CA_STATUS_OK
 */
#if (defined(__LINUX__) || defined(__TIZEN__) || defined(__ANDROID__) || defined(__WINDOWS__)) && defined(__ALL_TRANSPORT__)
TEST_F(CANetworkTest_btc, CACreateEndpoint_Address_NV_N)
{
    CAEndpoint_t* m_endpoint = NULL;

    CAResult_t m_result = CACreateEndpoint(CA_DEFAULT_FLAGS, (CATransportAdapter_t)m_caHelper.m_availableNetwork, NULL, ENDPOINT_PORT, &m_endpoint);
    if (m_result != CA_STATUS_OK)
    {
        SET_FAILURE(m_caHelper.getFailureMessage("CACreateEndpoint", m_result, CA_STATUS_OK));
        return;
    }

    if(m_endpoint != NULL)
    {
        CADestroyEndpoint(m_endpoint);
    }
}
#endif

/**
 * @since 2016-02-24
 * @see none
 * @objective Test 'CACreateEndpoint' negatively to check against NULL value in endpoint object [endpoint-NV]
 * @target CAResult_t CACreateEndpoint(CATransportFlags_t flags, CATransportAdapter_t adapter, const char *addr,
 *         uint16_t port, CAEndpoint_t **object)
 * @test_data  NULL endpoint reference
 * @pre_condition none
 * @procedure  1. call the CACreateEndpoint API and pass the NULL as endpoint arguments
 *             2. Check it's return value
 * @post_condition none
 * @expected It will fail to create end point and return CA_STATUS_INVALID_PARAM
 */
#if (defined(__LINUX__) || defined(__TIZEN__) || defined(__ANDROID__) || defined(__WINDOWS__)) && defined(__ALL_TRANSPORT__)
TEST_F(CANetworkTest_btc, CACreateEndpoint_Endpoint_NV_N)
{
    CAResult_t result = CACreateEndpoint(CA_DEFAULT_FLAGS, (CATransportAdapter_t)m_caHelper.m_availableNetwork, ENDPOINT_IP, ENDPOINT_PORT, NULL);
    if (result != CA_STATUS_INVALID_PARAM)
    {
        SET_FAILURE(m_caHelper.getFailureMessage("CACreateEndpoint", result, CA_STATUS_INVALID_PARAM));
    }
}
#endif

/**
 * @since 2016-02-25
 * @see void CADestroyEndpoint(CAEndpoint_t *object)
 * @objective Test 'CACreateEndpoint' positively to check addreess value with empty string [ESV]
 * @target CAResult_t CACreateEndpoint(CATransportFlags_t flags, CATransportAdapter_t adapter, const char *addr,
 *         uint16_t port, CAEndpoint_t **object)
 * @test_data  Adress value with empty string
 * @pre_condition none
 * @procedure  1. call the CACreateEndpoint API and pass the empty string as address arguments
 *             2. Check it's return value
 * @post_condition call CADestroyEndpoint to destroy endpoint
 * @expected It will create end point and return CA_STATUS_OK
 */
#if (defined(__LINUX__) || defined(__TIZEN__) || defined(__ANDROID__) || defined(__WINDOWS__)) && defined(__ALL_TRANSPORT__)
TEST_F(CANetworkTest_btc, CACreateEndpoint_ESV_P)
{
    CAEndpoint_t* m_endpoint = NULL;

    CAResult_t m_result = CACreateEndpoint(CA_DEFAULT_FLAGS, (CATransportAdapter_t)m_caHelper.m_availableNetwork, "", ENDPOINT_PORT, &m_endpoint);
    if (m_result != CA_STATUS_OK || m_endpoint == NULL)
    {
        SET_FAILURE(m_caHelper.getFailureMessage("CACreateEndpoint", m_result, CA_STATUS_OK));
        return;
    }

    if(m_endpoint != NULL)
    {
        CADestroyEndpoint(m_endpoint);
    }
}
#endif

/**
 * @since 2016-02-25
 * @see none
 * @objective Test 'CACreateEndpoint' negatively to check unformated reference sample value in endpoint [URSV]
 * @target CAResult_t CACreateEndpoint(CATransportFlags_t flags, CATransportAdapter_t adapter, const char *addr,
 *         uint16_t port, CAEndpoint_t **object);
 * @test_data  Unformated Reference sample value in endpoint
 * @pre_condition none
 * @procedure  1. call the CACreateEndpoint API and pass the unformated reference sample value in endpoint
 *             2. Check it's return value
 * @post_condition none
 * @expected It will not create end point and return CA_STATUS_INVALID_PARAM
 */
#if (defined(__LINUX__) || defined(__TIZEN__) || defined(__ANDROID__) || defined(__WINDOWS__)) && defined(__ALL_TRANSPORT__)
TEST_F(CANetworkTest_btc, CACreateEndpoint_URSV_N)
{
    CAEndpoint_t** m_endpoint = NULL;

    CAResult_t m_result = CACreateEndpoint(CA_DEFAULT_FLAGS, (CATransportAdapter_t)m_caHelper.m_availableNetwork, ENDPOINT_IP, ENDPOINT_PORT, m_endpoint);
    if (m_result != CA_STATUS_INVALID_PARAM)
    {
        SET_FAILURE(m_caHelper.getFailureMessage("CACreateEndpoint", m_result, CA_STATUS_INVALID_PARAM));
    }

    if(m_endpoint != NULL)
    {
        CADestroyEndpoint(*m_endpoint);
    }
}
#endif

/**
 * @since 2014-12-04
 * @see CAResult_t CAInitialize()
 * @see void CATerminate()
 * @objective Test 'CASelectNetwork' positively to select IP as network
 * @target CAResult_t CASelectNetwork(CATransportAdapter_t interestedNetwork)
 * @test_data valid IP Adapter
 * @pre_condition Initialize CA using CAInitialize
 * @procedure  1. Call CASelectNetowrk to select IP as network
 *             2. Check it's return value
 * @post_condition Terminate CA using CATerminate API
 * @expected It will select the given network (IP) and return CA_STATUS_OK
 */
#if (defined(__LINUX__) || defined(__TIZEN__) || defined(__ANDROID__) || defined(__WINDOWS__)) && defined(__ALL_TRANSPORT__)
TEST_F(CANetworkTest_btc, CASelectNetwork_P)
{
    if (!m_caHelper.initialize())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
        return;
    }

    if (!m_caHelper.selectNetwork())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
    }

    CATerminate();
}
#endif

/**
 * @since 2017-06-22
 * @see CAResult_t CAInitialize()
 * @see void CATerminate()
 * @objective Test 'CAGetSelectedNetwork' API. Whether API returns with selected transport
 * @target CAResult_t CAGetSelectedNetwork()
 * @test_data valid transport Adapter
 * @pre_condition Initialize CA using CAInitialize
 * @procedure  1. Select a valid transport adapter using CASelectNetowrk API
 *             2. call CAGetSelectedNetwork to obtained selected transport
 *             3. Check obtained transport value with selected transport value
 * @post_condition Terminate CA using CATerminate API
 * @expected  Obtained transport value should be same as selected transport value
 */
#if (defined(__LINUX__) || defined(__TIZEN__) || defined(__ANDROID__) || defined(__WINDOWS__)) && defined(__ALL_TRANSPORT__)
TEST_F(CANetworkTest_btc, CAGetSelectedNetwork_P)
{
    if (!m_caHelper.initialize())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
        return;
    }

    CAResult_t result = CASelectNetwork(m_caHelper.m_availableNetwork);
    if (result != CA_STATUS_OK)
    {
        SET_FAILURE(m_caHelper.getFailureMessage("CASelectNetwork", CA_STATUS_OK));
        CATerminate();
        return;
    }

    if (m_caHelper.m_availableNetwork != m_caHelper.getselectNetwork())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
    }

    CATerminate();
}
#endif

/**
 * @since 2017-06-22
 * @objective Test 'CAGetSelectedNetwork' API without call CASelectNetwork API
 * @target CAResult_t CAGetSelectedNetwork()
 * @test_data valid IP Adapter
 * @pre_condition Initialize CA using CAInitialize
 * @procedure  1. call CAGetSelectedNetwork to obtained selected network
 *             2. Compare obtained network value against a valid transport
 * @post_condition Terminate CA using CATerminate API
 * @expected obtained network value should not be any valid transport
 */
#if (defined(__LINUX__) || defined(__TIZEN__) || defined(__ANDROID__) || defined(__WINDOWS__)) && defined(__ALL_TRANSPORT__)
TEST_F(CANetworkTest_btc, CAGetSelectedNetwork_N)
{
    if (!m_caHelper.initialize())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
        return;
    }

    CATransportAdapter_t obtainedAdapter = m_caHelper.getselectNetwork();

    IOTIVITYTEST_LOG(DEBUG, "Obtained Adapter value: %d\n", obtainedAdapter);
    IOTIVITYTEST_LOG(DEBUG, "Valid Adapter value: %d\n", m_caHelper.m_availableNetwork);

    if (m_caHelper.m_availableNetwork == obtainedAdapter)
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
    }

    CATerminate();
}
#endif

/**
 * @since 2014-11-28
 * @see CAResult_t CAInitialize()
 * @see CAResult_t CASelectNetwork(CATransportAdapter_t interestedNetwork)
 * @see void CATerminate()
 * @objective Test 'CAUnSelectNetwork' positively to un-select IP as network
 * @target CAResult_t CAUnSelectNetwork(CATransportAdapter_t nonInterestedNetwork)
 * @test_data valid IP Adapter
 * @pre_condition 1. Initialize CA using CAInitialize
 *                2. Call CASelectNetwork with IP as network
 * @procedure  1. Call CAUnSelectNetwork with IP as network
 *             2. Check it's return value
 * @post_condition Terminate CA using CATerminate
 * @expected It will un-select the given network (IP) and return CA_STATUS_OK
 */
#if (defined(__LINUX__) || defined(__TIZEN__) || defined(__ANDROID__) || defined(__WINDOWS__)) && defined(__ALL_TRANSPORT__)
TEST_F(CANetworkTest_btc, CAUnSelectNetwork_P)
{
    if (!m_caHelper.initialize())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
        return;
    }

    if (!m_caHelper.selectNetwork())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
        CATerminate();
        return;
    }

    if (!m_caHelper.unselectNetwork())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
    }

    CATerminate();
}
#endif

/**
 * @since 2014-12-02
 * @see CAResult_t CAInitialize()
 * @see CAResult_t CASelectNetwork(CATransportAdapter_t interestedNetwork)
 * @see CAResult_t CAStartDiscoveryServer()
 * @see CAResult_t CACreateEndpoint(CATransportFlags_t flags, CATransportAdapter_t adapter, const char *addr,
 *         uint16_t port, CAEndpoint_t **object)
 * @see CAResult_t CAGenerateToken(CAToken_t *token)
 * @see void CADestroyEndpoint(CAEndpoint_t *object)
 * @see void CADestroyToken(CAToken_t token)
 * @see void CATerminate()
 * @objective Test CASendRequest positively with valid information
 * @target CAResult_t CASendRequest(const CAEndpoint_t *object, const CARequestInfo_t *requestInfo)
 * @test_data none
 * @pre_condition 1. Initialize CA using CAInitialize
 *                2. Call CASelectNetwork to select the network
 *                3. Call CAStartDiscoveryServer
 *                4. Create endpoint
 *                5. Generate token
 * @procedure 1. Call CASendRequest with valid information
 *            2. Check it's return value
 * @post_condition 1. Destroy token
 *                 2. Destroy endpoint
 *                 3. Terminate CA using CATerminate
 * @expected It will return CA_STATUS_OK
 */
#if (defined(__LINUX__) || defined(__TIZEN__) || defined(__ANDROID__) || defined(__WINDOWS__)) && defined(__ALL_TRANSPORT__)
TEST_F(CANetworkTest_btc, CASendRequest_P)
{
    if (!m_caHelper.initClientNetwork())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
        return;
    }

    char* payload = (char*)"Temp Data";
    CAMethod_t method = CA_GET;
    CAMessageType_t type = CA_MSG_NONCONFIRM;

    if (!m_caHelper.sendRequest((char*)VALID_ENDPOINT_URI, payload, method, type))
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
    }

    CATerminate();
}
#endif

/**
 * @since 2014-12-02
 * @see CAResult_t CAInitialize()
 * @see CAResult_t CASelectNetwork(CATransportAdapter_t interestedNetwork)
 * @see CAResult_t CAStartDiscoveryServer()
 * @see void CATerminate()
 * @objective Test CASendRequest negatively with empty request info and token.
 * @target CAResult_t CASendRequest(const CAEndpoint_t *object, const CARequestInfo_t *requestInfo)
 * @test_data Empty Request Info & Token
 * @pre_condition 1. Initialize CA using CAInitialize
 *                2. Call CASelectNetwork to select the network
 *                3. Call CAStartDiscoveryServer
 * @procedure  1. Call CASendRequest with empty requestInfo and token
 *             2. Check it's return value
 * @post_condition Terminate CA using CATerminate
 * @expected It will return CA_STATUS_INVALID_PARAM
 */
#if (defined(__LINUX__) || defined(__TIZEN__) || defined(__ANDROID__) || defined(__WINDOWS__)) && defined(__ALL_TRANSPORT__)
TEST_F(CANetworkTest_btc, CASendRequest_N)
{
    if (!m_caHelper.initClientNetwork())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
        return;
    }

    CARequestInfo_t requestInfo;
    CAEndpoint_t* endpoint = NULL;
    memset(&requestInfo, 0, sizeof(CARequestInfo_t));

    CAResult_t result = CASendRequest(endpoint, &requestInfo);
    if (result != CA_STATUS_INVALID_PARAM)
    {
        SET_FAILURE(m_caHelper.getFailureMessage("CASendRequest", result, CA_STATUS_INVALID_PARAM));
    }

    CATerminate();
}
#endif

/**
 * @since 2014-12-11
 * @see CAResult_t CAInitialize()
 * @see CAResult_t CASelectNetwork(CATransportAdapter_t interestedNetwork)
 * @see CAResult_t CAStartDiscoveryServer()
 * @see void CATerminate()
 * @objective Test unicast message sending functionality using invalid method
 * @target  CAResult_t CASendRequest(const CAEndpoint_t *object, const CARequestInfo_t *requestInfo)
 * @test_data Invalid method code in CAMethod_t
 * @pre_condition 1. Initialize CA using CAInitialize
 *                2. Call CASelectNetwork to select the network
 *                3. Call CAStartDiscoveryServer
 * @procedure  1. Call CASendRequest with invalid method
 *             2. Check it's return value
 * @post_condition Terminate CA using CATerminate
 * @expected  It will return CA_MEMORY_ALLOC_FAILED
 */
#if (defined(__LINUX__) || defined(__TIZEN__) || defined(__ANDROID__) || defined(__WINDOWS__)) && defined(__ALL_TRANSPORT__)
TEST_F(CANetworkTest_btc, CASendRequestWithInvalidMethod_N)
{
    if (!m_caHelper.initClientNetwork())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
        return;
    }

    CAMethod_t invalidMethod = (CAMethod_t)-1;

    char* uri = (char*) SIM_REQ_ACK;
    char* payload = m_caHelper.s_tcInfo.identifier;

    if (!m_caHelper.sendRequest(uri, payload, invalidMethod, CA_MSG_NONCONFIRM, false, CA_MEMORY_ALLOC_FAILED))
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
        CATerminate();
        return;
    }

    CATerminate();
}
#endif

/**
 * @since 2014-12-02
 * @see CAResult_t CAInitialize()
 * @see CAResult_t CASelectNetwork(CATransportAdapter_t interestedNetwork)
 * @see CAResult_t CAStartListeningServer()
 * @see CAResult_t CACreateEndpoint(CATransportFlags_t flags, CATransportAdapter_t adapter, const char *addr,
 *         uint16_t port, CAEndpoint_t **object)
 * @see CAResult_t CAGenerateToken(CAToken_t *token)
 * @see void CADestroyEndpoint(CAEndpoint_t *object)
 * @see void CADestroyToken(CAToken_t token)
 * @see void CATerminate()
 * @objective Test CASendResponse positively with valid information
 * @target CAResult_t CASendResponse(const CAEndpoint_t *object, const CAResponseInfo_t *responseInfo)
 * @test_data none
 * @pre_condition 1. Initialize CA using CAInitialize
 *                2. Call CASelectNetwork to select the network
 *                3. Start server using CAStartListeningServer
 *                4. Create endpoint
 *                5. Generate token
 * @procedure  Call CASendResponse with endpoint and token
 * @post_condition 1. Destroy token
 *                 2. Destroy endpoint
 *                 3. Terminate CA using CATerminate function
 * @expected It will return CA_STATUS_OK
 */
#if (defined(__LINUX__) || defined(__TIZEN__) || defined(__ANDROID__) || defined(__WINDOWS__)) && defined(__ALL_TRANSPORT__)
TEST_F(CANetworkTest_btc, CASendResponse_P)
{
    if (!m_caHelper.initServerNetwork())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
        return;
    }

    char* payload = (char*)"Temp Data";
    CAResponseResult_t responseResult = CA_EMPTY;
    CAMessageType_t type = CA_MSG_NONCONFIRM;

    if (!m_caHelper.sendResponse((char*)SIM_RES_ACK, payload, responseResult, type))
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
    }

    CATerminate();
}
#endif

/**
 * @since 2014-12-02
 * @see CAResult_t CAInitialize()
 * @see CAResult_t CASelectNetwork(CATransportAdapter_t interestedNetwork)
 * @see CAResult_t CAStartListeningServer()
 * @see void CATerminate()
 * @objective Test CASendResponse with empty response info.
 * @target CAResult_t CASendResponse(const CAEndpoint_t *object, const CAResponseInfo_t *responseInfo)
 * @test_data Empty Request Info
 * @pre_condition 1. Initialize CA using CAInitialize
 *                2. Call CASelectNetwork to select the network
 *                3. Start server using CAStartListeningServer
 * @procedure  1. Call SendResponse with empty response info.
 *             2. Check it's return value
 * @post_condition Terminate CA using CATerminate
 * @expected It will return CA_STATUS_INVALID_PARAM
 */
#if (defined(__LINUX__) || defined(__TIZEN__) || defined(__ANDROID__) || defined(__WINDOWS__)) && defined(__ALL_TRANSPORT__)
TEST_F(CANetworkTest_btc, CASendResponse_N)
{
    if (!m_caHelper.initServerNetwork())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
        return;
    }

    CAResponseInfo_t responseInfo;
    memset(&responseInfo, 0, sizeof(CAResponseInfo_t));
    CAEndpoint_t* endpoint = NULL;

    CAResult_t result = CASendResponse(endpoint, &responseInfo);
    if (result != CA_STATUS_INVALID_PARAM)
    {
        SET_FAILURE(m_caHelper.getFailureMessage("CASendResponse", result, CA_STATUS_INVALID_PARAM));
    }

    CATerminate();
}
#endif

/**
 * @since 2014-12-02
 * @see CAResult_t CAInitialize()
 * @see CAResult_t CASelectNetwork(CATransportAdapter_t interestedNetwork)
 * @see void CATerminate()
 * @objective Test CAGetNetworkInformation positively with valid value
 * @target CAResult_t CAGetNetworkInformation(CAEndpoint_t **info, size_t *size)
 * @test_data none
 * @pre_condition 1. Initialize CA using CAInitialize
 *                2. Call CASelectNetwork to select network
 * @procedure  1. Call CAGetNetworkInformation
 *             2. Check it's return value
 * @post_condition Terminate CA using CATerminate
 * @expected It will return CA_STATUS_OK
 */
#if (defined(__LINUX__) || defined(__TIZEN__) || defined(__ANDROID__) || defined(__WINDOWS__)) && defined(__ALL_TRANSPORT__)
TEST_F(CANetworkTest_btc, CAGetNetworkInformation_P)
{
    if (!m_caHelper.initNetwork())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
        return;
    }

    if(!m_caHelper.getNetworkInfo())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
    }

    CATerminate();
}
#endif

/**
 * @since 2014-12-02
 * @see CAResult_t CAInitialize()
 * @see void CATerminate()
 * @objective Test CAGetNetworkInformation negatively with invalid value
 * @target CAResult_t CAGetNetworkInformation(CAEndpoint_t **info, size_t *size)
 * @test_data Pass NULL value as endpoint and size argument
 * @pre_condition Initialize CA using CAInitialize
 * @procedure call CAGetNetworkInformation with NULL
 * @post_condition Terminate CA using CATerminate
 * @expected It will return CA_STATUS_INVALID_PARAM
 */
#if (defined(__LINUX__) || defined(__TIZEN__) || defined(__ANDROID__) || defined(__WINDOWS__)) && defined(__ALL_TRANSPORT__)
TEST_F(CANetworkTest_btc, CAGetNetworkInformation_N)
{
    if (!m_caHelper.initialize())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
        return;
    }

    CAResult_t result = CAGetNetworkInformation(NULL, NULL);
    if (result != CA_STATUS_INVALID_PARAM)
    {
        SET_FAILURE(m_caHelper.getFailureMessage("CAGetNetworkInformation", result, CA_STATUS_INVALID_PARAM));
    }

    CATerminate();
}
#endif

/**
 * @since 2016-02-24
 * @see CAResult_t CAInitialize()
 * @see CAResult_t CASelectNetwork(CATransportAdapter_t interestedNetwork)
 * @see void CATerminate()
 * @objective Test CAGetNetworkInformation negatively with invalid sample value in Info parameter [Info_USV]
 * @target CAResult_t CAGetNetworkInformation(CAEndpoint_t **info, size_t *size)
 * @test_data unformated sample value as tempInfo argument
 * @pre_condition 1. Initialize CA using CAInitialize
 *                2. Call CASelectNetwork to select the network
 * @procedure  1. Call CAGetNetworkInformation with inavlid info value
 *             2. Check it's return value
 * @post_condition Terminate CA using CATerminate
 * @expected It will return CA_STATUS_INVALID_PARAM
 */
#if (defined(__LINUX__) || defined(__TIZEN__) || defined(__ANDROID__) || defined(__WINDOWS__)) && defined(__ALL_TRANSPORT__)
TEST_F(CANetworkTest_btc, CAGetNetworkInformation_Info_USV_N)
{
    if (!m_caHelper.initNetwork())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
        return;
    }

    CAResult_t m_res = (CAResult_t)NULL;
    CAEndpoint_t **tempInfo = NULL;
    size_t tempSize = 0;

    m_res = CAGetNetworkInformation(tempInfo, &tempSize);
    if (m_res != CA_STATUS_INVALID_PARAM)
    {
        SET_FAILURE(m_caHelper.getFailureMessage("CAGetNetworkInformation", CA_STATUS_INVALID_PARAM));
    }

    CATerminate();
}
#endif

/**
 * @since 2016-02-24
 * @see CAResult_t CAInitialize()
 * @see CAResult_t CASelectNetwork(CATransportAdapter_t interestedNetwork)
 * @see void CATerminate()
 * @objective Test CAGetNetworkInformation negatively with invalid sample value in size parameter [Size_USV]
 * @target CAResult_t CAGetNetworkInformation(CAEndpoint_t **info, size_t *size)
 * @test_data unformated sample value as size argument
 * @pre_condition 1. Initialize CA using CAInitialize
 *                2. Call CASelectNetwork to select the network
 * @procedure  1. Call CAGetNetworkInformation with inavlid size value
 *             2. Check it's return value
 * @post_condition Terminate CA using CATerminate
 * @expected It will return CA_STATUS_INVALID_PARAM
 */
#if (defined(__LINUX__) || defined(__TIZEN__) || defined(__ANDROID__) || defined(__WINDOWS__)) && defined(__ALL_TRANSPORT__)
TEST_F(CANetworkTest_btc, CAGetNetworkInformation_temSize_USV_N)
{
    if (!m_caHelper.initNetwork())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
        return;
    }

    CAResult_t m_res = (CAResult_t)NULL;
    CAEndpoint_t *tempInfo = NULL;
    size_t *tempSize = 0;

    m_res = CAGetNetworkInformation(&tempInfo, tempSize);
    if (m_res != CA_STATUS_INVALID_PARAM)
    {
        SET_FAILURE(m_caHelper.getFailureMessage("CAGetNetworkInformation", CA_STATUS_INVALID_PARAM));
    }

    CATerminate();
}
#endif


#ifdef TCP_ADAPTER
/**
 * @since 2016-08-05
 * @see CAResult_t CAInitialize()
 * @see CAResult_t CASelectNetwork(CATransportAdapter_t interestedNetwork)
 * @see void CATerminate()
 * @objective Test whether CARegisterKeepAliveHandler API can register a valid callback function
 * @target void CARegisterKeepAliveHandler(CAKeepAliveConnectionCallback ConnHandler)
 * @test_data A callback function to pass the connection information
 * @pre_condition 1. Initialize CA using CAInitialize
 *                2. Call CASelectNetwork to select Coap Over TCP network
 * @procedure  Call CARegisterKeepAliveHandler with a valid callback function
 * @post_condition Terminate CA using CATerminate
 * @expected No abnormal termination of program
 */
#if (defined(__LINUX__) || defined(__ANDROID__) || defined(__TIZEN__) || defined(__WINDOWS__)) && defined(__TCP__)
TEST_F(CANetworkTest_btc, CARegisterKeepAliveHandler_P)
{
    if (!m_caHelper.initNetwork())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
        return;
    }

    CARegisterKeepAliveHandler(CAHelper::keepAliveHandler);

    CATerminate();
}
#endif

/**
 * @since 2016-08-05
 * @see CAResult_t CAInitialize()
 * @see CAResult_t CASelectNetwork(CATransportAdapter_t interestedNetwork)
 * @see void CATerminate()
 * @objective Test whether CARegisterKeepAliveHandler API can handle an invalid callback function
 * @target void CARegisterKeepAliveHandler(CAKeepAliveConnectionCallback ConnHandler)
 * @test_data NULL value as callback function
 * @pre_condition 1. Initialize CA using CAInitialize
 *                2. Call CASelectNetwork to select Coap Over TCP network
 * @procedure  Call CARegisterKeepAliveHandler with null value
 * @post_condition Terminate CA using CATerminate
 * @expected No abnormal termination of program
 */
#if (defined(__LINUX__) || defined(__ANDROID__) || defined(__TIZEN__) || defined(__WINDOWS__)) && defined(__TCP__)
TEST_F(CANetworkTest_btc, CARegisterKeepAliveHandler_N)
{
    if (!m_caHelper.initNetwork())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
        return;
    }

    CARegisterKeepAliveHandler(NULL);

    CATerminate();
}
#endif

#endif

/**
 * @since 2016-08-05
 * @see CAResult_t CAInitialize()
 * @see CAResult_t CASelectNetwork(CATransportAdapter_t interestedNetwork)
 * @see CAResult_t  CAStartListeningServer()
 * @see void CATerminate()
 * @objective Test whether CAGetAssignedPortNumber API can obtained normal port number
 * @target uint16_t CAGetAssignedPortNumber(CATransportAdapter_t adapter, CATransportFlags_t flag)
 * @test_data CA_IPV4 as flag value
 * @pre_condition 1. Initialize CA using CAInitialize
 *                2. Call CASelectNetwork to select the network
 *                3. Call CAStartListeningServer, If network is Coap Over TCP
 * @procedure  1. Call CAGetAssignedPortNumber with CA_IPV4 as flag value
 *             2. Check it's return value
 * @post_condition Terminate CA using CATerminate
 * @expected It will return ephemeral port number
 */
#if ((defined(__LINUX__) || defined(__ANDROID__) || defined(__WINDOWS__)) && defined(__ALL_TRANSPORT__)) || (defined(__TIZEN__) && (defined(__IP__) || defined(__TCP__)))
TEST_F(CANetworkTest_btc, CAGetNonSecurePortNumber_P)
{
#ifdef TCP_ADAPTER
    caglobals.server = true;
#endif

    if (!m_caHelper.initNetwork())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
        return;
    }

#ifdef TCP_ADAPTER
    CAResult_t result = CAStartListeningServer();
    if(result != CA_STATUS_OK)
    {
        SET_FAILURE(m_caHelper.getFailureMessage("CAStartListeningServer", result, CA_STATUS_OK));
    }
#endif

    int port = CAGetAssignedPortNumber((CATransportAdapter_t)m_caHelper.m_availableNetwork, CA_IPV4);

    if(port <= 0)
    {
        SET_FAILURE("Unable to get assigned port number");
    }

    CATerminate();
}
#endif

#ifdef __WITH_DTLS__
/**
 * @since 2016-08-05
 * @see CAResult_t CAInitialize()
 * @see CAResult_t CASelectNetwork(CATransportAdapter_t interestedNetwork)
 * @see void CATerminate()
 * @objective Test whether CAGetAssignedPortNumber API can obtained secure port number
 * @target uint16_t CAGetAssignedPortNumber(CATransportAdapter_t adapter, CATransportFlags_t flag)
 * @test_data Combination of CA_IPV4 and CA_SECURE as flag value
 * @pre_condition 1. Initialize CA using CAInitialize
 *                2. Call CASelectNetwork to select the network
 * @procedure  1. Call CAGetAssignedPortNumber with combination CA_IPV4 and CA_SECURE as flag value
 *             2. Check it's return value
 * @post_condition Terminate CA using CATerminate
 * @expected It will return ephemeral port number
 */
#if (defined(__LINUX__) || defined(__TIZEN__) || defined(__ANDROID__) || defined(__WINDOWS__)) && defined(__IP__)
TEST_F(CANetworkTest_btc, CAGetSecurePortNumber_P)
{
    if (!m_caHelper.initNetwork())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
        return;
    }

    CATransportFlags_t secureflag = (CATransportFlags_t)(CA_IPV4 | CA_SECURE);
    int port = CAGetAssignedPortNumber((CATransportAdapter_t)m_caHelper.m_availableNetwork, secureflag);

    if(port <= 0)
    {
        SET_FAILURE("Unable to get assigned secured port number");
    }

    CATerminate();
}
#endif
#endif

/**
 * @since 2016-08-05
 * @see CAResult_t CAInitialize()
 * @see CAResult_t CASelectNetwork(CATransportAdapter_t interestedNetwork)
 * @see void CATerminate()
 * @objective Test whether CAGetAssignedPortNumber API can handle invalid adapter type
 * @target uint16_t CAGetAssignedPortNumber(CATransportAdapter_t adapter, CATransportFlags_t flag)
 * @test_data invalid adapter type
 * @pre_condition 1. Initialize CA using CAInitialize
 *                2. Call CASelectNetwork to select the network
 * @procedure  1. Call CAGetAssignedPortNumber with invalid adapter type
 *             2. Check it's return value
 * @post_condition Terminate CA using CATerminate
 * @expected It will return 0 to indicate that API is failed to get port number
 */
#if ((defined(__LINUX__) || defined(__ANDROID__) || defined(__WINDOWS__)) && defined(__ALL_TRANSPORT__)) || (defined(__TIZEN__) && (defined(__IP__) || defined(__TCP__)))
TEST_F(CANetworkTest_btc, CAGetAssignedPortNumberForInvalidAdapter_N)
{
    if (!m_caHelper.initNetwork())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
        return;
    }

    int port = CAGetAssignedPortNumber((CATransportAdapter_t)CA_INVALID_ADAPTER, CA_TRANSPORT_FLAG);

    if(port != 0)
    {
        SET_FAILURE("Able to get port number for invalid network");
    }

    CATerminate();
}
#endif


/**
 * @since 2016-08-05
 * @see CAResult_t CAInitialize()
 * @see CAResult_t CASelectNetwork(CATransportAdapter_t interestedNetwork)
 * @see void CATerminate()
 * @objective Test whether CAGetAssignedPortNumber API can handle invalid flag
 * @target uint16_t CAGetAssignedPortNumber(CATransportAdapter_t adapter, CATransportFlags_t flag)
 * @test_data invalid flag value
 * @pre_condition 1. Initialize CA using CAInitialize
 *                2. Call CASelectNetwork to select the network
 * @procedure  1. Call CAGetAssignedPortNumber with inavlid flag
 *             2. Check it's return value
 * @post_condition Terminate CA using CATerminate
 * @expected It will return 0 to indicate that API is failed to get port number
 */
#if ((defined(__LINUX__) || defined(__ANDROID__) || defined(__WINDOWS__)) && defined(__ALL_TRANSPORT__)) || (defined(__TIZEN__) && (defined(__IP__) || defined(__TCP__)))
TEST_F(CANetworkTest_btc, CAGetAssignedPortNumberForInvalidFlag_N)
{
    if (!m_caHelper.initNetwork())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
        return;
    }

    int port = CAGetAssignedPortNumber((CATransportAdapter_t)m_caHelper.m_availableNetwork, CA_INVALID_FLAG);

    if(port != 0)
    {
        SET_FAILURE("Able to get port number for invalid flag");
    }

    CATerminate();
}
#endif

/**
 * @since 2016-08-05
 * @see CAResult_t CAInitialize()
 * @see void CATerminate()
 * @objective Test CASetPortNumberToAssign positively
 * @target CAResult_t CASetPortNumberToAssign(CATransportAdapter_t adapter, CATransportFlags_t flag, uint16_t port)
 * @test_data An ephemeral port number
 * @pre_condition Initialize CA using CAInitialize
 * @procedure  1. Call CASetPortNumberToAssign with a valid port number
 *             2. Check it's return value
 * @post_condition Terminate CA using CATerminate
 * @expected It will return CA_STATUS_OK
 */
#if ((defined(__LINUX__) || defined(__ANDROID__) || defined(__WINDOWS__)) && defined(__ALL_TRANSPORT__)) || (defined(__TIZEN__) && (defined(__IP__) || defined(__TCP__)))
TEST_F(CANetworkTest_btc, CASetPortNumberToAssign_P)
{
    if (!m_caHelper.initNetwork())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
        return;
    }

    CAResult_t result = CASetPortNumberToAssign((CATransportAdapter_t)m_caHelper.m_availableNetwork,
                                   CA_TRANSPORT_FLAG, ENDPOINT_PORT);

    if(result != CA_STATUS_OK)
    {
        SET_FAILURE("Unable to set port number");
    }

    CATerminate();
}
#endif

/**
 * @since 2016-08-05
 * @see CAResult_t CAInitialize()
 * @see void CATerminate()
 * @objective Test whether CASetPortNumberToAssign API can handle invalid adapter type
 * @target CAResult_t CASetPortNumberToAssign(CATransportAdapter_t adapter, CATransportFlags_t flag, uint16_t port)
 * @test_data An invalid port number
 * @pre_condition Initialize CA using CAInitialize
 * @procedure  1. Call CASetPortNumberToAssign with inavlid adapter type
 *             2. Check it's return value
 * @post_condition Terminate CA using CATerminate
 * @expected It will return CA_NOT_SUPPORTED
 */
#if ((defined(__LINUX__) || defined(__ANDROID__) || defined(__WINDOWS__)) && defined(__ALL_TRANSPORT__)) || (defined(__TIZEN__) && (defined(__IP__) || defined(__TCP__)))
TEST_F(CANetworkTest_btc, CASetPortNumberToAssignWithInvalidAdapter_N)
{
    if (!m_caHelper.initialize())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
        return;
    }

    int invalidAdapterType = 0;

    CAResult_t result = CASetPortNumberToAssign((CATransportAdapter_t)invalidAdapterType,
                                   CA_TRANSPORT_FLAG, ENDPOINT_PORT);

    if(result != CA_NOT_SUPPORTED)
    {
        SET_FAILURE("Able to set port number");
    }
}
#endif


/**
 * @since 2016-08-05
 * @see CAResult_t CAInitialize()
 * @see void CATerminate()
 * @objective Test whether CASetPortNumberToAssign API can handle invalid flag
 * @target CAResult_t CASetPortNumberToAssign(CATransportAdapter_t adapter, CATransportFlags_t flag, uint16_t port)
 * @test_data An invalid port number
 * @pre_condition Initialize CA using CAInitialize
 * @procedure  1. Call CASetPortNumberToAssign with inavlid flag
 *             2. Check it's return value
 * @post_condition Terminate CA using CATerminate
 * @expected It will return CA_NOT_SUPPORTED
 */
#if ((defined(__LINUX__) || defined(__ANDROID__) || defined(__WINDOWS__)) && defined(__ALL_TRANSPORT__)) || (defined(__TIZEN__) && (defined(__IP__) || defined(__TCP__)))
TEST_F(CANetworkTest_btc, CASetPortNumberToAssignWithInvalidFlag_N)
{
    if (!m_caHelper.initialize())
    {
        SET_FAILURE(m_caHelper.getFailureMessage());
        return;
    }

    CAResult_t result = CASetPortNumberToAssign((CATransportAdapter_t)m_caHelper.m_availableNetwork,
                                   CA_INVALID_FLAG, ENDPOINT_PORT);

    if(result != CA_NOT_SUPPORTED)
    {
        SET_FAILURE("Able to set port number");
    }
}
#endif
