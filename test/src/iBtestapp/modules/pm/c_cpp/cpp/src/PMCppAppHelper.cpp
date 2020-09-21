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
 *      LICENSE-2.0" target="_blank">http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 ******************************************************************/
#include "PMCppAppHelper.h"

int g_cbInvoked = CALLBACK_NOT_INVOKED;

static const OicSecPrm_t SUPPORTED_PRMS[1] =
{ PRM_PRE_CONFIGURED, };

void printDevices(DeviceList_t &list)
{
    IOTIVITYTEST_LOG(DEBUG, "[PMHelper] Total Devices %d", list.size());

    for (unsigned int i = 0; i < list.size(); i++)
    {
        std::cout << "Device " << i + 1 << " ID: ";
        std::cout << list[i]->getDeviceID() << " From IP: ";
        std::cout << list[i]->getDevAddr() << " Device Status(On/ Off) ";
        std::cout << list[i]->getDeviceStatus() << " Owned Status: ";
        std::cout << list[i]->getOwnedStatus() << std::endl;
    }
}

void printUuid(OicUuid_t uuid)
{
    for (int i = 0; i < UUID_LENGTH; i++)
    {
        std::cout << std::hex << uuid.id[i] << " ";
    }
    std::cout << std::endl;
}

FILE* clientCppAppOpen(const char *UNUSED_PARAM, const char *mode)
{
    //(void) UNUSED_PARAM;
    if (0 == strcmp(UNUSED_PARAM, OC_SECURITY_DB_DAT_FILE_NAME))
    {
        return fopen(CLIENT_CBOR, mode);
    }
    else
    {
        return fopen(UNUSED_PARAM, mode);
    }
}

FILE* clientCppMOTAppOpen(const char *UNUSED_PARAM, const char *mode)
{
    //(void) UNUSED_PARAM;
    if (0 == strcmp(UNUSED_PARAM, OC_SECURITY_DB_DAT_FILE_NAME))
    {
        return fopen(CLIENT_MOT_CBOR, mode);
    }
    else
    {
        return fopen(UNUSED_PARAM, mode);
    }
}

void InputPinCB(char* pin, size_t len)
{

    if (!pin || OXM_RANDOM_PIN_MIN_SIZE >= len)
    {
        IOTIVITYTEST_LOG(ERROR, "inputPinCB invalid parameters");
        return;
    }

    printf("   > INPUT PIN: ");
    for (int ret = 0; 1 != ret;)
    {
        ret = scanf("%8s", pin);
        for (; 0x20 <= getchar();)
            ; // for removing overflow garbages
              // '0x20<=code' is character region
    }

}

void OnInputPinCB(OicUuid_t deviceId, char* pinBuf, size_t bufSize)
{
    if (pinBuf)
    {
        std::cout << "INPUT PIN : ";
        std::string ptr;
        std::cin >> ptr;
        strcpy(pinBuf, (const char*) ptr.c_str());
        return;
    }
}

OCStackResult displayMutualVerifNumCB(uint8_t mutualVerifNum[MUTUAL_VERIF_NUM_LEN])
{
    IOTIVITYTEST_LOG(DEBUG, "[PMHelper] displayNumCB IN");
    IOTIVITYTEST_LOG(DEBUG, "[PMHelper] ############ mutualVerifNum ############");

    for (int i = 0; i < MUTUAL_VERIF_NUM_LEN; i++)
    {
        IOTIVITYTEST_LOG(DEBUG, "[Test Server] %02X ", mutualVerifNum[i]);
    }

    IOTIVITYTEST_LOG(DEBUG, "[PMHelper] ############ mutualVerifNum ############");
    IOTIVITYTEST_LOG(DEBUG, "[PMHelper] displayNumCB OUT");
    return OC_STACK_OK;
}

