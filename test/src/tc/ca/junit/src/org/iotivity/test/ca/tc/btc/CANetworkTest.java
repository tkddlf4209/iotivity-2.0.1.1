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

package org.iotivity.test.ca.tc.btc;

import android.content.Context;
import android.test.AndroidTestCase;
import org.iotivity.CAJni;
import org.iotivity.test.ca.tc.helper.*;

import static org.iotivity.test.ca.tc.helper.TestInfo.*;

public class CANetworkTest extends AndroidTestCase {

    public CANetworkTest() {
    }

    protected void setUp() throws Exception {
        super.setUp();
        CAJni.readConfig();
        CAJni.setContext(getContext(), MainActivity.getActivity());
    }

    protected void tearDown() throws Exception {
        super.tearDown();
    }

    /**
     * @since 2014-09-17
     * @see CAResult_t CAInitialize()
     * @see void CATerminate()
     * @objective Test 'CAStartListeningServer' positively to start the
     *            listening service
     * @target CAResult_t CAStartListeningServer()
     * @transport bt, ble
     * @test_data Request Messages none
     * @pre_condition 1. Initialize CA using CAInitialize 2. Select IP as
     *                network
     * @procedure 1. Call the CAStartListeningServer API 2. Check it's return
     *            value
     * @post_condition Terminate CA using CATerminate
     * @expected It will start the listening server and will return CA_STATUS_OK
     */
    public void testCAStartListeningServer_P() {
        if (!CAJni.initServerNetwork()) {
            fail();
        }

        CAJni.terminate();
    }

    /**
     * @since 2016-02-29
     * @see CAResult_t CAInitialize()
     * @see CAResult_t CASelectNetwork(CATransportAdapter_t interestedNetwork)
     * @see CAResult_t CAStartListeningServer()
     * @see void CATerminate()
     * @objective Test 'CAStopListeningServer' positively to stop the listening
     *            service [SRC]
     * @target CAResult_t CAStopListeningServer()
     * @transport bt, ble
     * @test_data Request Messages none
     * @pre_condition 1. Initialize CA using CAInitialize 2. Call
     *                CASelectNetwork to select network 3. Call
     *                CAStartListeningServer to start server
     * @procedure 1. Call the CAStopListeningServer API 2. Check it's return
     *            value
     * @post_condition Terminate CA using CATerminate
     * @expected It will stop the listening server and will return CA_STATUS_OK
     */
    public void testCAStopListeningServer_SRC_P() {
        if (!CAJni.initServerNetwork()) {
            fail();
        }

        if (!CAJni.stopListeningServer(CA_STATUS_OK)) {
            fail();
        }

        CAJni.terminate();
    }

    /**
     * @since 2014-09-17
     * @see CAResult_t CAInitialize()
     * @see void CATerminate()
     * @objective Test 'CAStartDiscoveryServer' positively to start the
     *            discovery service
     * @target CAResult_t CAStartDiscoveryServer()
     * @transport bt, ble
     * @test_data Request Messages none
     * @pre_condition 1. Initialize CA using CAInitialize 2. Select IP as
     *                network
     * @procedure 1. Call the CAStartDiscoveryServer API 2. Check it's return
     *            value
     * @post_condition Terminate CA using CATerminate function
     * @expected It will start the discovery server and will return CA_STATUS_OK
     */
    public void testCAStartDiscoveryServer_P() {
        if (!CAJni.initClientNetwork()) {
            fail();
            return;
        }

        CAJni.terminate();
    }

    /**
     * @since 2014-11-28
     * @see void CADestroyEndpoint(CAEndpoint_t *object)
     * @objective Test 'CACreateEndpoint' positively to create a remote endpoint
     * @target CAResult_t CACreateEndpoint(CATransportFlags_t flags,
     *         CATransportAdapter_t adapter, const char *addr, uint16_t port,
     *         CAEndpoint_t **object)
     * @transport bt, ble
     * @test_data Request Messages none
     * @pre_condition none
     * @procedure call the CACreateEndpoint API and pass the valid flags,
     *            adapter, addr, port & endpoint reference as arguments
     * @post_condition call CADestroyEndpoint to destroy the endpoint
     * @expected 1. It will create the endpoint object and return CA_STATUS_OK
     *           2. CAEndpoint must not be NULL.
     */

    public void testCACreateEndpoint_P() {
        if (!CAJni.createEndpoint(ENDPOINT_VALID, CA_STATUS_OK)) {
            fail();
        }

        CAJni.destroyEndpoint(ENDPOINT_VALID);
    }

