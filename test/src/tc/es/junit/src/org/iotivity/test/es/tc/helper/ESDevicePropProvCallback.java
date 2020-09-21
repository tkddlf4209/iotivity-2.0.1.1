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

package org.iotivity.test.es.tc.helper;

import org.iotivity.service.easysetup.mediator.DevicePropProvisioningCallback;
import org.iotivity.service.easysetup.mediator.DevicePropProvisioningStatus;
import org.iotivity.service.easysetup.mediator.enums.ESResult;

import android.util.Log;

public class ESDevicePropProvCallback extends DevicePropProvisioningCallback {

    private static final String TAG = "DevicePropProvisioningStatus";
    public static boolean       isProvisionDevicePropSuccess;
    public static boolean       isDevicePropProvCallbackCalled;
    public static ESResult      eSResult;

    @Override
    public void onProgress(
            DevicePropProvisioningStatus devPropProvisioningStatus) {
        final ESResult result = devPropProvisioningStatus.getESResult();

        if (result.equals(ESResult.ES_OK)) {
            isProvisionDevicePropSuccess = true;
            eSResult = ESResult.ES_OK;
            Log.d(TAG, eSResult.toString());
        }
        else if (result.equals(ESResult.ES_ERROR)) {
            eSResult = ESResult.ES_ERROR;
            Log.d(TAG, eSResult.toString());
        } else if (result.equals(ESResult.ES_COMMUNICATION_ERROR)) {
            eSResult = ESResult.ES_COMMUNICATION_ERROR;
            Log.d(TAG, eSResult.toString());
        }
        isDevicePropProvCallbackCalled = true;
    }

    public static void initialCallbackValuse() {
        isDevicePropProvCallbackCalled = false;
        isProvisionDevicePropSuccess = false;
        eSResult = null;
        ESGetConfigurationCallback.isConfigurationCallbackCalled = false;
        ESGetConfigurationCallback.isConfigurationCallbackSuccess = false;
    }
}