OCStackResult confirmMutualVerifNumCB(void)
{
    IOTIVITYTEST_LOG(DEBUG, "[PMHelper] confirmMutualVerifNumCB IN");
    for (;;)
    {
        int clientConfirm;
        CommonUtil::waitInSecond(DELAY_SHORT);
        printf("   > Press 1 if the mutual verification numbers are the same\n");
        printf("   > Press 0 if the mutual verification numbers are not the same\n");

        for (int ret = 0; 1 != ret;)
        {
            ret = scanf("%d", &clientConfirm);
            for (; 0x20 <= getchar();)
                ; // for removing overflow garbage
                  // '0x20<=code' is character region
        }

        if (1 == clientConfirm)
        {
            break;
        }
        else if (0 == clientConfirm)
        {
            return OC_STACK_USER_DENIED_REQ;
        }
        printf("   Entered Wrong Number. Please Enter Again\n");
    }
    IOTIVITYTEST_LOG(DEBUG, "[PMHelper] confirmMutualVerifNumCB OUT");
    return OC_STACK_OK;
}

OicSecAcl_t* createAcl(const int dev_num, int nPermission, DeviceList_t &m_OwnedDevList)
{
    IOTIVITYTEST_LOG(DEBUG, "[PMHelper] createAcl IN");

    OicSecAcl_t* acl = (OicSecAcl_t*) OICCalloc(1, sizeof(OicSecAcl_t));
    OicSecAce_t* ace = (OicSecAce_t*) OICCalloc(1, sizeof(OicSecAce_t));

    LL_APPEND(acl->aces, ace);
    int num = 0;

    if (dev_num == 1)
    {
        memcpy(&ace->subjectuuid, ACL_ROWNER_UUID_02, UUID_LENGTH);
    }
    else
    {
        memcpy(&ace->subjectuuid, ACL_ROWNER_UUID_01, UUID_LENGTH);
    }

    num = 1;

    for (int i = 0; num > i; ++i)
    {
        OicSecRsrc_t* rsrc = (OicSecRsrc_t*) OICCalloc(1, sizeof(OicSecRsrc_t));

        // Resource URI
        size_t len = strlen(LIGHT_RESOURCE_URI_01) + 1; // '1' for null termination
        rsrc->href = (char*) OICCalloc(len, sizeof(char));
        OICStrcpy(rsrc->href, len, LIGHT_RESOURCE_URI_01);

        // Resource Type
        rsrc->typeLen = 1;
        rsrc->types = (char**) OICCalloc(rsrc->typeLen, sizeof(char*));
        for (size_t i = 0; i < rsrc->typeLen; i++)
        {
            rsrc->types[i] = OICStrdup(LIGHT_RESOURCE_URI_01);
        }

        rsrc->interfaceLen = 1;
        rsrc->interfaces = (char**) OICCalloc(rsrc->typeLen, sizeof(char*));
        for (size_t i = 0; i < rsrc->interfaceLen; i++)
        {
            rsrc->interfaces[i] = OICStrdup(LIGHT_RESOURCE_URI_01);
        }

        LL_APPEND(ace->resources, rsrc);
    }

    ace->permission = nPermission;

    IOTIVITYTEST_LOG(DEBUG, "[PMHelper] createAcl OUT");
    return acl;
}