    /**
     * @since 2016-03-02
     * @see none
     * @objective Test 'CACreateEndpoint' negatively to check against invalid
     *            value in flags [flags-ECRC]
     * @target CAResult_t CACreateEndpoint(CATransportFlags_t flags,
     *         CATransportAdapter_t adapter, const char *addr, uint16_t port,
     *         CAEndpoint_t **object)
     * @transport bt, ble
     * @test_data Request Messages Invalid flags
     * @pre_condition none
     * @procedure 1. call the CACreateEndpoint API and pass the invalid value as
     *            flags arguments 2. Check it's return value
     * @post_condition none
     * @expected It return CA_STATUS_OK
     */
    public void testCACreateEndpoint_Flag_ECRC_N() {
        if (!CAJni.createEndpoint(
                ENDPOINT_INVALID_FLAG, CA_STATUS_OK)) {
            fail();
        }
    }

    /**
     * @since 2016-03-02
     * @see void CADestroyEndpoint(CAEndpoint_t *object)
     * @objective Test 'CACreateEndpoint' negatively to check against NULL value
     *            in address [address-NV]
     * @target CAResult_t CACreateEndpoint(CATransportFlags_t flags,
     *         CATransportAdapter_t adapter, const char *addr, uint16_t port,
     *         CAEndpoint_t **object)
     * @transport bt, ble
     * @test_data Request Messages NULL address
     * @pre_condition none
     * @procedure 1. call the CACreateEndpoint API and pass the NULL as address
     *            arguments 2. Check it's return value
     * @post_condition call CADestroyEndpoint to destroy the endpoint
     * @expected It will create end point and return CA_STATUS_OK
     */
    public void testCACreateEndpoint_Address_NV_N() {
        if (!CAJni.createEndpoint(
                ENDPOINT_INVALID_ADDRESS, CA_STATUS_OK)) {
            fail();
        }
    }

    /**
     * @since 2016-03-02
     * @see none
     * @objective Test 'CACreateEndpoint' negatively to check against NULL value
     *            in endpoint object [endpoint-NV]
     * @target CAResult_t CACreateEndpoint(CATransportFlags_t flags,
     *         CATransportAdapter_t adapter, const char *addr, uint16_t port,
     *         CAEndpoint_t **object)
     * @transport bt, ble
     * @test_data Request Messages NULL endpoint reference
     * @pre_condition none
     * @procedure 1. call the CACreateEndpoint API and pass the NULL as endpoint
     *            arguments 2. Check it's return value
     * @post_condition none
     * @expected It will fail to create end point and return
     *           CA_STATUS_INVALID_PARAM
     */
    public void testCACreateEndpoint_Endpoint_NV_N() {
        if (!CAJni.createEndpoint(
                ENDPOINT_INVALID_ENDPOINT, CA_STATUS_INVALID_PARAM)) {
            fail();
        }
    }

    /**
     * @since 2016-03-02
     * @see void CADestroyEndpoint(CAEndpoint_t *object)
     * @objective Test 'CACreateEndpoint' positively to check addreess value
     *            with empty string [ESV]
     * @target CAResult_t CACreateEndpoint(CATransportFlags_t flags,
     *         CATransportAdapter_t adapter, const char *addr, uint16_t port,
     *         CAEndpoint_t **object)
     * @transport bt, ble
     * @test_data Request Messages Adress value with empty string
     * @pre_condition none
     * @procedure 1. call the CACreateEndpoint API and pass the empty string as
     *            address arguments 2. Check it's return value
     * @post_condition call CADestroyEndpoint to destroy endpoint
     * @expected It will create end point and return CA_STATUS_OK
     */
    public void testCACreateEndpoint_ESV_P() {
        if (!CAJni.createEndpoint(
                ENDPOINT_ESV_ADDRESS, CA_STATUS_OK)) {
            fail();
        }
    }

    /**
     * @since 2016-03-02
     * @see none
     * @objective Test 'CACreateEndpoint' negatively to check unformated
     *            reference sample value in endpoint [URSV]
     * @target CAResult_t CACreateEndpoint(CATransportFlags_t flags,
     *         CATransportAdapter_t adapter, const char *addr, uint16_t port,
     *         CAEndpoint_t **object);
     * @transport bt, ble
     * @test_data Request Messages Unformated Reference sample value in endpoint
     * @pre_condition none
     * @procedure 1. call the CACreateEndpoint API and pass the unformated
     *            reference sample value in endpoint 2. Check it's return value
     * @post_condition none
     * @expected It will not create end point and return CA_STATUS_INVALID_PARAM
     */
    public void testCACreateEndpoint_URSV_N() {
        if (!CAJni.createEndpoint(
                ENDPOINT_URV_ENDPOINT, CA_STATUS_INVALID_PARAM)) {
            fail();
        }
    }

