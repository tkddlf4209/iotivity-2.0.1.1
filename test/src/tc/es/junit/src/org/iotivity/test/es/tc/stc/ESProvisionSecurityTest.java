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

package org.iotivity.test.es.tc.stc;

import static org.iotivity.test.es.tc.helper.ESUtility.EXCEPTION_SHOULD_NOT_BE_THROWN;

import org.iotivity.base.OcResource;
import org.iotivity.service.easysetup.mediator.EasySetup;
import org.iotivity.service.easysetup.mediator.RemoteEnrollee;
import org.iotivity.test.es.tc.helper.ESEnrolleeHelper;
import org.iotivity.test.es.tc.helper.ESOICStackHelper;
import org.iotivity.test.es.tc.helper.ESSecurityProvisioningCallback;
import org.iotivity.test.es.tc.helper.ESUtility;

import android.test.AndroidTestCase;

public class ESProvisionSecurityTest extends AndroidTestCase {

    private ESEnrolleeHelper eSEnrolleeHelper;

    protected void setUp() throws Exception {
        super.setUp();
        eSEnrolleeHelper = new ESEnrolleeHelper();
        ESSecurityProvisioningCallback.initialCallbackValuse();
    }

    protected void tearDown() throws Exception {
        super.tearDown();
        eSEnrolleeHelper = null;
        ESSecurityProvisioningCallback.initialCallbackValuse();
    }

    /**
     * @throws ESException
     * @since 2016-08-14
     * @see public synchronized static EasySetup getInstance(Context context)
     * @see public synchronized RemoteEnrollee createRemoteEnrollee(OcResource
     *      enrolleeResource)
     * @objective if mediator can perform security provision
     * @target public void provisionSecurity(ESSecurityProvisioningCallback
     *         callback)
     * @test_data ESSecurityProvisioningCallback
     * @pre_condition 1.create remote enrollee
     * @procedure 1. Call provisionDeviceProperties API.
     * @post_condition none
     * @expected can perform security provision
     */
    public void testESProvisionSecurity_P_RSV() {
        assertTrue("Should build in secured mode",ESOICStackHelper.IS_SECURED);
        ESOICStackHelper.copyJsonFromAsset(getContext());
        ESOICStackHelper.initOICStack(getContext());
        OcResource remoteEnrolleeResource = eSEnrolleeHelper
                .findEnrolleeResource(getContext());
        assertNotNull("Got null remote enrollee resource",
                remoteEnrolleeResource);
        RemoteEnrollee remoteEnrollee = EasySetup.getInstance(getContext())
                .createRemoteEnrollee(remoteEnrolleeResource);
        assertNotNull("Got null remote enrollee", remoteEnrollee);
        try {
            remoteEnrollee
                    .provisionSecurity(new ESSecurityProvisioningCallback());
            ESUtility.waitUntilEnrolleeResponse(
                    ESSecurityProvisioningCallback.isSecurityProvCallbackCalled);

        } catch (Exception ex) {
            fail(EXCEPTION_SHOULD_NOT_BE_THROWN);
        }
        assertTrue("SecurityProvisioningCallback not called",
                ESSecurityProvisioningCallback.isSecurityProvCallbackCalled);
        assertTrue("Security provisioning fail : ",
                ESSecurityProvisioningCallback.isSecurityProvPropSuccess);
    }
}