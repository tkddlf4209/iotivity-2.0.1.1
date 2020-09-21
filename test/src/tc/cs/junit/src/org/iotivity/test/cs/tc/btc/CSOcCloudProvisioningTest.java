/******************************************************************
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************/

package org.iotivity.test.cs.tc.btc;

import java.util.ArrayList;

import org.iotivity.base.ModeType;
import org.iotivity.base.OcPlatform;
import org.iotivity.base.PlatformConfig;
import org.iotivity.base.QualityOfService;
import org.iotivity.base.ServiceType;
import org.iotivity.ca.CaInterface;
import org.iotivity.ca.OicCipher;
import org.iotivity.base.OcCloudProvisioning;
import org.iotivity.base.OcConnectivityType;
import org.iotivity.test.cs.tc.helper.CSConstants;
import org.iotivity.test.cs.tc.helper.OcCloudProvisioningHelper;
import org.iotivity.testcase.IoTivityLog;
import org.iotivity.testcase.IoTivityTc;

public class CSOcCloudProvisioningTest extends IoTivityTc {

    private OcCloudProvisioningHelper mOcCloudProvisioningHelper;
    public OcCloudProvisioning        mOcCloudProvisioning;

    protected void setUp() throws Exception {
        super.setUp();
        CSConstants.mErrorMessage = CSConstants.EMPTY_STRING;
        mOcCloudProvisioningHelper = new OcCloudProvisioningHelper(this);
        mOcCloudProvisioning = new OcCloudProvisioning(
                OcCloudProvisioningHelper.DEFAULT_HOST_IP,
                OcCloudProvisioningHelper.DEFAULT_PORT);
        // create platform config
        mOcCloudProvisioningHelper
                .copyCborFromAsset(CSConstants.OIC_CLOUD_CLIENT);
        mOcCloudProvisioningHelper.configClientServerPlatform(CSConstants.OIC_CLOUD_CLIENT);

    }

    protected void tearDown() throws Exception {
        super.tearDown();
    }

    /**
     * @since 2016-10-20
     * @see none
     * @objective Call requestCertificate negatively with
     *            certificateIssueRequestListener as null
     * @target public native void requestCertificate(RequestCertificateListener
     *         certificateIssueRequestListener) throws OcException
     * @test_data certificateIssueRequestListener as null
     * @pre_condition none
     * @procedure Call requestCertificate() API
     * @post_condition none
     * @expected API should throw INVALID_CALLBACK error
     */
    public void testRequestCertificateCb_NV_N() {
        assertFalse(CSConstants.mErrorMessage, mOcCloudProvisioningHelper
                .requestCertificate(mOcCloudProvisioning, null));
        assertEquals(CSConstants.WRONG_ERROR_CODE,
                CSConstants.ERROR_INVALID_CALLBACK, CSConstants.mErrorMessage);
    }

    /**
     * @since 2016-10-20
     * @see none
     * @objective Call getIndividualAclInfo negatively with
     *            cloudAclIndividualGetInfoHandler as null
     * @target public native void getIndividualAclInfo(String aclId,
     *         GetIndividualAclInfoListener cloudAclIndividualGetInfoHandler)
     *         throws OcException
     * @test_data cloudAclIndividualGetInfoHandler as null
     * @pre_condition none
     * @procedure Call getIndividualAclInfo() API
     * @post_condition none
     * @expected API should throw INVALID_CALLBACK error
     */
    public void testGetIndividualAclInfoCb_NV_N() {
        assertFalse(CSConstants.mErrorMessage,
                mOcCloudProvisioningHelper.getIndividualAclInfo(
                        mOcCloudProvisioning,
                        OcCloudProvisioningHelper.DEFAULT_ACL_ID, null));
        assertEquals(CSConstants.WRONG_ERROR_CODE,
                CSConstants.ERROR_INVALID_CALLBACK, CSConstants.mErrorMessage);
    }

    /**
     * @since 2016-10-20
     * @see none
     * @objective Call getIndividualAclInfo negatively with aclId as null
     * @target public native void getIndividualAclInfo(String aclId,
     *         GetIndividualAclInfoListener cloudAclIndividualGetInfoHandler)
     *         throws OcException
     * @test_data aclId as null
     * @pre_condition none
     * @procedure Call getIndividualAclInfo() API
     * @post_condition none
     * @expected API should throw INVALID_PARAM error
     */
    public void testGetIndividualAclInfoAclId_NV_N() {
        assertFalse(CSConstants.mErrorMessage,
                mOcCloudProvisioningHelper.getIndividualAclInfo(
                        mOcCloudProvisioning, null,
                        mOcCloudProvisioningHelper));
        assertEquals(CSConstants.WRONG_ERROR_CODE,
                CSConstants.ERROR_INVALID_PARAM, CSConstants.mErrorMessage);
    }

    /**
     * @since 2016-10-20
     * @see none
     * @objective Call getAclIdByDevice negatively with
     *            cloudAclIdGetByDeviceHandler as null
     * @target public native void getAclIdByDevice(String deviceId,
     *         GetAclIdByDeviceListener cloudAclIdGetByDeviceHandler) throws
     *         OcException
     * @test_data cloudAclIdGetByDeviceHandler as null
     * @pre_condition none
     * @procedure Call getAclIdByDevice() API
     * @post_condition none
     * @expected API should throw INVALID_CALLBACK error
     */
    public void testGetAclIdByDeviceCb_NV_N() {
        assertFalse(CSConstants.mErrorMessage,
                mOcCloudProvisioningHelper.getAclIdByDevice(
                        mOcCloudProvisioning,
                        OcCloudProvisioningHelper.DEFAULT_DEVICE_ID, null));
        assertEquals(CSConstants.WRONG_ERROR_CODE,
                CSConstants.ERROR_INVALID_CALLBACK, CSConstants.mErrorMessage);
    }