    /**
     * @since 2016-03-02
     * @see none
     * @objective Test 'CADestroyEndpoint' negatively to check with NULL value
     *            [NV]
     * @target void CADestroyEndpoint(CAEndpoint_t *object)
     * @transport bt, ble
     * @test_data Request Messages NULL value in endpoint
     * @pre_condition none
     * @procedure 1. call the CADestroyEndpoint API and pass the NULL value 2.
     *            Check it's return value
     * @post_condition none
     * @expected It will invoke the API without any error/exceptio
     */
    public void testCADestroyEndpoint_NV_N() {
        if (!CAJni.destroyEndpoint(ENDPOINT_NULL)) {
            fail();
        }
    }

    /**
     * @since 2014-09-17
     * @see CAResult_t CAInitialize()
     * @see void CATerminate()
     * @objective Test 'CASelectNetwork' positively to select IP as network
     * @target CAResult_t CASelectNetwork(CATransportAdapter_t
     *         interestedNetwork)
     * @transport bt, ble
     * @test_data Request Messages valid IP Adapter
     * @pre_condition Initialize CA using CAInitialize
     * @procedure 1. Call CASelectNetowrk to select IP as network 2. Check it's
     *            return value
     * @post_condition Terminate CA using CATerminate API
     * @expected It will select the given network (IP) and return CA_STATUS_OK
     */
    public void testCASelectNetwork_P() {
        if (!CAJni.initialize()) {
            fail();
            return;
        }

        if (!CAJni.selectNetwork()) {
            fail();
        }

        CAJni.terminate();
    }

    /**
     * @since 2014-09-17
     * @see CAResult_t CAInitialize()
     * @see CAResult_t CASelectNetwork(CATransportAdapter_t interestedNetwork)
     * @see void CATerminate()
     * @objective Test 'CAUnSelectNetwork' positively to un-select IP as network
     * @target CAResult_t CAUnSelectNetwork(CATransportAdapter_t
     *         nonInterestedNetwork)
     * @transport bt, ble
     * @test_data Request Messages valid IP Adapter
     * @pre_condition 1. Initialize CA using CAInitialize 2. Call
     *                CASelectNetwork with IP as network
     * @procedure 1. Call CAUnSelectNetwork with IP as network 2. Check it's
     *            return value
     * @post_condition Terminate CA using CATerminate
     * @expected It will un-select the given network (IP) and return
     *           CA_STATUS_OK
     */
    public void testCAUnSelectNetwork_P() {
        if (!CAJni.initialize()) {
            fail();
            return;
        }

        if (!CAJni.selectNetwork()) {
            fail();
        }

        if (!CAJni.unSelectNetwork()) {
            fail();
        }

        CAJni.terminate();
    }

    /**
     * @since 2014-09-17
     * @see CAResult_t CAInitialize()
     * @see CAResult_t CASelectNetwork(CATransportAdapter_t interestedNetwork)
     * @see CAResult_t CAStartDiscoveryServer()
     * @see CAResult_t CACreateEndpoint(CATransportFlags_t flags,
     *      CATransportAdapter_t adapter, const char *addr, uint16_t port,
     *      CAEndpoint_t **object)
     * @see CAResult_t CAGenerateToken(CAToken_t *token)
     * @see void CADestroyEndpoint(CAEndpoint_t *object)
     * @see void CADestroyToken(CAToken_t token)
     * @see void CATerminate()
     * @objective Test CASendRequest positively with valid information
     * @target CAResult_t CASendRequest(const CAEndpoint_t *object, const
     *         CARequestInfo_t *requestInfo)
     * @transport bt, ble
     * @test_data Request Messages none
     * @pre_condition 1. Initialize CA using CAInitialize 2. Call
     *                CASelectNetwork to select the network 3. Call
     *                CAStartDiscoveryServer 4. Create endpoint 5. Generate
     *                token
     * @procedure 1. Call CASendRequest with valid information 2. Check it's
     *            return value
     * @post_condition 1. Destroy token 2. Destroy endpoint 3. Terminate CA
     *                 using CATerminate
     * @expected It will return CA_STATUS_OK
     */
    public void testCASendRequest_P() {
        if (!CAJni.initClientNetwork()) {
            fail();
            return;
        }

        if (!CAJni.sendRequest(URI_TYPE_ARBITRARY_ENDPOINT, PAYLOAD_TYPE_NORMAL,
                CA_MSG_NONCONFIRM, CA_GET, HEADER_NONE, false)) {
            fail();
            return;
        }

        CAJni.terminate();
    }