bool provisionInit()
{
    CommonUtil::killApp(KILL_SERVERS);
    CommonUtil::waitInSecond(DELAY_MEDIUM);
    //removeAllResFile();
    CommonUtil::waitInSecond(DELAY_MEDIUM);
    CommonUtil::copyFile(JUSTWORKS_SERVER1_CBOR_BACKUP, JUSTWORKS_SERVER1_CBOR);
    CommonUtil::copyFile(JUSTWORKS_SERVER2_CBOR_BACKUP, JUSTWORKS_SERVER2_CBOR);
    CommonUtil::copyFile(PRECONFIG_SERVER1_CBOR_BACKUP, PRECONFIG_SERVER1_CBOR);
    CommonUtil::copyFile(RANDOMPIN_SERVER_CBOR_BACKUP, RANDOMPIN_SERVER_CBOR);
    CommonUtil::copyFile(JUSTWORKS_SERVER7_CBOR_BACKUP, JUSTWORKS_SERVER7_CBOR);
    CommonUtil::copyFile(PRECONFIG_SERVER2_CBOR_BACKUP, PRECONFIG_SERVER2_CBOR);
    CommonUtil::copyFile(MOT_CLIENT_CBOR_BACKUP, MOT_CLIENT_CBOR);
    CommonUtil::copyFile(CLIENT_CBOR_BACKUP, CLIENT_CBOR);

    OCPersistentStorage ps
    { clientCppAppOpen, fread, fwrite, fclose, unlink };

    PlatformConfig cfg
    { OC::ServiceType::InProc, OC::ModeType::Both, "0.0.0.0", 0, OC::QualityOfService::LowQos, &ps };

    OCPlatform::Configure(cfg);

    if (OCSecure::provisionInit(EMPTY_STRING) != OC_STACK_OK)
    {
        printf("Provision Initialization failed");
        return false;
    }

    IOTIVITYTEST_LOG(DEBUG, "Provision Initialization Successful\n");
    return true;
}

bool discoverUnownedDevices(int time, DeviceList_t& deviceList, OCStackResult expectedResult)
{
    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] discoverUnownedDevices IN\n");

    OCStackResult res = OC_STACK_OK;
    res = OCSecure::discoverUnownedDevices(time, deviceList);
    IOTIVITYTEST_LOG(DEBUG, "[API Return Code] discoverUnownedDevices returns : %s\n",
            getOCStackResultCPP(res).c_str());

    if (res != expectedResult)
    {
        IOTIVITYTEST_LOG(ERROR, "Expected Result Mismatch");
        return false;
    }

    if (deviceList.size())
    {
        IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] Found secure devices, count = %d\n",
                deviceList.size());
        printDevices(deviceList);

    }

    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] discoverUnownedDevices OUT\n");
    return true;
}

bool discoverSingleDevice(unsigned short timeout, const OicUuid_t* deviceID,
        std::shared_ptr< OCSecureResource > &foundDevice, OCStackResult expectedResult)
{
    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] discoverSingleDevice IN");

    OCStackResult res = OC_STACK_OK;
    res = OCSecure::discoverSingleDevice(timeout, deviceID, foundDevice);
    IOTIVITYTEST_LOG(INFO, "[API Return Code] discoverSingleDevice returns : %s",
            getOCStackResultCPP(res).c_str());

    if (res != expectedResult)
    {
        IOTIVITYTEST_LOG(ERROR, "Expected Result Mismatch");
        return false;
    }

    if (foundDevice != NULL)
    {
        std::cout << "[PMCppHelper] Found secure devices: " << foundDevice->getDeviceID() << endl;
    }

    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] discoverSingleDevice OUT");
    return true;
}

bool discoverOwnedDevices(int time, DeviceList_t& deviceList, OCStackResult expectedResult)
{
    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] discoverOwnedDevices IN");

    OCStackResult res = OC_STACK_OK;
    res = OCSecure::discoverOwnedDevices(time, deviceList);
    IOTIVITYTEST_LOG(DEBUG, "[API Return Code] discoverOwnedDevices returns : %s\n",
            getOCStackResultCPP(res).c_str());

    if (res != expectedResult)
    {
        IOTIVITYTEST_LOG(ERROR, "Expected Result Mismatch");
        return false;
    }

    if (deviceList.size())
    {
        IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] Found secure devices, count = %d\n",
                deviceList.size());
        printDevices(deviceList);
    }

    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] discoverOwnedDevices OUT\n");
    return true;
}