    /**
     * @since 2016-10-20
     * @see none
     * @objective Call getAclIdByDevice negatively with deviceId as null
     * @target public native void getAclIdByDevice(String deviceId,
     *         GetAclIdByDeviceListener cloudAclIdGetByDeviceHandler) throws
     *         OcException
     * @test_data deviceId as null
     * @pre_condition none
     * @procedure Call getAclIdByDevice() API
     * @post_condition none
     * @expected API should throw INVALID_PARAM error
     */
    public void testGetAclIdByDeviceDev_NV_N() {
        assertFalse(CSConstants.mErrorMessage,
                mOcCloudProvisioningHelper.getAclIdByDevice(
                        mOcCloudProvisioning, null,
                        mOcCloudProvisioningHelper));
        assertEquals(CSConstants.WRONG_ERROR_CODE,
                CSConstants.ERROR_INVALID_PARAM, CSConstants.mErrorMessage);
    }

    /**
     * @since 2016-10-20
     * @see none
     * @objective Call getCRL negatively with cloudGetCRLHandler as null
     * @target public native void getCRL(GetCRLListener cloudGetCRLHandler)
     *         throws OcException
     * @test_data cloudGetCRLHandler as null
     * @pre_condition none
     * @procedure Call getCRL() API
     * @post_condition none
     * @expected API should throw INVALID_CALLBACK error
     */
    public void testGetCrlCb_NV_N() {
        assertFalse(CSConstants.mErrorMessage,
                mOcCloudProvisioningHelper.getCRL(mOcCloudProvisioning, null));
        assertEquals(CSConstants.WRONG_ERROR_CODE,
                CSConstants.ERROR_INVALID_CALLBACK, CSConstants.mErrorMessage);
    }

    /**
     * @since 2016-10-20
     * @see none
     * @objective Call postCRL negatively with cloudPostCRLHandler as null
     * @target public void postCRL(String thisUpdate, String nextUpdate, String
     *         crl, ArrayList<String> serialNumbers, PostCRLListener
     *         cloudPostCRLHandler) throws OcException
     * @test_data cloudPostCRLHandler as null
     * @pre_condition none
     * @procedure Call postCRL() API
     * @post_condition none
     * @expected API should throw INVALID_CALLBACK error
     */
    public void testPostCrlCb_NV_N() {
        ArrayList<String> serialNumbers = new ArrayList<String>();
        serialNumbers.add(CSConstants.CERT_SERIAL_ONE);
        assertFalse(CSConstants.mErrorMessage,
                mOcCloudProvisioningHelper.postCRL(mOcCloudProvisioning,
                        OcCloudProvisioningHelper.thisUpdate,
                        OcCloudProvisioningHelper.nextUpdate,
                        CSConstants.DEFAULT_CRL, serialNumbers, null));
        assertEquals(CSConstants.WRONG_ERROR_CODE,
                CSConstants.ERROR_INVALID_CALLBACK, CSConstants.mErrorMessage);
    }

    /**
     * @since 2016-10-20
     * @see none
     * @objective Call postCRL negatively with thisUpdate as null
     * @target public void postCRL(String thisUpdate, String nextUpdate, String
     *         crl, ArrayList<String> serialNumbers, PostCRLListener
     *         cloudPostCRLHandler) throws OcException
     * @test_data thisUpdate as null
     * @pre_condition none
     * @procedure Call postCRL() API
     * @post_condition none
     * @expected API should throw INVALID_PARAM error
     */
    public void testPostCrlThisUpdate_NV_N() {
        ArrayList<String> serialNumbers = new ArrayList<String>();
        serialNumbers.add(CSConstants.CERT_SERIAL_ONE);
        assertFalse(CSConstants.mErrorMessage,
                mOcCloudProvisioningHelper.postCRL(mOcCloudProvisioning, null,
                        OcCloudProvisioningHelper.nextUpdate,
                        CSConstants.DEFAULT_CRL, serialNumbers,
                        mOcCloudProvisioningHelper));
        assertEquals(CSConstants.WRONG_ERROR_CODE,
                CSConstants.ERROR_INVALID_PARAM, CSConstants.mErrorMessage);
    }

    /**
     * @since 2016-10-20
     * @see none
     * @objective Call postCRL negatively with nextUpdate as null
     * @target public void postCRL(String thisUpdate, String nextUpdate, String
     *         crl, ArrayList<String> serialNumbers, PostCRLListener
     *         cloudPostCRLHandler) throws OcException
     * @test_data nextUpdate as null
     * @pre_condition none
     * @procedure Call postCRL() API
     * @post_condition none
     * @expected API should throw INVALID_PARAM error
     */
    public void testPostCrlNextUpdate_NV_N() {
        ArrayList<String> serialNumbers = new ArrayList<String>();
        serialNumbers.add(CSConstants.CERT_SERIAL_ONE);
        assertFalse(CSConstants.mErrorMessage,
                mOcCloudProvisioningHelper.postCRL(mOcCloudProvisioning,
                        OcCloudProvisioningHelper.thisUpdate, null,
                        CSConstants.DEFAULT_CRL, serialNumbers,
                        mOcCloudProvisioningHelper));
        assertEquals(CSConstants.WRONG_ERROR_CODE,
                CSConstants.ERROR_INVALID_PARAM, CSConstants.mErrorMessage);
    }

}
