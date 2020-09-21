/*
 * //******************************************************************
 * //
 * // Copyright 2016 Samsung Electronics All Rights Reserved.
 * //
 * //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 * //
 * // Licensed under the Apache License, Version 2.0 (the "License");
 * // you may not use this file except in compliance with the License.
 * // You may obtain a copy of the License at
 * //
 * //      http://www.apache.org/licenses/LICENSE-2.0
 * //
 * // Unless required by applicable law or agreed to in writing, software
 * // distributed under the License is distributed on an "AS IS" BASIS,
 * // WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * // See the License for the specific language governing permissions and
 * // limitations under the License.
 * //
 * //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */
package org.iotivity.cloud.accountserver.resources.account.tokenrefresh;

import org.iotivity.cloud.accountserver.Constants;

import java.util.Arrays;

/**
 *
 * This class provides a set of APIs to handle token refresh requests. Version 2.0.0
 *
 */

public class SecTokenRefreshResource extends TokenRefreshResource {

    public SecTokenRefreshResource() {
        super(Arrays.asList(Constants.PREFIX_OIC, Constants.SEC_URI,
                Constants.TOKEN_REFRESH_URI));
    }
}