bool getDevInfoFromNetwork(unsigned short time, DeviceList_t& ownedDevList,
        DeviceList_t &unownedDevList, OCStackResult expectedResult)
{
    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] getDevInfoFromNetwork IN");

    OCStackResult res = OC_STACK_OK;
    res = OCSecure::getDevInfoFromNetwork(time, ownedDevList, unownedDevList);
    IOTIVITYTEST_LOG(DEBUG, "[API Return Code] getDevInfoFromNetwork returns : %s",
            getOCStackResultCPP(res).c_str());

    if (res != expectedResult)
    {
        IOTIVITYTEST_LOG(ERROR, "Expected Result Mismatch");
        return false;
    }

    if (unownedDevList.size())
    {
        IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] Found secure devices, count = %d\n",
                unownedDevList.size());
        printDevices(unownedDevList);
    }

    if (ownedDevList.size())
    {
        IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] Found secure devices, count = %d\n",
                ownedDevList.size());
        printDevices(ownedDevList);
    }

    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] getDevInfoFromNetwork OUT");
    return true;
}

bool setOwnerTransferCallbackData(int num, OTMCallbackData_t &data, InputPinCallback inputPin,
        OCStackResult expectedResult)
{
    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] setOwnerTransferCallbackData IN");
#ifdef __1.2.1__
    OCStackResult res = OC_STACK_OK;

    res = OCSecure::setOwnerTransferCallbackData((OicSecOxm_t) num, &data, inputPin);
    IOTIVITYTEST_LOG(DEBUG, "[API Return Code] setOwnerTransferCallbackData returns : %s\n",
            getOCStackResultCPP(res).c_str());

    if (res != expectedResult)
    {
        IOTIVITYTEST_LOG(ERROR, "Expected Result Mismatch");
        return false;
    }
#endif
    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] setOwnerTransferCallbackData OUT");
    return true;
}

bool doOwnershipTransfer(DeviceList_t &data, ResultCallBack resultCallback,
        OCStackResult expectedResult)
{
    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] ownershipTransfer IN\n");
    OCStackResult res = OC_STACK_OK;
    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] Device to be owned in : %d \n", data.size());

    for (size_t i = 0; i < data.size(); i++)
    {
        g_cbInvoked = CALLBACK_NOT_INVOKED;

        if (data[i]->getDevAddr().find("wlan0"))
        {
            continue;
        }

        res = data[i]->doOwnershipTransfer(resultCallback);

        IOTIVITYTEST_LOG(DEBUG, "[API Return Code] doOwnershipTransfer returns : %s\n",
                getOCStackResultCPP(res).c_str());

        if (res != expectedResult)
        {
            IOTIVITYTEST_LOG(ERROR, "Expected Result Mismatch");
            return false;
        }

        if (OC_STACK_OK == res)
        {
            if (CALLBACK_NOT_INVOKED == waitCallbackRet())
            {
                IOTIVITYTEST_LOG(ERROR, "Callback Not Invoked");
                return false;
            }
        }
    }

    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] ownershipTransfer OUT\n");
    return true;
}

bool provisionACL(DeviceList_t& deviceList, const OicSecAcl_t* acl, ResultCallBack resultCallback,
        OCStackResult expectedResult)
{
    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] provisionACL IN\n");
    OCStackResult res = OC_STACK_OK;
    g_cbInvoked = CALLBACK_NOT_INVOKED;

    res = deviceList[0]->provisionACL(acl, resultCallback);
    IOTIVITYTEST_LOG(DEBUG, "[API Return Code] provisionACL returns : %s\n",
            getOCStackResultCPP(res).c_str());

    if (res != expectedResult)
    {
        IOTIVITYTEST_LOG(ERROR, "Expected Result Mismatch");
        return false;
    }

    if (OC_STACK_OK == res)
    {
        if (CALLBACK_NOT_INVOKED == waitCallbackRet())
        {
            IOTIVITYTEST_LOG(ERROR, "Callback Not Invoked");
            return false;
        }
    }

    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] provisionACL OUT\n");
    return true;
}