    /**
     * @since 2014-09-17
     * @see CAResult_t CAInitialize()
     * @see CAResult_t CASelectNetwork(CATransportAdapter_t interestedNetwork)
     * @see CAResult_t CAStartDiscoveryServer()
     * @see void CATerminate()
     * @objective Test CASendRequest negatively with empty request info and
     *            token.
     * @target CAResult_t CASendRequest(const CAEndpoint_t *object, const
     *         CARequestInfo_t *requestInfo)
     * @transport bt, ble
     * @test_data Request Messages Empty Request Info & Token
     * @pre_condition 1. Initialize CA using CAInitialize 2. Call
     *                CASelectNetwork to select the network 3. Call
     *                CAStartDiscoveryServer
     * @procedure 1. Call CASendRequest with empty requestInfo and token 2.
     *            Check it's return value
     * @post_condition Terminate CA using CATerminate
     * @expected It will return CA_STATUS_INVALID_PARAM
     */
    public void testCASendRequest_N() {
        if (!CAJni.initClientNetwork()) {
            fail();
            return;
        }

        if (!CAJni.sendRequest(URI_TYPE_NULL, PAYLOAD_TYPE_NORMAL,
                CA_MSG_NONCONFIRM, CA_GET, HEADER_NONE, false)) {
            fail();
            return;
        }

        CAJni.terminate();
    }

    /**
     * @since 2014-09-17
     * @see CAResult_t CAInitialize()
     * @see CAResult_t CASelectNetwork(CATransportAdapter_t interestedNetwork)
     * @see CAResult_t CAStartListeningServer()
     * @see CAResult_t CACreateEndpoint(CATransportFlags_t flags,
     *      CATransportAdapter_t adapter, const char *addr, uint16_t port,
     *      CAEndpoint_t **object)
     * @see CAResult_t CAGenerateToken(CAToken_t *token)
     * @see void CADestroyEndpoint(CAEndpoint_t *object)
     * @see void CADestroyToken(CAToken_t token)
     * @see void CATerminate()
     * @objective Test CASendResponse positively with valid information
     * @target CAResult_t CASendResponse(const CAEndpoint_t *object, const
     *         CAResponseInfo_t *responseInfo)
     * @transport bt, ble
     * @test_data Request Messages none
     * @pre_condition 1. Initialize CA using CAInitialize 2. Call
     *                CASelectNetwork to select the network 3. Start server
     *                using CAStartListeningServer 4. Create endpoint 5.
     *                Generate token
     * @procedure Call CASendResponse with endpoint and token
     * @post_condition 1. Destroy token 2. Destroy endpoint 3. Terminate CA
     *                 using CATerminate function
     * @expected It will return CA_STATUS_OK
     */
    public void testCASendResponse_P() {
        if (!CAJni.initServerNetwork()) {
            fail();
            return;
        }

        if (!CAJni.sendResponse(URI_TYPE_ARBITRARY_ENDPOINT, PAYLOAD_TYPE_NORMAL,
                CA_MSG_NONCONFIRM, CA_VALID, HEADER_NONE)) {
            fail();
            return;
        }

        CAJni.terminate();
    }

    /**
     * @since 2014-09-17
     * @see CAResult_t CAInitialize()
     * @see CAResult_t CASelectNetwork(CATransportAdapter_t interestedNetwork)
     * @see CAResult_t CAStartListeningServer()
     * @see void CATerminate()
     * @objective Test CASendResponse with empty response info.
     * @target CAResult_t CASendResponse(const CAEndpoint_t *object, const
     *         CAResponseInfo_t *responseInfo)
     * @transport bt, ble
     * @test_data Request Messages Empty Request Info
     * @pre_condition 1. Initialize CA using CAInitialize 2. Call
     *                CASelectNetwork to select the network 3. Start server
     *                using CAStartListeningServer
     * @procedure 1. Call SendResponse with empty response info. 2. Check it's
     *            return value
     * @post_condition Terminate CA using CATerminate
     * @expected It will return CA_STATUS_INVALID_PARAM
     */
    public void testCASendResponse_N() {
        if (!CAJni.initServerNetwork()) {
            fail();
            return;
        }

        if (!CAJni.sendResponse(URI_TYPE_NULL, PAYLOAD_TYPE_NORMAL,
                CA_MSG_NONCONFIRM, CA_VALID, HEADER_NONE)) {
            fail();
            return;
        }

        CAJni.terminate();
    }

