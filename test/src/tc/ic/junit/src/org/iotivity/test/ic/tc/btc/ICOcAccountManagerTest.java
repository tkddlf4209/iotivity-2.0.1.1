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
 ******************************************************************/

package org.iotivity.test.ic.tc.btc;

import java.util.EnumSet;
import java.util.HashMap;
import java.util.Map;

import org.iotivity.base.OcAccountManager;
import org.iotivity.base.OcPlatform;
import org.iotivity.base.OcConnectivityType;
import org.iotivity.base.OcException;
import org.iotivity.base.ErrorCode;
import org.iotivity.base.OcResource.*;
import org.iotivity.base.OcRepresentation;

import org.iotivity.test.ic.tc.helper.ICHelperStaticUtil;
import org.iotivity.test.ic.tc.helper.ICHelper;
import org.iotivity.test.ic.tc.helper.OcAccountManagerAdapter;
import org.iotivity.testcase.IoTivityLog;
import org.iotivity.testcase.IoTivityTc;

public class ICOcAccountManagerTest extends IoTivityTc {

    public OcAccountManager         m_accountManager;

    private OcAccountManagerAdapter m_OcAccountManagerAdapter;
    private ICHelper m_ICHelper;
    private OcRepresentation        m_propertyValue;

    protected void setUp() throws Exception {
        super.setUp();
        m_ICHelper = new ICHelper(this);
        m_ICHelper.configClientServerPlatform();

        m_OcAccountManagerAdapter = new OcAccountManagerAdapter();

        m_accountManager = OcPlatform.constructAccountManagerObject(
                ICHelperStaticUtil.IC_HOST_ADDRESS, EnumSet.of(OcConnectivityType.CT_DEFAULT));
        m_propertyValue = new OcRepresentation();
        m_propertyValue.setValue("property", "values");
    }