bool saveACL(const OicSecAcl_t* acl, OCStackResult expectedResult)
{
    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] saveACL IN");
    OCStackResult res = OC_STACK_OK;
    g_cbInvoked = CALLBACK_NOT_INVOKED;

    res = OCSecure::saveACL(acl);
    IOTIVITYTEST_LOG(DEBUG, "[API Return Code] saveACL returns : %s",
            getOCStackResultCPP(res).c_str());

    if (res != expectedResult)
    {
        IOTIVITYTEST_LOG(ERROR, "Expected Result Mismatch");
        return false;
    }

    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] saveACL OUT");
    return true;
}

bool provisionCredentials(DeviceList_t& deviceList, const Credential &cred,
        const OCSecureResource &device2, ResultCallBack resultCallback,
        OCStackResult expectedResult)
{
    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] provisionCredentials IN\n");
    OCStackResult res = OC_STACK_OK;
    g_cbInvoked = CALLBACK_NOT_INVOKED;

    res = deviceList[0]->provisionCredentials(cred, device2, resultCallback);
    IOTIVITYTEST_LOG(DEBUG, "[API Return Code] provisionCredentials returns : %s\n",
            getOCStackResultCPP(res).c_str());

    if (res != expectedResult)
    {
        IOTIVITYTEST_LOG(ERROR, "Expected Result Mismatch");
        return false;
    }

    if (OC_STACK_OK == res)
    {
        if (CALLBACK_NOT_INVOKED == waitCallbackRet())
        {
            IOTIVITYTEST_LOG(ERROR, "Callback Not Invoked");
            return false;
        }
    }

    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] provisionCredentials OUT\n");
    return true;
}

bool provisionPairwiseDevices(DeviceList_t& deviceList, const Credential &cred,
        const OicSecAcl_t* acl1, const OCSecureResource &device2, const OicSecAcl_t* acl2,
        ResultCallBack resultCallback, OCStackResult expectedResult)
{
    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] provisionPairwiseDevices IN\n");
    OCStackResult res = OC_STACK_OK;
    g_cbInvoked = CALLBACK_NOT_INVOKED;

    res = deviceList[0]->provisionPairwiseDevices(cred, acl1, device2, acl2, resultCallback);
    IOTIVITYTEST_LOG(DEBUG, "[API Return Code] provisionPairwiseDevices returns : %s\n",
            getOCStackResultCPP(res).c_str());

    if (res != expectedResult)
    {
        IOTIVITYTEST_LOG(ERROR, "Expected Result Mismatch");
        return false;
    }

    if (OC_STACK_OK == res)
    {
        if (CALLBACK_NOT_INVOKED == waitCallbackRet())
        {
            IOTIVITYTEST_LOG(ERROR, "Callback Not Invoked");
            return false;
        }
    }

    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] provisionPairwiseDevices OUT\n");
    return true;
}

bool getLinkedDevices(DeviceList_t& deviceList, UuidList_t &uuidList, OCStackResult expectedResult)
{
    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] getLinkedDevices IN\n");
    OCStackResult res = OC_STACK_OK;
    g_cbInvoked = CALLBACK_NOT_INVOKED;

    res = deviceList[0]->getLinkedDevices(uuidList);

    IOTIVITYTEST_LOG(DEBUG, "[API Return Code] getLinkedDevices returns : %s\n",
            getOCStackResultCPP(res).c_str());

    if (res != expectedResult)
    {
        //m_failureMessage = setFailureMessage(expectedResult, res);
        return false;
    }

    if (!uuidList.size())
    {
        IOTIVITYTEST_LOG(DEBUG, "No Devices are Linked\n");
    }

    for (unsigned int i = 0; i < uuidList.size(); i++)
    {
        printf("%d. Linked Device ID : ", i);
        printUuid(uuidList[i]);
    }

    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] getLinkedDevices OUT\n");
    return true;
}