    /**
     * @since 2014-09-17
     * @see CAResult_t CAInitialize()
     * @see CAResult_t CASelectNetwork(CATransportAdapter_t interestedNetwork)
     * @see void CATerminate()
     * @objective Test CAGetNetworkInformation positively with valid value
     * @target CAResult_t CAGetNetworkInformation(CAEndpoint_t **info, uint32_t
     *         *size)
     * @transport bt, ble
     * @test_data Request Messages none
     * @pre_condition 1. Initialize CA using CAInitialize 2. Call
     *                CASelectNetwork to select network
     * @procedure 1. Call CAGetNetworkInformation 2. Check it's return value
     * @post_condition Terminate CA using CATerminate
     * @expected It will return CA_STATUS_OK
     */

    public void testCAGetNetworkInformation_P() {
        if (!CAJni.initNetwork()) {
            fail();
            return;
        }

        if (!CAJni.getNetworkInfomation(NETWORKINFO_ENDPOINT_VALID,
            NETWORKINFO_SIZE_VALID, CA_STATUS_OK)) {
            fail();
        }

        CAJni.terminate();
    }

    /**
     * @since 2014-09-17
     * @see CAResult_t CAInitialize()
     * @see void CATerminate()
     * @objective Test CAGetNetworkInformation negatively with invalid value
     * @target CAResult_t CAGetNetworkInformation(CAEndpoint_t **info, uint32_t
     *         *size)
     * @transport bt, ble
     * @test_data Request Messages Pass NULL value as endpoint and size argument
     * @pre_condition Initialize CA using CAInitialize
     * @procedure call CAGetNetworkInformation with NULL
     * @post_condition Terminate CA using CATerminate
     * @expected It will return CA_STATUS_INVALID_PARAM
     */
    public void testCAGetNetworkInformation_N() {
        if (!CAJni.initialize()) {
            fail();
            return;
        }

        if (!CAJni.getNetworkInfomation(NETWORKINFO_ENDPOINT_NULL,
                NETWORKINFO_SIZE_NULL, CA_STATUS_INVALID_PARAM)) {
            fail();
        }

        CAJni.terminate();
    }

    /**
     * @since 2016-03-02
     * @see CAResult_t CAInitialize()
     * @see CAResult_t CASelectNetwork(CATransportAdapter_t interestedNetwork)
     * @see void CATerminate()
     * @objective Test CAGetNetworkInformation negatively with invalid sample
     *            value in Info parameter [Info_USV]
     * @target CAResult_t CAGetNetworkInformation(CAEndpoint_t **info, uint32_t
     *         *size)
     * @transport bt, ble
     * @test_data Request Messages unformated sample value as tempInfo argument
     * @pre_condition 1. Initialize CA using CAInitialize 2. Call
     *                CASelectNetwork to select the network
     * @procedure 1. Call CAGetNetworkInformation with inavlid info value 2.
     *            Check it's return value
     * @post_condition Terminate CA using CATerminate
     * @expected It will return CA_STATUS_INVALID_PARAM
     */
    public void testCAGetNetworkInformation_Info_USV_N() {
        if (!CAJni.initNetwork()) {
            fail();
            return;
        }

        if (!CAJni.getNetworkInfomation(
                NETWORKINFO_ENDPOINT_NULL, NETWORKINFO_SIZE_VALID, CA_STATUS_INVALID_PARAM)) {
            fail();
        }

        CAJni.terminate();
    }

    /**
     * @since 2016-03-02
     * @see CAResult_t CAInitialize()
     * @see CAResult_t CASelectNetwork(CATransportAdapter_t interestedNetwork)
     * @see void CATerminate()
     * @objective Test CAGetNetworkInformation negatively with invalid sample
     *            value in size parameter [Size_USV]
     * @target CAResult_t CAGetNetworkInformation(CAEndpoint_t **info, uint32_t
     *         *size)
     * @transport bt, ble
     * @test_data Request Messages unformated sample value as size argument
     * @pre_condition 1. Initialize CA using CAInitialize 2. Call
     *                CASelectNetwork to select the network
     * @procedure 1. Call CAGetNetworkInformation with inavlid size value 2.
     *            Check it's return value
     * @post_condition Terminate CA using CATerminate
     * @expected It will return CA_STATUS_INVALID_PARAM
     */
    public void testCAGetNetworkInformation_temSize_USV_N() {
        if (!CAJni.initNetwork()) {
            fail();
            return;
        }

        if (!CAJni.getNetworkInfomation(
                NETWORKINFO_ENDPOINT_VALID, NETWORKINFO_SIZE_USV, CA_STATUS_INVALID_PARAM)) {
            fail();
        }

        CAJni.terminate();
    }
}