    protected void tearDown() throws Exception {
        super.tearDown();
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'signUp' function with valid authProvide, authCode and
     *            OnPostListener
     * @target void signUp(String authProvider, String authCode, OnPostListener
     *         onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform signUp(String authProvider, String authCode,
     *            OnPostListener onPostListener)API
     * @post_condition None
     * @expected signUp throws OcException if failure.
     */
    public void testSignUp_SRC_P() {
        try {
            m_accountManager.signUp(ICHelperStaticUtil.AUTH_PROVIDER, ICHelperStaticUtil.AUTH_CODE,
                    m_OcAccountManagerAdapter);
        } catch (OcException ex) {
            ex.printStackTrace();
            fail("SignUp API Exception occurred: " + ex.getLocalizedMessage());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'signUp' function with empty authProvide, authCode and
     *            valid OnPostListener
     * @target void signUp(String authProvider, String authCode, OnPostListener
     *         onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform signUp(String authProvider, String authCode,
     *            OnPostListener onPostListener)API
     * @post_condition None
     * @expected signUp throws OcException
     */
    public void testSignUp_ESV_N() {
        try {
            m_accountManager.signUp(ICHelperStaticUtil.EMPTY_VALUE, ICHelperStaticUtil.EMPTY_VALUE,
                    m_OcAccountManagerAdapter);
            fail("SignUp API does not throw exception occurred");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'signUp' function with invalid authProvide, authCode and
     *            valid OnPostListener
     * @target void signUp(String authProvider, String authCode, OnPostListener
     *         onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform signUp(String authProvider, String authCode,
     *            OnPostListener onPostListener)API
     * @post_condition None
     * @expected signUp throws OcException
     */
    public void testSignUp_USV_N() {
        try {
            m_accountManager.signUp(ICHelperStaticUtil.INVALID_PARAMETER, ICHelperStaticUtil.INVALID_PARAMETER,
                    m_OcAccountManagerAdapter);
            fail("SignUp API does not throw exception occurred");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'signUp' function with NULL authProvide, authCode and
     *            valid OnPostListener
     * @target void signUp(String authProvider, String authCode, OnPostListener
     *         onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform signUp(String authProvider, String authCode,
     *            OnPostListener onPostListener)API
     * @post_condition None
     * @expected signUp throws OcException
     */
    public void testSignUp_NV_N() {
        try {
            m_accountManager.signUp(ICHelperStaticUtil.NULL_VAL, ICHelperStaticUtil.NULL_VAL,
                    m_OcAccountManagerAdapter);
            fail("SignUp API does not throw exception occurred");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'signUp' function with authProvide, authCode and NULL
     *            OnPostListener
     * @target void signUp(String authProvider, String authCode, OnPostListener
     *         onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform signUp(String authProvider, String authCode,
     *            OnPostListener onPostListener)API
     * @post_condition None
     * @expected signUp throws OcException
     */
    public void testSignUpNULLCallback_NV_N() {
        try {
            m_accountManager.signUp(ICHelperStaticUtil.AUTH_PROVIDER, ICHelperStaticUtil.AUTH_CODE,
                    ICHelperStaticUtil.IC_OcAccountManager_OnPostListener_NULL);
            fail("SignUp API does not throw exception occurred");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'signUp' function with valid authProvide, authCode, Map
     *            Option and OnPostListener
     * @target void signUp(String authProvider, String authCode, Map<String,
     *         String> options, OnPostListener onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform signUp(String authProvider, String authCode,
     *            Map<String, String> options, OnPostListener onPostListener)API
     * @post_condition None
     * @expected signUp throws OcException if failure.
     */
    public void testSignUpWithMap_SRC_P() {
        try {
            m_accountManager.signUp(ICHelperStaticUtil.AUTH_PROVIDER, ICHelperStaticUtil.AUTH_CODE, ICHelperStaticUtil.MY_MAP,
                    m_OcAccountManagerAdapter);
        } catch (OcException ex) {
            ex.printStackTrace();
            fail("SignUp API Exception occurred: " + ex.getLocalizedMessage());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'signUp' function with empty authProvide, authCode, Map
     *            Option and valid OnPostListener
     * @target void signUp(String authProvider, String authCode, Map<String,
     *         String> options, OnPostListener onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform signUp(String authProvider, String authCode,
     *            Map<String, String> options, OnPostListener onPostListener)API
     * @post_condition None
     * @expected signUp throws OcException.
     */
    public void testSignUpWithMap_ESV_N() {
        try {
            m_accountManager.signUp(ICHelperStaticUtil.EMPTY_VALUE, ICHelperStaticUtil.EMPTY_VALUE, ICHelperStaticUtil.EMPTY_MY_MAP,
                    m_OcAccountManagerAdapter);
            fail("SignUp API does not throw exception occurred");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'signUp' function with invalid authProvide, authCode,
     *            valid Map Option and valid OnPostListener
     * @target void signUp(String authProvider, String authCode, Map<String,
     *         String> options, OnPostListener onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform signUp(String authProvider, String authCode,
     *            Map<String, String> options, OnPostListener onPostListener)API
     * @post_condition None
     * @expected signUp throws OcException.
     */
    public void testSignUpWithMap_USV_N() {
        try {
            m_accountManager.signUp(ICHelperStaticUtil.INVALID_PARAMETER, ICHelperStaticUtil.INVALID_PARAMETER,
                    ICHelperStaticUtil.MY_MAP, m_OcAccountManagerAdapter);
            fail("SignUp API does not throw exception occurred");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'signUp' function with empty authProvide, authCode, Map
     *            Option and valid OnPostListener
     * @target void signUp(String authProvider, String authCode, Map<String,
     *         String> options, OnPostListener onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform signUp(String authProvider, String authCode,
     *            Map<String, String> options, OnPostListener onPostListener)API
     * @post_condition None
     * @expected signUp throws OcException.
     */
    public void testSignUpWithMap_NV_N() {
        try {
            m_accountManager.signUp(ICHelperStaticUtil.INVALID_PARAMETER, ICHelperStaticUtil.INVALID_PARAMETER, ICHelperStaticUtil.EMPTY_MY_MAP,
                    m_OcAccountManagerAdapter);
            fail("SignUp API does not throw exception occurred");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'signUp' function with authProvide, authCode, Map Option
     *            and NULL OnPostListener
     * @target void signUp(String authProvider, String authCode, Map<String,
     *         String> options, OnPostListener onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform signUp(String authProvider, String authCode,
     *            Map<String, String> options, OnPostListener onPostListener)API
     * @post_condition None
     * @expected signUp throws OcException
     */
    public void testSignUpWithMapNULLCallback_NV_N() {
        try {
            m_accountManager.signUp(ICHelperStaticUtil.AUTH_PROVIDER, ICHelperStaticUtil.AUTH_CODE, ICHelperStaticUtil.MY_MAP,
                    ICHelperStaticUtil.IC_OcAccountManager_OnPostListener_NULL);
            fail("SignUp API does not throw exception occurred");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'signIn' function with valid userUuid, accessToken and
     *            OnPostListener
     * @target void signIn(String userUuid, String accessToken, OnPostListener
     *         onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform signIn(String userUuid, String accessToken,
     *            OnPostListener onPostListener) API
     * @post_condition None
     * @expected signIn throws OcException if failure.
     */
    public void testSignIn_SRC_P() {
        try {
            m_accountManager.signIn(ICHelperStaticUtil.USER_ID, ICHelperStaticUtil.ACCESS_TOKEN,
                    m_OcAccountManagerAdapter);
        } catch (OcException ex) {
            ex.printStackTrace();
            fail("SignUp API Exception occurred: " + ex.getLocalizedMessage());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'signIn' function with invalid userUuid, accessToken and
     *            valid OnPostListener
     * @target void signIn(String userUuid, String accessToken, OnPostListener
     *         onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform signIn(String userUuid, String accessToken,
     *            OnPostListener onPostListener) API
     * @post_condition None
     * @expected signIn throws OcException.
     */
    public void testSignIn_USV_N() {
        try {
            m_accountManager.signIn(ICHelperStaticUtil.INVALID_PARAMETER, ICHelperStaticUtil.INVALID_PARAMETER,
                    m_OcAccountManagerAdapter);
            fail("SignIn does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'signIn' function with empty userUuid, accessToken and
     *            valid OnPostListener
     * @target void signIn(String userUuid, String accessToken, OnPostListener
     *         onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform signIn(String userUuid, String accessToken,
     *            OnPostListener onPostListener) API
     * @post_condition None
     * @expected signIn throws OcException.
     */
    public void testSignIn_ESV_N() {
        try {
            m_accountManager.signIn(ICHelperStaticUtil.EMPTY_VALUE, ICHelperStaticUtil.EMPTY_VALUE,
                    m_OcAccountManagerAdapter);
            fail("SignIn does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'signIn' function with NULL userUuid, accessToken and
     *            valid OnPostListener
     * @target void signIn(String userUuid, String accessToken, OnPostListener
     *         onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform signIn(String userUuid, String accessToken,
     *            OnPostListener onPostListener) API
     * @post_condition None
     * @expected signIn throws OcException.
     */
    public void testSignIn_NV_N() {
        try {
            m_accountManager.signIn(ICHelperStaticUtil.NULL_VAL, ICHelperStaticUtil.NULL_VAL,
                    m_OcAccountManagerAdapter);
            fail("SignIn does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'signIn' function with valid userUuid, accessToken and
     *            NULL OnPostListener
     * @target void signIn(String userUuid, String accessToken, OnPostListener
     *         onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform signIn(String userUuid, String accessToken,
     *            OnPostListener onPostListener) API
     * @post_condition None
     * @expected signIn throws OcException.
     */
    public void testSignInNULLCallback_NV_N() {
        try {
            m_accountManager.signIn(ICHelperStaticUtil.USER_ID, ICHelperStaticUtil.ACCESS_TOKEN,
                    ICHelperStaticUtil.IC_OcAccountManager_OnPostListener_NULL);
            fail("SignIn does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'signOut' function with valid userUuid, accessToken and
     *            OnPostListener
     * @target void signOut(String accessToken, OnPostListener onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform signOut(String accessToken, OnPostListener
     *            onPostListener) API
     * @post_condition None
     * @expected signOut throws OcException if failure.
     */
    public void testsignOut_SRC_P() {
        try {
            m_accountManager.signOut(ICHelperStaticUtil.ACCESS_TOKEN, m_OcAccountManagerAdapter);
            fail("SignUp API should throw an Exception");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals("Wrong Exception is thrown by API", ErrorCode.ERROR, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'signOut' function with invalid userUuid, accessToken and
     *            valid OnPostListener
     * @target void signOut(String accessToken, OnPostListener onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform signOut(String accessToken, OnPostListener
     *            onPostListener) API
     * @post_condition None
     * @expected signOut throws OcException.
     */
    public void testsignOut_USV_N() {
        try {
            m_accountManager.signOut(ICHelperStaticUtil.INVALID_PARAMETER,
                    m_OcAccountManagerAdapter);
            fail("signOut does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.ERROR, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'signOut' function with empty userUuid, accessToken and
     *            valid OnPostListener
     * @target void signOut(String accessToken, OnPostListener onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform signOut(String accessToken, OnPostListener
     *            onPostListener) API
     * @post_condition None
     * @expected signOut throws OcException.
     */
    public void testsignOut_ESV_N() {
        try {
            m_accountManager.signOut(ICHelperStaticUtil.EMPTY_VALUE, m_OcAccountManagerAdapter);
            fail("signOut does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.ERROR, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'signOut' function with NULL userUuid, accessToken and
     *            valid OnPostListener
     * @target void signOut(String accessToken, OnPostListener onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform signOut(String accessToken, OnPostListener
     *            onPostListener) API
     * @post_condition None
     * @expected signOut throws OcException.
     */
    public void testsignOut_NV_N() {
        try {
            m_accountManager.signOut(ICHelperStaticUtil.NULL_VAL, m_OcAccountManagerAdapter);
            fail("signOut does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'signOut' function with valid userUuid, accessToken and
     *            NULL OnPostListener
     * @target void signOut(String accessToken, OnPostListener onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform signOut(String accessToken, OnPostListener
     *            onPostListener) API
     * @post_condition None
     * @expected signOut throws OcException.
     */
    public void testsignOutNULLCallback_NV_N() {
        try {
            m_accountManager.signOut(ICHelperStaticUtil.ACCESS_TOKEN,
                    ICHelperStaticUtil.IC_OcAccountManager_OnPostListener_NULL);
            fail("signOut does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'refreshAccessToken' function with valid userId,
     *            refreshToken and OnPostListener
     * @target void refreshAccessToken(userUuid, refreshToken, onPostListener)
     * @test_data OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform refreshAccessToken(userUuid, refreshToken,
     *            onPostListener) API
     * @post_condition None
     * @expected refreshAccessToken throws OcException if failure.
     */
    public void testRefreshAccessToken_SRC_P() {
        try {
            m_accountManager.refreshAccessToken(ICHelperStaticUtil.USER_ID, ICHelperStaticUtil.REFRESH_TOKEN,
                    m_OcAccountManagerAdapter);
        } catch (OcException ex) {
            ex.printStackTrace();
            fail("refreshAccessToken API Exception occurred: "
                    + ex.getLocalizedMessage());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'refreshAccessToken' function with invalid userId,
     *            refreshToken and valid OnPostListener
     * @target void refreshAccessToken(userUuid, refreshToken, onPostListener)
     * @test_data OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform refreshAccessToken(userUuid, refreshToken,
     *            onPostListener) API
     * @post_condition None
     * @expected refreshAccessToken throws OcException.
     */
    public void testRefreshAccessToken_USV_N() {
        try {
            m_accountManager.refreshAccessToken(ICHelperStaticUtil.INVALID_PARAMETER,
                    ICHelperStaticUtil.INVALID_PARAMETER, m_OcAccountManagerAdapter);
            fail("refreshAccessToken does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'refreshAccessToken' function with invalid userId,
     *            refreshToken and valid OnPostListener
     * @target void refreshAccessToken(userUuid, refreshToken, onPostListener)
     * @test_data OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform refreshAccessToken(userUuid, refreshToken,
     *            onPostListener) API
     * @post_condition None
     * @expected refreshAccessToken throws OcException.
     */
    public void testRefreshAccessToken_ESV_N() {
        try {
            m_accountManager.refreshAccessToken(ICHelperStaticUtil.EMPTY_VALUE, ICHelperStaticUtil.EMPTY_VALUE,
                    m_OcAccountManagerAdapter);
            fail("refreshAccessToken does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'refreshAccessToken' function with NULL userId,
     *            refreshToken and valid OnPostListener
     * @target void refreshAccessToken(userUuid, refreshToken, onPostListener)
     * @test_data OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform refreshAccessToken(userUuid, refreshToken,
     *            onPostListener) API
     * @post_condition None
     * @expected refreshAccessToken throws OcException.
     */
    public void testRefreshAccessToken_NV_N() {
        try {
            m_accountManager.refreshAccessToken(ICHelperStaticUtil.NULL_VAL, ICHelperStaticUtil.NULL_VAL,
                    m_OcAccountManagerAdapter);
            fail("refreshAccessToken does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'refreshAccessToken' function with valid userId,
     *            refreshToken and NULL OnPostListener
     * @target void refreshAccessToken(userUuid, refreshToken, onPostListener)
     * @test_data OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform refreshAccessToken(userUuid, refreshToken,
     *            onPostListener) API
     * @post_condition None
     * @expected refreshAccessToken throws OcException.
     */
    public void testRefreshAccessTokenNULLCallback_NV_N() {
        try {
            m_accountManager.refreshAccessToken(ICHelperStaticUtil.USER_ID, ICHelperStaticUtil.REFRESH_TOKEN,
                    ICHelperStaticUtil.IC_OcAccountManager_OnPostListener_NULL);
            fail("refreshAccessToken does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'searchUser' function with valid queryMap and
     *            OnGetListener
     * @target void searchUser(Map<String, String> queryMap, OnGetListener)
     * @test_data queryMap and OnGetListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform searchUser(Map<String, String> queryMap,
     *            OnGetListener) API
     * @post_condition None
     * @expected searchUser throws OcException if failure.
     */
    public void testSearchUserWithQueryMap_SRC_P() {
        try {
            m_accountManager.searchUser(ICHelperStaticUtil.MY_MAP, m_OcAccountManagerAdapter);
        } catch (OcException ex) {
            ex.printStackTrace();
            fail("SearchUser API Exception occurred: "
                    + ex.getLocalizedMessage());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'searchUser' function with invalid queryMap and valid
     *            OnGetListener
     * @target void searchUser(Map<String, String> queryMap, OnGetListener)
     * @test_data queryMap and OnGetListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform searchUser(Map<String, String> queryMap,
     *            OnGetListener) API
     * @post_condition None
     * @expected searchUser throws OcException.
     */
    public void testSearchUserWithQueryMap_ESV_N() {
        try {
            m_accountManager.searchUser(ICHelperStaticUtil.EMPTY_MY_MAP,
                    m_OcAccountManagerAdapter);
            fail("searchUser does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'searchUser' function with valid queryMap and NULL
     *            OnGetListener
     * @target void searchUser(Map<String, String> queryMap, OnGetListener)
     * @test_data queryMap and OnGetListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform searchUser(Map<String, String> queryMap,
     *            OnGetListener) API
     * @post_condition None
     * @expected searchUser throws OcException.
     */
    public void testSearchUserWithQueryMap_NV_N() {
        try {
            m_accountManager.searchUser(ICHelperStaticUtil.MY_MAP,
                    ICHelperStaticUtil.IC_OcAccountManager_OnGetListener_NULL);
            fail("searchUser does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'deleteDevice' function with valid deviceId and
     *            onDeleteListener
     * @target void deleteDevice(String deviceId, OnDeleteListener
     *         onDeleteListener)
     * @test_data deviceId and onDeleteListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform deleteDevice(String deviceId, OnDeleteListener
     *            onDeleteListener) API
     * @post_condition None
     * @expected searchUser throws OcException if failure.
     */
    public void testDeleteDevice_SRC_P() {
        try {
            m_accountManager.deleteDevice(ICHelperStaticUtil.ACCESS_TOKEN, ICHelperStaticUtil.DEVICE_ID,
                    m_OcAccountManagerAdapter);
        } catch (OcException ex) {
            ex.printStackTrace();
            fail("deleteDevice API Exception occurred: "
                    + ex.getLocalizedMessage());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'deleteDevice' function with invalid deviceId and valid
     *            onDeleteListener
     * @target void deleteDevice(String deviceId, OnDeleteListener
     *         onDeleteListener)
     * @test_data deviceId and onDeleteListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform deleteDevice(String deviceId, OnDeleteListener
     *            onDeleteListener) API
     * @post_condition None
     * @expected searchUser throws OcException.
     */
    public void testDeleteDevice_USV_N() {
        try {
            m_accountManager.deleteDevice(ICHelperStaticUtil.INVALID_PARAMETER, ICHelperStaticUtil.INVALID_PARAMETER,
                    m_OcAccountManagerAdapter);
            fail("deleteDevice does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'deleteDevice' function with empty deviceId and valid
     *            onDeleteListener
     * @target void deleteDevice(String deviceId, OnDeleteListener
     *         onDeleteListener)
     * @test_data deviceId and onDeleteListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform deleteDevice(String deviceId, OnDeleteListener
     *            onDeleteListener) API
     * @post_condition None
     * @expected searchUser throws OcException.
     */
    public void testDeleteDevice_ESV_N() {
        try {
            m_accountManager.deleteDevice(ICHelperStaticUtil.EMPTY_VALUE, ICHelperStaticUtil.EMPTY_VALUE,
                    m_OcAccountManagerAdapter);
            fail("deleteDevice does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'deleteDevice' function NULL valid deviceId and valid
     *            onDeleteListener
     * @target void deleteDevice(String deviceId, OnDeleteListener
     *         onDeleteListener)
     * @test_data deviceId and onDeleteListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform deleteDevice(String deviceId, OnDeleteListener
     *            onDeleteListener) API
     * @post_condition None
     * @expected searchUser throws OcException.
     */
    public void testDeleteDevice_NV_N() {
        try {
            m_accountManager.deleteDevice(ICHelperStaticUtil.NULL_VAL, ICHelperStaticUtil.NULL_VAL,
                    m_OcAccountManagerAdapter);
            fail("deleteDevice does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'deleteDevice' function with valid deviceId and NULL
     *            onDeleteListener
     * @target void deleteDevice(String deviceId, OnDeleteListener
     *         onDeleteListener)
     * @test_data deviceId and onDeleteListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform deleteDevice(String deviceId, OnDeleteListener
     *            onDeleteListener) API
     * @post_condition None
     * @expected searchUser throws OcException.
     */
    public void testDeleteDeviceNULLCallback_NV_N() {
        try {
            m_accountManager.deleteDevice(ICHelperStaticUtil.ACCESS_TOKEN, ICHelperStaticUtil.DEVICE_ID,
                    ICHelperStaticUtil.IC_OcAccountManager_OnDeleteListener_NULL);
            fail("deleteDevice does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'createGroup' function with valid deviceId and
     *            OnPostListener
     * @target void createGroup(OnPostListener onPostListener)
     * @test_data onPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform createGroup(OnPostListener onPostListener) API
     * @post_condition None
     * @expected searchUser throws OcException if failure.
     */
    public void testCreateGroup_SRC_P() {
        try {
            m_accountManager.createGroup(m_OcAccountManagerAdapter);
            fail("deleteDevice API Exception not occurred: ");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals("Error code not same",ErrorCode.ERROR, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'createGroup' function with valid deviceId and NULL
     *            OnPostListener
     * @target void createGroup(OnPostListener onPostListener)
     * @test_data onPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform createGroup(OnPostListener onPostListener) API with
     *            null listener
     * @post_condition None
     * @expected searchUser throws OcException.
     */
    public void testCreateGroup_NV_N() {
        try {
            m_accountManager
                    .createGroup(ICHelperStaticUtil.IC_OcAccountManager_OnPostListener_NULL);
            fail("createGroup does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2017-01-19
     * @see None
     * @objective Test 'createGroup' function with valid deviceId and
     *            OnPostListener
     * @target void createGroup(Map<String, String> queryMap,OnPostListener
     *         onPostListener)
     * @test_data queryMap, onPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform createGroup(Map<String, String>
     *            queryMap,OnPostListener onPostListener) API
     * @post_condition None
     * @expected searchUser throws OcException if failure.
     */
    public void testCreateGroupWithQueryMap_SRC_P() {
        try {
            m_accountManager.createGroup(ICHelperStaticUtil.MY_MAP, m_OcAccountManagerAdapter);
            fail("deleteDevice API Exception not occurred: ");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals("Error code not same",ErrorCode.ERROR, ex.getErrorCode());
        }
    }

    /**
     * @since 2017-01-19
     * @see None
     * @objective Test 'createGroup' function with valid deviceId and NULL
     *            OnPostListener
     * @target void createGroup(Map<String, String> queryMap,OnPostListener
     *         onPostListener)
     * @test_data queryMap, onPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform createGroup(Map<String, String>
     *            queryMap,OnPostListener onPostListener) API with null queryMap
     * @post_condition None
     * @expected searchUser throws OcException.
     */
    public void testCreateGroupWithQueryMap_NV_N() {
        try {
            m_accountManager.createGroup(ICHelperStaticUtil.EMPTY_MY_MAP,
                    ICHelperStaticUtil.IC_OcAccountManager_OnPostListener_NULL);
            fail("createGroup does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'deleteGroup' function with valid groupId and
     *            onGetListener
     * @target void deleteGroup(String groupId, OnDeleteListener
     *         onDeleteListener)
     * @test_data groupI and onDeleteListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform deleteGroup(String groupId, OnDeleteListener
     *            onDeleteListener) API
     * @post_condition None
     * @expected deleteGroup throws OcException if failure.
     */
    public void testDeleteGroup_SRC_P() {
        try {
            m_accountManager.deleteGroup(ICHelperStaticUtil.GROUP_ID, m_OcAccountManagerAdapter);
            fail("getGroupList API Exception not occurred:");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals("Error code not same",ErrorCode.ERROR, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'deleteGroup' function with invalid groupId and valid
     *            onDeleteListener
     * @target void deleteGroup(String groupId, OnDeleteListener
     *         onDeleteListener)
     * @test_data groupI and onDeleteListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform deleteGroup(String groupId, OnDeleteListener
     *            onDeleteListener) API
     * @post_condition None
     * @expected deleteGroup throws OcException.
     */
    public void testDeleteGroup_USV_N() {
        try {
            m_accountManager.deleteGroup(ICHelperStaticUtil.INVALID_PARAMETER,
                    m_OcAccountManagerAdapter);
            fail("deleteGroup does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.ERROR, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'deleteGroup' function with empty groupId and valid
     *            onDeleteListener
     * @target void deleteGroup(String groupId, OnDeleteListener
     *         onDeleteListener)
     * @test_data groupI and onDeleteListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform deleteGroup(String groupId, OnDeleteListener
     *            onDeleteListener) API
     * @post_condition None
     * @expected deleteGroup throws OcException.
     */
    public void testDeleteGroup_ESV_N() {
        try {
            m_accountManager.deleteGroup(ICHelperStaticUtil.EMPTY_VALUE,
                    m_OcAccountManagerAdapter);
            fail("deleteGroup does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.ERROR, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'deleteGroup' function with NULL groupId and valid
     *            onDeleteListener
     * @target void deleteGroup(String groupId, OnDeleteListener
     *         onDeleteListener)
     * @test_data groupI and onDeleteListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform deleteGroup(String groupId, OnDeleteListener
     *            onDeleteListener) API
     * @post_condition None
     * @expected deleteGroup throws OcException.
     */
    public void testDeleteGroup_NV_N() {
        try {
            m_accountManager.deleteGroup(ICHelperStaticUtil.NULL_VAL, m_OcAccountManagerAdapter);
            fail("deleteGroup does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'deleteGroup' function with groupId and NULL
     *            onDeleteListener
     * @target void deleteGroup(String groupId, OnDeleteListener
     *         onDeleteListener)
     * @test_data groupI and onDeleteListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform deleteGroup(String groupId, OnDeleteListener
     *            onDeleteListener) API
     * @post_condition None
     * @expected deleteGroup throws OcException.
     */
    public void testDeleteGroupNULLCallback_NV_N() {
        try {
            m_accountManager.deleteGroup(ICHelperStaticUtil.GROUP_ID,
                    ICHelperStaticUtil.IC_OcAccountManager_OnDeleteListener_NULL);
            fail("deleteGroup does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'getGroupInfo' function with groupId and onGetListener
     * @target void getGroupInfo(String groupId, OnGetListener onGetListener)
     * @test_data groupId and onGetListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform getGroupInfo(String groupId, OnGetListener
     *            onGetListener) API
     * @post_condition None
     * @expected getGroupInfo throws OcException if fail.
     */
    public void testGetGroupInfo_SRC_P() {
        try {
            m_accountManager.getGroupInfo(ICHelperStaticUtil.GROUP_ID, m_OcAccountManagerAdapter);
            fail("getGroupInfo API should throw an Exception ");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals("getGroupInfo API throws wrong Exception", ErrorCode.ERROR
                    , ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'getGroupInfo' function with invalid groupId and valid
     *            onGetListener
     * @target void getGroupInfo(String groupId, OnGetListener onGetListener)
     * @test_data groupId and onGetListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform getGroupInfo(String groupId, OnGetListener
     *            onGetListener) API
     * @post_condition None
     * @expected getGroupInfo throws OcException.
     */
    public void testGetGroupInfo_USV_N() {
        try {
            m_accountManager.getGroupInfo(ICHelperStaticUtil.INVALID_PARAMETER,
                    m_OcAccountManagerAdapter);
            fail("getGroupInfo does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.ERROR, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'getGroupInfo' function with empty groupId and valid
     *            onGetListener
     * @target void getGroupInfo(String groupId, OnGetListener onGetListener)
     * @test_data groupId and onGetListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform getGroupInfo(String groupId, OnGetListener
     *            onGetListener) API
     * @post_condition None
     * @expected getGroupInfo throws OcException.
     */
    public void testGetGroupInfo_ESV_N() {
        try {
            m_accountManager.getGroupInfo(ICHelperStaticUtil.EMPTY_VALUE,
                    m_OcAccountManagerAdapter);
            fail("getGroupInfo does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.ERROR, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'getGroupInfo' function with NULL groupId and valid
     *            onGetListener
     * @target void getGroupInfo(String groupId, OnGetListener onGetListener)
     * @test_data groupId and onGetListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform getGroupInfo(String groupId, OnGetListener
     *            onGetListener) API
     * @post_condition None
     * @expected getGroupInfo throws OcException.
     */
    public void testGetGroupInfo_NV_N() {
        try {
            m_accountManager.getGroupInfo(ICHelperStaticUtil.NULL_VAL, m_OcAccountManagerAdapter);
            fail("getGroupInfo does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'getGroupInfo' function with NULL groupId and valid
     *            onGetListener
     * @target void getGroupInfo(String groupId, OnGetListener onGetListener)
     * @test_data groupId and onGetListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform getGroupInfo(String groupId, OnGetListener
     *            onGetListener) API
     * @post_condition None
     * @expected getGroupInfo throws OcException.
     */
    public void testGetGroupInfoNULLCallback_NV_N() {
        try {
            m_accountManager.getGroupInfo(ICHelperStaticUtil.GROUP_ID,
                    ICHelperStaticUtil.IC_OcAccountManager_OnGetListener_NULL);
            fail("getGroupInfo does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2017-01-19
     * @see None
     * @objective Test 'getGroupInfo' function with onGetListener
     * @target void getGroupInfoAll(OnGetListener onGetListener)
     * @test_data onGetListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform getGroupInfo(OnGetListener onGetListener) API
     * @post_condition None
     * @expected getGroupInfo throws OcException if fail.
     */
    public void testGetGroupInfoWithListener_SRC_P() {
        try {
            m_accountManager.getGroupInfoAll(m_OcAccountManagerAdapter);
            fail("getGroupInfo API Exception not occurred: ");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals("Error code not same",ErrorCode.ERROR, ex.getErrorCode());
        }
    }

    /**
     * @since 2017-01-19
     * @see None
     * @objective Test 'getGroupInfo' function with onGetListener null
     * @target void getGroupInfoAll(OnGetListener onGetListener)
     * @test_data onGetListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform getGroupInfo(OnGetListener onGetListener) API where
     *            OnGetListener is null
     * @post_condition None
     * @expected getGroupInfo throws OcException if fail.
     */
    public void testGetGroupInfoWithListener_NV_N() {
        try {
            m_accountManager.getGroupInfoAll(null);
            fail("getGroupInfoAll does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2017-01-19
     * @see None
     * @objective Test 'getGroupInfo' function with onGetListener e
     * @target void getGroupInfoAll(OnGetListener onGetListener)
     * @test_data onGetListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform getGroupInfo(OnGetListener onGetListener) API where
     *            OnGetListener is null
     * @post_condition None
     * @expected getGroupInfo throws OcException if fail.
     */
    public void testGetGroupInfoWithListener_EMV_N() {
        try {
            OcAccountManagerAdapter ocAccountManagerAdapter = null;
            m_accountManager.getGroupInfoAll(ocAccountManagerAdapter);
            fail("getGroupInfoAll does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'observeGroup' function with groupId and
     *            onObserveListener
     * @target void observeGroup(String groupId, OnObserveListener
     *         onObserveListener)
     * @test_data groupId and onDeleteListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform observeGroup(String groupId, OnObserveListener
     *            onObserveListener) API
     * @post_condition None
     * @expected observeGroup throws OcException if fail.
     */
    public void testObserveGroup_SRC_P() {
        try {
            m_accountManager.observeGroup(m_OcAccountManagerAdapter);
            fail("observeGroup API Exception not occurred: ");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals("Error code not same",ErrorCode.ERROR, ex.getErrorCode());           
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'observeGroup' function with invalid groupId and valid
     *            onObserveListener
     * @target void observeGroup(String groupId, OnObserveListener
     *         onObserveListener)
     * @test_data groupId and onDeleteListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform observeGroup(String groupId, OnObserveListener
     *            onObserveListener) API
     * @post_condition None
     * @expected observeGroup throws OcException.
     */
    public void testObserveGroupNULLCallback_NV_N() {
        try {
            m_accountManager.observeGroup(null);
            fail("observeGroup does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'cancelObserveGroup' function with groupId
     * @target void cancelObserveGroup()
     * @test_data groupId
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform cancelObserveGroup() API
     * @post_condition None
     * @expected cancelObserveGroup throws OcException if fail.
     */
    public void testCancelObserveGroup_SRC_P() {
        try {
            m_accountManager.observeGroup(m_OcAccountManagerAdapter);
            m_accountManager.cancelObserveGroup();
            fail("SignUp API should throw an Exception");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals("Wrong Exception is thrown by API", ErrorCode.ERROR, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'cancelObserveGroup' function with invalid groupId
     * @target void cancelObserveGroup(String groupId)
     * @test_data groupId
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform cancelObserveGroup(String groupId) API
     * @post_condition None
     * @expected cancelObserveGroup throws OcException.
     */
    public void testCancelObserveGroup_USV_N() {
        try {
            m_accountManager.cancelObserveGroup();
            fail("cancelObserveGroup does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            System.out.println("####Error Message:" + ex.getMessage());
            System.out.println("####Error Code:" + ex.getErrorCode());
            System.out.println(
                    "####Error Localize Message:" + ex.getLocalizedMessage());
            assertEquals(ErrorCode.ERROR, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'observeInvitation' function with onObserveListener
     * @target void observeInvitation(OnObserveListener onObserveListener)
     * @test_data groupId
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform observeInvitation(OnObserveListener onObserveListener)
     *            API
     * @post_condition None
     * @expected observeInvitation throws OcException if fail.
     */
    public void testObserveInvitation_SRC_P() {
        try {
            m_accountManager.observeInvitation(m_OcAccountManagerAdapter);
        } catch (OcException ex) {
            ex.printStackTrace();
            fail("observeInvitation API Exception occurred: "
                    + ex.getLocalizedMessage());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'observeInvitation' function with NULL onObserveListener
     * @target void observeInvitation(OnObserveListener onObserveListener)
     * @test_data groupId
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform observeInvitation(OnObserveListener onObserveListener)
     *            API
     * @post_condition None
     * @expected observeInvitation throws OcException.
     */
    public void testObserveInvitation_NV_N() {
        try {
            m_accountManager.observeInvitation(null);
            fail("observeInvitation does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'cancelObserveInvitation' function poditively
     * @target void cancelObserveInvitation()
     * @test_data groupId
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform cancelObserveInvitation() API
     * @post_condition None
     * @expected cancelObserveInvitation throws OcException if fail.
     */
    public void testCancelObserveInvitation_SRC_P() {
        try {
            m_accountManager.observeInvitation(m_OcAccountManagerAdapter);
            m_accountManager.cancelObserveInvitation();
        } catch (OcException ex) {
            ex.printStackTrace();
            fail("cancelObserveInvitation API Exception occurred: "
                    + ex.getLocalizedMessage());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'sendInvitation' function with groupId, UserUuid and
     *            onPostListener
     * @target void sendInvitation(String groupId, String userUuid,
     *         OnPostListener onPostListener)
     * @test_data groupId, UserUuid and onPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform sendInvitation(String groupId, String userUuid,
     *            OnPostListener onPostListener) API
     * @post_condition None
     * @expected sendInvitation throws OcException if fail.
     */
    public void testSendInvitation_SRC_P() {
        try {
            m_accountManager.sendInvitation(ICHelperStaticUtil.GROUP_ID, ICHelperStaticUtil.USER_UUID,
                    m_OcAccountManagerAdapter);
            fail("SignUp API should throw an Exception");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals("Wrong Exception is thrown by API", ErrorCode.ERROR, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'sendInvitation' function with invalid groupId, UserUuid
     *            and valid onPostListener
     * @target void sendInvitation(String groupId, String userUuid,
     *         OnPostListener onPostListener)
     * @test_data groupId, UserUuid and onPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform sendInvitation(String groupId, String userUuid,
     *            OnPostListener onPostListener) API
     * @post_condition None
     * @expected sendInvitation throws OcException.
     */
    public void testSendInvitation_USV_N() {
        try {
            m_accountManager.sendInvitation(ICHelperStaticUtil.INVALID_PARAMETER,
                    ICHelperStaticUtil.INVALID_PARAMETER, m_OcAccountManagerAdapter);
            fail("observeInvitation does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            System.out.println("####Error Message:" + ex.getMessage());
            System.out.println("####Error Code:" + ex.getErrorCode());
            System.out.println(
                    "####Error Localize Message:" + ex.getLocalizedMessage());
            assertEquals(ErrorCode.ERROR, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'sendInvitation' function with empty groupId, UserUuid
     *            and valid onPostListener
     * @target void sendInvitation(String groupId, String userUuid,
     *         OnPostListener onPostListener)
     * @test_data groupId, UserUuid and onPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform sendInvitation(String groupId, String userUuid,
     *            OnPostListener onPostListener) API
     * @post_condition None
     * @expected sendInvitation throws OcException.
     */
    public void testSendInvitation_ESV_N() {
        try {
            m_accountManager.sendInvitation(ICHelperStaticUtil.EMPTY_VALUE, ICHelperStaticUtil.EMPTY_VALUE,
                    m_OcAccountManagerAdapter);
            fail("observeInvitation does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            System.out.println("####Error Message:" + ex.getMessage());
            System.out.println("####Error Code:" + ex.getErrorCode());
            System.out.println(
                    "####Error Localize Message:" + ex.getLocalizedMessage());
            assertEquals(ErrorCode.ERROR, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'sendInvitation' function with NULL groupId, UserUuid and
     *            valid onPostListener
     * @target void sendInvitation(String groupId, String userUuid,
     *         OnPostListener onPostListener)
     * @test_data groupId, UserUuid and onPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform sendInvitation(String groupId, String userUuid,
     *            OnPostListener onPostListener) API
     * @post_condition None
     * @expected sendInvitation throws OcException.
     */
    public void testSendInvitation_NV_N() {
        try {
            m_accountManager.sendInvitation(ICHelperStaticUtil.NULL_VAL, ICHelperStaticUtil.NULL_VAL,
                    m_OcAccountManagerAdapter);
            fail("observeInvitation does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'sendInvitation' function with NULL groupId, UserUuid and
     *            valid onPostListener
     * @target void sendInvitation(String groupId, String userUuid,
     *         OnPostListener onPostListener)
     * @test_data groupId, UserUuid and onPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform sendInvitation(String groupId, String userUuid,
     *            OnPostListener onPostListener) API
     * @post_condition None
     * @expected sendInvitation throws OcException.
     */
    public void testSendInvitationNULLCallback_NV_N() {
        try {
            m_accountManager.sendInvitation(ICHelperStaticUtil.GROUP_ID, ICHelperStaticUtil.USER_UUID,
                    ICHelperStaticUtil.IC_OcAccountManager_OnPostListener_NULL);
            fail("observeInvitation does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();

            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'cancelInvitation' function with groupId, UserUuid and
     *            onDeleteListener
     * @target void cancelInvitation(String groupId, String userUuid,
     *         OnDeleteListener onDeleteListener)
     * @test_data groupId, UserUuid and onDeleteListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform cancelInvitation(String groupId, String userUuid,
     *            OnDeleteListener onDeleteListener) API
     * @post_condition None
     * @expected cancelInvitation throws OcException if fail.
     */
    public void testCancelInvitation_SRC_P() {
        try {
            m_accountManager.cancelInvitation(ICHelperStaticUtil.GROUP_ID, ICHelperStaticUtil.USER_UUID,
                    m_OcAccountManagerAdapter);
        } catch (OcException ex) {
            ex.printStackTrace();
            fail("cancelInvitation API Exception occurred: "
                    + ex.getLocalizedMessage());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'cancelInvitation' function with invalid groupId,
     *            UserUuid and valid onDeleteListener
     * @target void cancelInvitation(String groupId, String userUuid,
     *         OnDeleteListener onDeleteListener)
     * @test_data groupId, UserUuid and onDeleteListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform cancelInvitation(String groupId, String userUuid,
     *            OnDeleteListener onDeleteListener) API
     * @post_condition None
     * @expected cancelInvitation throws OcException.
     */
    public void testCancelInvitation_USV_N() {
        try {
            m_accountManager.cancelInvitation(ICHelperStaticUtil.INVALID_PARAMETER,
                    ICHelperStaticUtil.INVALID_PARAMETER, m_OcAccountManagerAdapter);
            fail("cancelInvitation does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            System.out.println("####Error Message:" + ex.getMessage());
            System.out.println("####Error Code:" + ex.getErrorCode());
            System.out.println(
                    "####Error Localize Message:" + ex.getLocalizedMessage());
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'cancelInvitation' function with empty groupId, UserUuid
     *            and valid onDeleteListener
     * @target void cancelInvitation(String groupId, String userUuid,
     *         OnDeleteListener onDeleteListener)
     * @test_data groupId, UserUuid and onDeleteListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform cancelInvitation(String groupId, String userUuid,
     *            OnDeleteListener onDeleteListener) API
     * @post_condition None
     * @expected cancelInvitation throws OcException.
     */
    public void testCancelInvitation_ESV_N() {
        try {
            m_accountManager.cancelInvitation(ICHelperStaticUtil.EMPTY_VALUE, ICHelperStaticUtil.EMPTY_VALUE,
                    m_OcAccountManagerAdapter);
            fail("cancelInvitation does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            System.out.println("####Error Message:" + ex.getMessage());
            System.out.println("####Error Code:" + ex.getErrorCode());
            System.out.println(
                    "####Error Localize Message:" + ex.getLocalizedMessage());
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'cancelInvitation' function with NULL groupId, UserUuid
     *            and valid onDeleteListener
     * @target void cancelInvitation(String groupId, String userUuid,
     *         OnDeleteListener onDeleteListener)
     * @test_data groupId, UserUuid and onDeleteListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform cancelInvitation(String groupId, String userUuid,
     *            OnDeleteListener onDeleteListener) API
     * @post_condition None
     * @expected cancelInvitation throws OcException.
     */
    public void testCancelInvitation_NV_N() {
        try {
            m_accountManager.cancelInvitation(ICHelperStaticUtil.NULL_VAL, ICHelperStaticUtil.NULL_VAL,
                    m_OcAccountManagerAdapter);
            fail("cancelInvitation does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2016-08-29
     * @see None
     * @objective Test 'cancelInvitation' function with NULL groupId, UserUuid
     *            and valid onDeleteListener
     * @target void cancelInvitation(String groupId, String userUuid,
     *         OnDeleteListener onDeleteListener)
     * @test_data groupId, UserUuid and onDeleteListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform cancelInvitation(String groupId, String userUuid,
     *            OnDeleteListener onDeleteListener) API
     * @post_condition None
     * @expected cancelInvitation throws OcException.
     */
    public void testCancelInvitationNULLCallback_NV_N() {
        try {
            m_accountManager.cancelInvitation(ICHelperStaticUtil.GROUP_ID, ICHelperStaticUtil.USER_UUID,
                    null);
            fail("cancelInvitation does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2017-03-01
     * @see None
     * @objective Test 'addPropertyValueToGroup' function with valid userUuid,
     *            accessToken and OnPostListener
     * @target void addPropertyValueToGroup(String groupId, OcRepresentation
     *         propertyValue, OnPostListener onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform addPropertyValueToGroup(String groupId,
     *            OcRepresentation propertyValue, OnPostListener onPostListener)
     *            API
     * @post_condition None
     * @expected addPropertyValueToGroup throws OcException if failure.
     */
    public void testaddPropertyValueToGroup_SRC_P() {
        try {
            m_accountManager.addPropertyValueToGroup(ICHelperStaticUtil.GROUP_ID, m_propertyValue,
                    m_OcAccountManagerAdapter);
        } catch (OcException ex) {
            ex.printStackTrace();
            fail("SignUp API Exception occurred: " + ex.getLocalizedMessage());
        }
    }

    /**
     * @since 2017-03-01
     * @see None
     * @objective Test 'addPropertyValueToGroup' function with invalid userUuid,
     *            accessToken and valid OnPostListener
     * @target void addPropertyValueToGroup(String accessToken, OnPostListener
     *         onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform addPropertyValueToGroup(String groupId,
     *            OcRepresentation propertyValue, OnPostListener onPostListener)
     *            API
     * @post_condition None
     * @expected addPropertyValueToGroup throws OcException.
     */
    public void testaddPropertyValueToGroup_USV_N() {
        try {
            m_accountManager.addPropertyValueToGroup(ICHelperStaticUtil.INVALID_PARAMETER,
                    m_propertyValue, m_OcAccountManagerAdapter);
            fail("addPropertyValueToGroup does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2017-03-01
     * @see None
     * @objective Test 'addPropertyValueToGroup' function with empty userUuid,
     *            accessToken and valid OnPostListener
     * @target void addPropertyValueToGroup(String groupId, OcRepresentation
     *         propertyValue, OnPostListener onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform addPropertyValueToGroup(String groupId,
     *            OcRepresentation propertyValue, OnPostListener onPostListener)
     *            API
     * @post_condition None
     * @expected addPropertyValueToGroup throws OcException.
     */
    public void testaddPropertyValueToGroup_ESV_N() {
        try {
            m_accountManager.addPropertyValueToGroup(ICHelperStaticUtil.EMPTY_VALUE, m_propertyValue,
                    m_OcAccountManagerAdapter);
            fail("addPropertyValueToGroup does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2017-03-01
     * @see None
     * @objective Test 'addPropertyValueToGroup' function with NULL userUuid,
     *            accessToken and valid OnPostListener
     * @target void addPropertyValueToGroup(String groupId, OcRepresentation
     *         propertyValue, OnPostListener onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform addPropertyValueToGroup( String groupId,
     *            OcRepresentation propertyValue, OnPostListener onPostListener)
     *            API
     * @post_condition None
     * @expected addPropertyValueToGroup throws OcException.
     */
    public void testaddPropertyValueToGroup_NV_N() {
        try {
            m_accountManager.addPropertyValueToGroup(ICHelperStaticUtil.NULL_VAL, m_propertyValue,
                    m_OcAccountManagerAdapter);
            fail("addPropertyValueToGroup does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2017-03-01
     * @see None
     * @objective Test 'addPropertyValueToGroup' function with valid userUuid,
     *            accessToken and NULL OnPostListener
     * @target void addPropertyValueToGroup(String groupId, OcRepresentation
     *         propertyValue, OnPostListener onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform addPropertyValueToGroup(String groupId,
     *            OcRepresentation propertyValue, OnPostListener onPostListener)
     *            API
     * @post_condition None
     * @expected addPropertyValueToGroup throws OcException.
     */
    public void testaddPropertyValueToGroupNULLCallback_NV_N() {
        try {
            m_accountManager.addPropertyValueToGroup(ICHelperStaticUtil.GROUP_ID, m_propertyValue,
                    ICHelperStaticUtil.IC_OcAccountManager_OnPostListener_NULL);
            fail("addPropertyValueToGroup does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2017-03-01
     * @see None
     * @objective Test 'deletePropertyValueFromGroup' function with valid
     *            userUuid, accessToken and OnPostListener
     * @target void deletePropertyValueFromGroup(String groupId,
     *         OcRepresentation propertyValue, OnPostListener onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform deletePropertyValueFromGroup(String groupId,
     *            OcRepresentation propertyValue, OnPostListener onPostListener)
     *            API
     * @post_condition None
     * @expected deletePropertyValueFromGroup throws OcException if failure.
     */
    public void testdeletePropertyValueFromGroup_SRC_P() {
        try {
            m_accountManager.deletePropertyValueFromGroup(ICHelperStaticUtil.GROUP_ID,
                    m_propertyValue, m_OcAccountManagerAdapter);
        } catch (OcException ex) {
            ex.printStackTrace();
            fail("SignUp API Exception occurred: " + ex.getLocalizedMessage());
        }
    }

    /**
     * @since 2017-03-01
     * @see None
     * @objective Test 'deletePropertyValueFromGroup' function with invalid
     *            userUuid, accessToken and valid OnPostListener
     * @target void deletePropertyValueFromGroup(String groupId,
     *         OcRepresentation propertyValue, OnPostListener onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform deletePropertyValueFromGroup(String groupId,
     *            OcRepresentation propertyValue, OnPostListener onPostListener)
     *            API
     * @post_condition None
     * @expected deletePropertyValueFromGroup throws OcException.
     */
    public void testdeletePropertyValueFromGroup_USV_N() {
        try {
            m_accountManager.deletePropertyValueFromGroup(ICHelperStaticUtil.INVALID_PARAMETER,
                    m_propertyValue, m_OcAccountManagerAdapter);
            fail("deletePropertyValueFromGroup does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2017-03-01
     * @see None
     * @objective Test 'deletePropertyValueFromGroup' function with empty
     *            userUuid, accessToken and valid OnPostListener
     * @target void deletePropertyValueFromGroup(String groupId,
     *         OcRepresentation propertyValue, OnPostListener onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform deletePropertyValueFromGroup(String groupId,
     *            OcRepresentation propertyValue, OnPostListener onPostListener)
     *            API
     * @post_condition None
     * @expected deletePropertyValueFromGroup throws OcException.
     */
    public void testdeletePropertyValueFromGroup_ESV_N() {
        try {
            m_accountManager.deletePropertyValueFromGroup(ICHelperStaticUtil.NULL_VAL,
                    m_propertyValue, m_OcAccountManagerAdapter);
            fail("deletePropertyValueFromGroup does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2017-03-01
     * @see None
     * @objective Test 'deletePropertyValueFromGroup' function with NULL
     *            userUuid, accessToken and valid OnPostListener
     * @target void deletePropertyValueFromGroup(String groupId,
     *         OcRepresentation propertyValue, OnPostListener onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform deletePropertyValueFromGroup( String accessToken,
     *            OnPostListener onPostListener) API
     * @post_condition None
     * @expected deletePropertyValueFromGroup throws OcException.
     */
    public void testdeletePropertyValueFromGroup_NV_N() {
        try {
            m_accountManager.deletePropertyValueFromGroup(ICHelperStaticUtil.NULL_VAL,
                    m_propertyValue, m_OcAccountManagerAdapter);
            fail("deletePropertyValueFromGroup does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2017-03-01
     * @see None
     * @objective Test 'deletePropertyValueFromGroup' function with valid
     *            userUuid, accessToken and NULL OnPostListener
     * @target void deletePropertyValueFromGroup(String groupId,
     *         OcRepresentation propertyValue, OnPostListener onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform deletePropertyValueFromGroup(String groupId,
     *            OcRepresentation propertyValue, OnPostListener onPostListener)
     *            API
     * @post_condition None
     * @expected deletePropertyValueFromGroup throws OcException.
     */
    public void testdeletePropertyValueFromGroupNULLCallback_NV_N() {
        try {
            m_accountManager.deletePropertyValueFromGroup(ICHelperStaticUtil.GROUP_ID,
                    m_propertyValue, ICHelperStaticUtil.IC_OcAccountManager_OnPostListener_NULL);
            fail("deletePropertyValueFromGroup does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2017-03-01
     * @see None
     * @objective Test 'updatePropertyValueOnGroup' function with valid
     *            userUuid, accessToken and OnPostListener
     * @target void updatePropertyValueOnGroup(String groupId, OcRepresentation
     *         propertyValue, OnPostListener onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform updatePropertyValueOnGroup(String groupId,
     *            OcRepresentation propertyValue, OnPostListener onPostListener)
     *            API
     * @post_condition None
     * @expected updatePropertyValueOnGroup throws OcException if failure.
     */
    public void testupdatePropertyValueOnGroup_SRC_P() {
        try {
            m_accountManager.updatePropertyValueOnGroup(ICHelperStaticUtil.GROUP_ID, m_propertyValue,
                    m_OcAccountManagerAdapter);
        } catch (OcException ex) {
            ex.printStackTrace();
            fail("SignUp API Exception occurred: " + ex.getLocalizedMessage());
        }
    }

    /**
     * @since 2017-03-01
     * @see None
     * @objective Test 'updatePropertyValueOnGroup' function with invalid
     *            userUuid, accessToken and valid OnPostListener
     * @target void updatePropertyValueOnGroup(String groupId, OcRepresentation
     *         propertyValue, OnPostListener onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform updatePropertyValueOnGroup(String groupId,
     *            OcRepresentation propertyValue, OnPostListener onPostListener)
     *            API
     * @post_condition None
     * @expected updatePropertyValueOnGroup throws OcException.
     */
    public void testupdatePropertyValueOnGroup_USV_N() {
        try {
            m_accountManager.updatePropertyValueOnGroup(ICHelperStaticUtil.INVALID_PARAMETER,
                    m_propertyValue, m_OcAccountManagerAdapter);
            fail("updatePropertyValueOnGroup does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2017-03-01
     * @see None
     * @objective Test 'updatePropertyValueOnGroup' function with empty
     *            userUuid, accessToken and valid OnPostListener
     * @target void updatePropertyValueOnGroup(String groupId, OcRepresentation
     *         propertyValue, OnPostListener onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform updatePropertyValueOnGroup(String groupId,
     *            OcRepresentation propertyValue, OnPostListener onPostListener)
     *            API
     * @post_condition None
     * @expected signIn throws OcException.
     */
    public void testupdatePropertyValueOnGroup_ESV_N() {
        try {
            m_accountManager.updatePropertyValueOnGroup(ICHelperStaticUtil.EMPTY_VALUE,
                    m_propertyValue, m_OcAccountManagerAdapter);
            fail("updatePropertyValueOnGroup does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2017-03-01
     * @see None
     * @objective Test 'updatePropertyValueOnGroup' function with NULL userUuid,
     *            accessToken and valid OnPostListener
     * @target void updatePropertyValueOnGroup(String groupId, OcRepresentation
     *         propertyValue, OnPostListener onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform updatePropertyValueOnGroup( String accessToken,
     *            OnPostListener onPostListener) API
     * @post_condition None
     * @expected updatePropertyValueOnGroup throws OcException.
     */
    public void testupdatePropertyValueOnGroup_NV_N() {
        try {
            m_accountManager.updatePropertyValueOnGroup(ICHelperStaticUtil.NULL_VAL, m_propertyValue,
                    m_OcAccountManagerAdapter);
            fail("updatePropertyValueOnGroup does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2017-03-01
     * @see None
     * @objective Test 'updatePropertyValueOnGroup' function with valid
     *            userUuid, accessToken and NULL OnPostListener
     * @target void updatePropertyValueOnGroup(String groupId, OcRepresentation
     *         propertyValue, OnPostListener onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform updatePropertyValueOnGroup(String groupId,
     *            OcRepresentation propertyValue, OnPostListener onPostListener)
     *            API
     * @post_condition None
     * @expected updatePropertyValueOnGroup throws OcException.
     */
    public void testupdatePropertyValueOnGroupNULLCallback_NV_N() {
        try {
            m_accountManager.updatePropertyValueOnGroup(ICHelperStaticUtil.GROUP_ID, m_propertyValue,
                    ICHelperStaticUtil.IC_OcAccountManager_OnPostListener_NULL);
            fail("updatePropertyValueOnGroup does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2017-03-01
     * @see None
     * @objective Test 'replyToInvitation' function with valid userUuid,
     *            accessToken and OnPostListener
     * @target void replyToInvitation(String groupId, boolean accept,
     *         OnPostListener onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform replyToInvitation(String groupId, boolean accept,
     *            OnPostListener onPostListener) API
     * @post_condition None
     * @expected replyToInvitation throws OcException if failure.
     */
    public void testreplyToInvitation_SRC_P() {
        try {
            m_accountManager.replyToInvitation(ICHelperStaticUtil.GROUP_ID, true,
                    m_OcAccountManagerAdapter);
            m_accountManager.replyToInvitation(ICHelperStaticUtil.GROUP_ID, false,
                    m_OcAccountManagerAdapter);
        } catch (OcException ex) {
            ex.printStackTrace();
            fail("SignUp API Exception occurred: " + ex.getLocalizedMessage());
        }
    }

    /**
     * @since 2017-03-01
     * @see None
     * @objective Test 'replyToInvitation' function with invalid userUuid,
     *            accessToken and valid OnPostListener
     * @target void replyToInvitation(String groupId, boolean accept,
     *         OnPostListener onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform replyToInvitation(String groupId, boolean accept,
     *            OnPostListener onPostListener) API
     * @post_condition None
     * @expected replyToInvitation throws OcException.
     */
    public void testreplyToInvitation_USV_N() {
        try {
            m_accountManager.replyToInvitation(ICHelperStaticUtil.INVALID_PARAMETER, true,
                    m_OcAccountManagerAdapter);
            fail("replyToInvitation does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2017-03-01
     * @see None
     * @objective Test 'replyToInvitation' function with empty userUuid,
     *            accessToken and valid OnPostListener
     * @target void replyToInvitation(String groupId, boolean accept,
     *         OnPostListener onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform replyToInvitation(String groupId, boolean accept,
     *            OnPostListener onPostListener) API
     * @post_condition None
     * @expected signIn throws OcException.
     */
    public void testreplyToInvitation_ESV_N() {
        try {
            m_accountManager.replyToInvitation(ICHelperStaticUtil.EMPTY_VALUE, true,
                    m_OcAccountManagerAdapter);
            fail("replyToInvitation does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2017-03-01
     * @see None
     * @objective Test 'replyToInvitation' function with NULL userUuid,
     *            accessToken and valid OnPostListener
     * @target void replyToInvitation(String groupId, boolean accept,
     *         OnPostListener onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform replyToInvitation( String accessToken, OnPostListener
     *            onPostListener) API
     * @post_condition None
     * @expected replyToInvitation throws OcException.
     */
    public void testreplyToInvitation_NV_N() {
        try {
            m_accountManager.replyToInvitation(ICHelperStaticUtil.NULL_VAL, true,
                    m_OcAccountManagerAdapter);
            fail("replyToInvitation does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

    /**
     * @since 2017-03-01
     * @see None
     * @objective Test 'replyToInvitation' function with valid userUuid,
     *            accessToken and NULL OnPostListener
     * @target void replyToInvitation(String groupId, boolean accept,
     *         OnPostListener onPostListener)
     * @test_data authProvider authCode and OnPostListener
     * @pre_condition constructAccountManagerObject(host, connectivity_type) API
     * @procedure Perform replyToInvitation(String groupId, boolean accept,
     *            OnPostListener onPostListener) API
     * @post_condition None
     * @expected replyToInvitation throws OcException.
     */
    public void testreplyToInvitationNULLCallback_NV_N() {
        try {
            m_accountManager.replyToInvitation(ICHelperStaticUtil.GROUP_ID, true,
                    ICHelperStaticUtil.IC_OcAccountManager_OnDeleteListener_NULL);
            fail("replyToInvitation does not throw exception!");
        } catch (OcException ex) {
            ex.printStackTrace();
            assertEquals(ErrorCode.INVALID_PARAM, ex.getErrorCode());
        }
    }

}