bool unlinkDevices(DeviceList_t& deviceList, const OCSecureResource &device2,
        ResultCallBack resultCallback, OCStackResult expectedResult)
{
    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] unlinkDevices IN\n");
    OCStackResult res = OC_STACK_OK;
    g_cbInvoked = CALLBACK_NOT_INVOKED;

    res = deviceList[0]->unlinkDevices(device2, resultCallback);
    IOTIVITYTEST_LOG(DEBUG, "[API Return Code] unlinkDevices returns : %s\n",
            getOCStackResultCPP(res).c_str());

    if (res != expectedResult)
    {
        IOTIVITYTEST_LOG(ERROR, "Expected Result Mismatch");
        return false;
    }

    if (OC_STACK_OK == res)
    {
        if (CALLBACK_NOT_INVOKED == waitCallbackRet())
        {
            IOTIVITYTEST_LOG(ERROR, "Callback Not Invoked");
            return false;
        }
    }

    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] unlinkDevices OUT\n");
    return true;
}

bool removeDevice(DeviceList_t& deviceList, unsigned short waitTimeForOwnedDeviceDiscovery,
        ResultCallBack resultCallback, OCStackResult expectedResult)
{
    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] removeDevice IN");
    OCStackResult res = OC_STACK_OK;
    g_cbInvoked = CALLBACK_NOT_INVOKED;

    res = deviceList[0]->removeDevice(waitTimeForOwnedDeviceDiscovery, resultCallback);
    IOTIVITYTEST_LOG(DEBUG, "[API Return Code] removeDevice returns : %s\n",
            getOCStackResultCPP(res).c_str());

    if (res != expectedResult)
    {
        IOTIVITYTEST_LOG(ERROR, "Expected Result Mismatch");
        return false;
    }

    if (OC_STACK_OK == res)
    {
        if (CALLBACK_NOT_INVOKED == waitCallbackRet())
        {
            IOTIVITYTEST_LOG(ERROR, "Callback Not Invoked");
            return false;
        }
    }

    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] removeDevice OUT");
    return true;
}

bool removeDeviceWithUuid(unsigned short waitTimeForOwnedDeviceDiscovery, std::string uuid,
        ResultCallBack resultCallback, OCStackResult expectedResult)
{
    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] removeDeviceWithUuid IN");
    OCStackResult res = OC_STACK_OK;
    g_cbInvoked = CALLBACK_NOT_INVOKED;

    res = OCSecure::removeDeviceWithUuid(waitTimeForOwnedDeviceDiscovery, uuid, resultCallback);
    IOTIVITYTEST_LOG(DEBUG, "[API Return Code] removeDeviceWithUuid returns : %s",
            getOCStackResultCPP(res).c_str());

    if (res != expectedResult)
    {
        IOTIVITYTEST_LOG(ERROR, "Expected Result Mismatch");
        return false;
    }

    if (OC_STACK_OK == res)
    {
        if (CALLBACK_NOT_INVOKED == waitCallbackRet())
        {
            IOTIVITYTEST_LOG(ERROR, "Callback Not Invoked");
            return false;
        }
    }

    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] removeDeviceWithUuid OUT");
    return true;
}

/**
 * Callback function for doOwnership Transfer
 *
 * @param[in] result Result list
 * @param[in] hasError indicates if the result has error
 */
void ownershipTransferCB(PMResultList_t *result, int hasError)
{
    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] ownershipTransferCB IN");

    if (hasError)
    {
        IOTIVITYTEST_LOG(ERROR, "[PMCppHelper] OwnershipTransfer ERROR!!!");
        g_cbInvoked = CALLBACK_INVOKED;
    }
    else
    {
        IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] \nTransferred Ownership successfully for device : ");
        printUuid(result->at(0).deviceId);
        delete result;
        g_cbInvoked = CALLBACK_INVOKED;
    }

    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] ownershipTransferCB OUT");
}

/**
 * Callback function for provisioning.
 *
 * @param[in] result Result list
 * @param[in] hasError indicates if the result has error
 */
void provisionCB(PMResultList_t *result, int hasError)
{
    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] provisionCB IN\n");

    if (hasError > 4)
    {
        IOTIVITYTEST_LOG(ERROR, "[PMCppHelper] Provisioning ERROR %d!!!\n", hasError);
        g_cbInvoked = CALLBACK_INVOKED;
    }
    else
    {
        IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] Received provisioning results: \n");

        for (unsigned int i = 0; i < result->size(); i++)
        {
            IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] Result is = %d for device : ",
                    result->at(i).res);
            printUuid(result->at(i).deviceId);

            g_cbInvoked = CALLBACK_INVOKED;
        }

        delete result;
    }

    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] provisionCB OUT");
}

std::string getOCStackResultCPP(OCStackResult ocstackresult)
{
    std::string resultString = EMPTY_STRING;

    switch (ocstackresult)
    {
        case OC_STACK_OK:
            resultString = "OC_STACK_OK";
            break;
        case OC_STACK_RESOURCE_CREATED:
            resultString = "OC_STACK_RESOURCE_CREATED";
            break;
        case OC_STACK_RESOURCE_DELETED:
            resultString = "OC_STACK_RESOURCE_DELETED";
            break;
        case OC_STACK_INVALID_URI:
            resultString = "OC_STACK_INVALID_URI";
            break;
        case OC_STACK_INVALID_QUERY:
            resultString = "OC_STACK_INVALID_QUERY";
            break;
        case OC_STACK_INVALID_IP:
            resultString = "OC_STACK_INVALID_IP";
            break;
        case OC_STACK_INVALID_PORT:
            resultString = "OC_STACK_INVALID_PORT";
            break;
        case OC_STACK_INVALID_CALLBACK:
            resultString = "OC_STACK_INVALID_CALLBACK";
            break;
        case OC_STACK_INVALID_METHOD:
            resultString = "OC_STACK_INVALID_METHOD";
            break;
        case OC_STACK_INVALID_PARAM:
            resultString = "OC_STACK_INVALID_PARAM";
            break;
        case OC_STACK_INVALID_OBSERVE_PARAM:
            resultString = "OC_STACK_INVALID_OBSERVE_PARAM";
            break;
        case OC_STACK_NO_MEMORY:
            resultString = "OC_STACK_NO_MEMORY";
            break;
        case OC_STACK_COMM_ERROR:
            resultString = "OC_STACK_COMM_ERROR";
            break;
        case OC_STACK_TIMEOUT:
            resultString = "OC_STACK_TIMEOUT";
            break;
        case OC_STACK_ADAPTER_NOT_ENABLED:
            resultString = "OC_STACK_ADAPTER_NOT_ENABLED";
            break;
        case OC_STACK_NOTIMPL:
            resultString = "OC_STACK_NOTIMPL";
            break;
        case OC_STACK_NO_RESOURCE:
            resultString = "OC_STACK_NO_RESOURCE";
            break;
        case OC_STACK_ERROR:
            return "OC_STACK_ERROR";
            break;
        case OC_STACK_INVALID_DEVICE_INFO:
            return "OC_STACK_INVALID_DEVICE_INFO";
            break;
        case OC_STACK_NOT_ACCEPTABLE:
            return "OC_STACK_NOT_ACCEPTABLE";
            break;
        default:
            resultString = "UNKNOWN_STATE";
    }

    return resultString;
}

int waitCallbackRet()
{
    printf("waitCallbackRet IN\n");

    for (int i = 0; CALLBACK_TIMEOUT > i; ++i)
    {
        printf("waitCallbackRet Loop = %d\n", i);

        if (CALLBACK_INVOKED == g_cbInvoked)
        {
            return CALLBACK_INVOKED;
        }

        sleep(DELAY_SHORT);

        if (OC_STACK_OK != OCProcess())
        {
            printf("OCStack process error\n");
            return CALLBACK_NOT_INVOKED;
        }
    }

    printf("waitCallbackRet OUT\n");
    return CALLBACK_NOT_INVOKED;
}

/**
 * Function for Convert String to Device Uuid
 */
bool convertStrToUuid(std::string uuid, OicUuid_t* deviceID, OCStackResult expectedResult)
{
    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] convertStrToUuid IN");

    OCStackResult rst;

    rst = ConvertStrToUuid(uuid.c_str(), deviceID);
    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] convertStrToUuid returns : %s",
            getOCStackResultCPP(rst).c_str());

    if (OC_STACK_OK != rst)
    {
        IOTIVITYTEST_LOG(ERROR, "Expected Result Mismatch");
        return false;
    }

    IOTIVITYTEST_LOG(DEBUG, "[PMCppHelper] convertStrToUuid OUT");

    return true;
}

void removeAllResFile(int resFile)
{
    if (resFile == JUSTWORK1)
    {
        CommonUtil::rmFile(JUSTWORKS_SERVER2_CBOR);
    }
    else if (resFile == JUSTWORK2)
    {
        CommonUtil::rmFile(JUSTWORKS_SERVER1_CBOR);
    }
    else if (resFile == MVJUSTWORK)
    {
        CommonUtil::rmFile(MVJUSTWORKS_SERVER_CBOR);
    }
    else if (resFile == RANDOMPIN)
    {
        CommonUtil::rmFile(RANDOMPIN_SERVER_CBOR);
    }
    else if (resFile == PRECONFIGPIN1)
    {
        CommonUtil::rmFile(PRECONFIG_SERVER1_CBOR);
    }
    else if (resFile == PRECONFIGPIN2)
    {
        CommonUtil::rmFile(PRECONFIG_SERVER2_CBOR);
    }
    else if (resFile == CLIENT)
    {
        CommonUtil::rmFile(DATABASE_PDM);
        CommonUtil::rmFile(CLIENT_CBOR);
    }
    else if (resFile == MOTCLIENT)
    {
        CommonUtil::rmFile(MOT_DB_FILE_NAME);
        CommonUtil::rmFile(MOT_CLIENT_CBOR);
    }
}

void copyAllResFile(int resFile)
{
    if (resFile == JUSTWORK1)
    {
        CommonUtil::copyFile(JUSTWORKS_SERVER2_CBOR_BACKUP, JUSTWORKS_SERVER2_CBOR);
    }
    else if (resFile == JUSTWORK2)
    {
        CommonUtil::copyFile(JUSTWORKS_SERVER1_CBOR_BACKUP, JUSTWORKS_SERVER1_CBOR);
    }
    else if (resFile == MVJUSTWORK)
    {
        CommonUtil::copyFile(MVJUSTWORKS_SERVER_CBOR_BACKUP, MVJUSTWORKS_SERVER_CBOR);
    }
    else if (resFile == RANDOMPIN)
    {
        CommonUtil::copyFile(RANDOMPIN_SERVER_CBOR_BACKUP, RANDOMPIN_SERVER_CBOR);
    }
    else if (resFile == PRECONFIGPIN1)
    {
        CommonUtil::copyFile(PRECONFIG_SERVER1_CBOR_BACKUP, PRECONFIG_SERVER1_CBOR);
    }
    else if (resFile == PRECONFIGPIN2)
    {
        CommonUtil::copyFile(PRECONFIG_SERVER2_CBOR_BACKUP, PRECONFIG_SERVER2_CBOR);
    }
    else if (resFile == CLIENT)
    {
        CommonUtil::copyFile(CLIENT_CBOR_BACKUP, CLIENT_CBOR);
    }
    else if (resFile == MOTCLIENT)
    {
        CommonUtil::copyFile(MOT_CLIENT_CBOR_BACKUP, MOT_CLIENT_CBOR);
    }
}
