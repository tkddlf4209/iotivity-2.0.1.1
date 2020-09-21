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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <stdarg.h>

#ifdef __LINUX__
#include <unistd.h>
#endif

#ifdef __TIZEN__
#include <unistd.h>
#include <glib.h>
#include <pthread.h>
#endif

#ifdef ARDUINO
#include "Arduino.h"
#ifdef ARDUINOWIFI
#include "WiFi.h"
#elif defined ARDUINOETH
#include "Ethernet.h"
#endif
#endif

#include "casimulator.h"
#include "cacommon.h"
#include "cainterface.h"
#include "cautilinterface.h"
#include "casecurityinterface.h"

#ifdef __WITH_DTLS__
#include "ca_adapter_net_ssl.h"
#include "ssl_ciphersuites.h"
#endif

#define IDENTITY     ("1111111111111111")
#define RS_CLIENT_PSK   ("AAAAAAAAAAAAAAAA")

char SIM_REQ_ACK[] = "SendReqAck";
char SIM_RES_ACK[] = "SendResAck";
char SIM_REQ_CONFIG[] = "Configure";

#define MAX_BUF_LEN 2048
#define MAX_SLEEP_TIME 1
#define CH_ZERO '0'

#ifdef __TIZEN__
static GMainLoop *g_mainloop = NULL;
pthread_t thread;
#endif

typedef enum
{
    SEND_MESSAGE = 0, SELECT_NETWORK, UNSELECT_NETWORK, STOP_SIM, TRANSFER_INFO
} SimulatorTask;

typedef struct
{
    SimulatorTask operationType;
    CAMethod_t caMethod;
    CAToken_t token;
    uint8_t tokenLength;
    int numberOfTimes;
    int interval;
    int bufLength;
    char *resourceUri;
    unsigned char payload[MAX_BUF_LEN];
    int payloadSize;
    uint16_t messageId;
} TestConfiguration;

bool g_firstMessage = true;
bool g_simulatorProcess = true;

CATransportAdapter_t g_selectedTransport = (CATransportAdapter_t)0;
int g_messageId = -1;
size_t g_identityLegth;
size_t g_pskLength;

char g_filterCommand[10];
char g_address[CA_MACADDR_SIZE];

void output(const char *format, ...)
{
#if defined (__LINUX__) || defined (__ANDROID_NATIVE__) || defined (__TIZEN__) || defined (__WINDOWS__)
    va_list arg;

    va_start (arg, format);

    vfprintf (stdout, format, arg);

    va_end (arg);
#endif

#ifdef ARDUINO
    Serial.print(format);
#endif

}

#ifdef ARDUINO
char *getData(char readInput[])
{
    while (!Serial.available())
    {

    }

    int len = 0;

    while (Serial.available())
    {
        delay(100);

        char c = Serial.read();

        if ('\n' != c && '\r' != c && len < MAX_BUF_LEN)
        {
            readInput[len++] = c;
        }
        else
        {
            break;
        }
    }

    readInput[len] = '\0';
    Serial.flush();
    Serial.print("PD: ");
    Serial.println(readInput);

    return readInput;
}
#endif

void customWait(int seconds)
{
#ifdef __LINUX__
    sleep(seconds);
#endif

#ifdef ARDUINO
    delay(seconds);
#endif
}

void clearDisplay()
{
#ifdef __LINUX__
    system("clear");
#endif

#ifdef ARDUINO
#endif
}

#ifdef __TIZEN__
void GMainLoopThread()
{
    g_main_loop_run(g_mainloop);
}
#endif

int initialize()
{
    CAResult_t result = CAInitialize(g_selectedTransport);

    if (result != CA_STATUS_OK)
    {
        output("CAInitialize failed\n");
        return 0;
    }

    return 1;
}

void printChars(unsigned char *p, int length)
{
    int i;

    for (i = 0; i < length; i++)
    {
        output("%c", p[i]);
    }
}

#ifdef __WITH_DTLS__

CAResult_t dtlsHandshakeCb(const CAEndpoint_t *endpoint, const CAErrorInfo_t *info)
{
    if (NULL == endpoint)
    {
      output("endpoint is null");
      return CA_STATUS_FAILED;
    }

    if (NULL == info)
    {
        output("ErrorInfo is null");
        return CA_STATUS_FAILED;
    }

    output("Remote device Address %s:%d:", endpoint->addr, endpoint->port);
    output("ErrorInfo: %d", info->result);
    return CA_STATUS_OK;
}

void initCipherSuiteList(bool *list, const char *deviceId)
{
    (void) deviceId;

    output("%s In\n", __func__);

    if (NULL == list)
    {
        output("Out %s", __func__);
        output("NULL list param");
        return;
    }

    list[0] = true;
    list[1] = true;

    output("%s Out\n", __func__);
}

int32_t getDtlsPskCredentials( CADtlsPskCredType_t type, const unsigned char *desc,
                               size_t desc_len, unsigned char *result, size_t result_length)
{
    output("getDtlsPskCredentials in\n");

    int32_t ret = -1;

    if (NULL == result)
    {
        return ret;
    }

    switch (type)
    {
        case CA_DTLS_PSK_HINT:
        case CA_DTLS_PSK_IDENTITY:
            output("CAGetDtlsPskCredentials CA_DTLS_PSK_IDENTITY\n");
            if (result_length < g_identityLegth)
            {
                output("ERROR : Wrong value for result for storing IDENTITY\n");
                return ret;
            }

            memcpy(result, IDENTITY, g_identityLegth);
            ret = g_identityLegth;
            break;

        case CA_DTLS_PSK_KEY:
            output("CAGetDtlsPskCredentials CA_DTLS_PSK_KEY\n");
            if ((desc_len == g_identityLegth) &&
                memcmp(desc, IDENTITY, g_identityLegth) == 0)
            {
                if (result_length < g_pskLength)
                {
                    output("ERROR : Wrong value for result for storing RS_CLIENT_PSK\n");
                    return ret;
                }

                memcpy(result, RS_CLIENT_PSK, g_pskLength);
                ret = g_pskLength;
            }
            break;

        default:

            output("Wrong value passed for PSK_CRED_TYPE.\n");
            ret = -1;
    }

    output("getDtlsPskCredentials out\n");

    return ret;
}

int setupSecurity(int selectedTransport)
{
    g_identityLegth = strlen(IDENTITY);
    g_pskLength = strlen(RS_CLIENT_PSK);

    int result = CAregisterPskCredentialsHandler(getDtlsPskCredentials);
    if (result != CA_STATUS_OK)
    {
        output("CAregisterPskCredentialsHandler failed. return value is %d\n", result);
        return 0;
    }

    if (selectedTransport == CA_ADAPTER_TCP)
    {
        result = CAregisterSslHandshakeCallback(dtlsHandshakeCb);
        if (result != CA_STATUS_OK)
        {
            output("CAregisterSslHandshakeCallback failed. return value is %d\n", result);
            return 0;
        }
    }

    CAsetCredentialTypesCallback(initCipherSuiteList);

    if (selectedTransport == CA_ADAPTER_IP)
    {
        result = CASelectCipherSuite(MBEDTLS_TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256,
                                     (CATransportAdapter_t)selectedTransport);
    }
    else if (selectedTransport == CA_ADAPTER_TCP)
    {
        result = CAsetTlsCipherSuite(MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8);
    }

    if (result != CA_STATUS_OK)
    {
        output("CACipherSuite failed. return value is %d\n", result);
        return 0;
    }

    return 1;
}

#endif

int selectNetwork(int argc, char *argv[])
{
    int number;
    int ret = 0;

    if (argc < 2)
    {
        output("\n=============================================\n");

        output("\tSelect Transport\n");
        output("IP     : 0\n");
        output("GATT   : 1\n");
        output("RFCOMM : 2\n");
        output("TCP    : 4\n");
        output("Select : ");

        scanf("%1d", &number);
    }
    else
    {
        number = argv[1][0] - '0';
    }

    if (number >= 0 && number <= 4)
    {
        g_selectedTransport = (CATransportAdapter_t)(1 << number);

        output("Selecting Network ...\n");

        CAResult_t result = CASelectNetwork((CATransportAdapter_t)g_selectedTransport);

        if (result == CA_STATUS_OK)
        {
            output("CASelectNetwork Successful\n");
#ifdef TCP_ADAPTER
            if (g_selectedTransport == CA_ADAPTER_TCP)
            {
                result = CASelectNetwork(CA_ADAPTER_IP);
                if (result == CA_STATUS_OK)
                {
                    output("IP Network selection Successful\n");
                    ret = 1;
                }
                else
                {
                    output("IP Network selection Failed\n");
                    ret = 0;
                }
            }
#endif

#if defined (__LINUX__) || defined (__WINDOWS__)
            if (argc < 3)
            {
                output("Enter Filter Address: ");
                scanf("%15s", g_address);
            }
            else
            {
                strncpy(g_address, argv[2], 15);
            }

            if (strchr(g_address, '.'))
            {
                if (argc < 4)
                {
                    output("Enter Filter Command: ");
                    scanf("%2s", g_filterCommand);
                }
                else
                {
                    strncpy(g_filterCommand, argv[3], 2);
                }
            }
#endif
            ret = 1;
        }
        else
        {
            output("CASelectNetwork Failed\n");
            ret = 0;
        }
    }
    else
    {
        output("Invalid transport number\n");
        ret = 0;
    }

    output("\n=============================================\n");

    return ret;
}

int startServer()
{
    CAResult_t result = CAStartListeningServer();
    if (result != CA_STATUS_OK)
    {
        output("CAStartListeningServer Failed\n");
        return 0;
    }
    else
    {
        output("Server Started Successfully\n");
    }

    return 1;
}

int startClient()
{
    CAResult_t result = CAStartDiscoveryServer();
    if (result != CA_STATUS_OK)
    {
        output("CAStartDiscoveryServer\n");
        return 0;
    }
    else
    {
        output("Client Started Successfully\n");
    }

    return 1;
}

int handleMessage()
{
    CAResult_t res = CAHandleRequestResponse();

    if (res != CA_STATUS_OK)
    {
        return 0;
    }

    return 1;
}

int returnResponse(const CAEndpoint_t *endpoint, char *resourceUri, unsigned char *payload,
                   int payloadSize,
                   CAMessageType_t type, CAResponseResult_t responseCode, uint16_t messageId, CAToken_t token,
                   uint8_t tokenLength, CAHeaderOption_t *options, uint8_t numOptions)
{
    output("[returnResponse] in\n");

    CAInfo_t responseData;

    responseData.payload = payload;

    if (payload != NULL)
    {
        responseData.payloadSize = payloadSize;
    }
    else
    {
        responseData.payloadSize = 0;
    }

    responseData.type = type;
    responseData.messageId = messageId;
    responseData.resourceUri = resourceUri;
    responseData.token = token;
    responseData.tokenLength = tokenLength;
    responseData.options = options;
    responseData.numOptions = numOptions;
    responseData.dataType = CA_RESPONSE_DATA;
    responseData.payloadFormat = CA_FORMAT_UNDEFINED;
    responseData.acceptFormat = CA_FORMAT_UNDEFINED;

    CAResponseInfo_t responseInfo;
    responseInfo.result = responseCode;
    responseInfo.info = responseData;
    responseInfo.isMulticast = 0;

    output("Sending response....\n");

    CAResult_t res = CASendResponse(endpoint, &responseInfo);

    output("Response Send....\n");

    if (res != CA_STATUS_OK)
    {
        output("send response error\n");
    }
    else
    {
        output("send response success\n");
    }

    output("[returnResponse] out\n");

    return 1;
}

void processRequestResponse(void)
{
    bool showSuccess = true;
    bool showError = true;

    while (g_simulatorProcess)
    {
        if (!handleMessage())
        {
            if (showError)
            {
                output("handle request error\n");
                showError = false;
                showSuccess = true;
            }
        }
        else
        {
            if (showSuccess)
            {
                output("handle request success\n");
                showSuccess = false;
                showError = true;
            }
        }

        customWait(MAX_SLEEP_TIME);
    }
}

void returnMessage(const CAEndpoint_t *endpoint, TestConfiguration *testConfig)
{
    int index = 0;

    output("returnMessage in\n");

    for (index = 0; index < testConfig->numberOfTimes; index++)
    {
        returnResponse(endpoint, testConfig->resourceUri, testConfig->payload,
                       testConfig->payloadSize, CA_MSG_NONCONFIRM, CA_VALID, testConfig->messageId,
                       testConfig->token, testConfig->tokenLength, NULL, 0);
        customWait(testConfig->interval);
    }

    output("returnMessage out\n");
}

void requestHandler(const CAEndpoint_t *endpoint, const CARequestInfo_t *requestInfo)
{
    TestConfiguration testConfig;
    char numConversion[5];

    output("requestHandler in\n");

    if (!endpoint)
    {
        output("endpoint is NULL\n");
        return;
    }

    if (!requestInfo)
    {
        output("requestInfo is NULL\n");
        return;
    }

    output("IP %s, Port %d\n", endpoint->addr, endpoint->port);
    output("Message Id: %d\n", requestInfo->info.messageId);

#if defined (__LINUX__) || defined (__WINDOWS__)
    if (strcmp(g_address, ""))
    {
        if (!strcmp(g_filterCommand, "ne") && !strcmp(g_address, endpoint->addr))
        {
            output("Ignoring filter address\n");
            return;
        }

        if (!strcmp(g_filterCommand, "eq") && strcmp(g_address, endpoint->addr))
        {
            output("Ignoring filter address\n");
            return;
        }
    }
#endif

    if (requestInfo->info.options)
    {
        output("Header Option Found\n");

        uint32_t len = requestInfo->info.numOptions;
        uint32_t i;
        for (i = 0; i < len; i++)
        {
            output("Option ID : %d\n", requestInfo->info.options[i].optionID);
            output("Option Data[%d]: %s\n", requestInfo->info.options[i].optionLength,
                   requestInfo->info.options[i].optionData);
        }
    }
    else
    {
        output("No Header Option Found\n");
    }

    if (requestInfo->info.type == CA_MSG_RESET)
    {
        char str[12];

        output("messageType: CA_MSG_RESET\n");

        sprintf(str, "%d", requestInfo->info.messageId);

        output("calling returnResponse ...\n");

        returnResponse(endpoint, SIM_RES_ACK, (unsigned char *)str, strlen(str), CA_MSG_NONCONFIRM,
                       CA_VALID,
                       requestInfo->info.messageId, requestInfo->info.token, requestInfo->info.tokenLength,
                       requestInfo->info.options, requestInfo->info.numOptions);

        output("returnResponse called\n");

        return;
    }

    output("PayloadSize: %d\n", requestInfo->info.payloadSize);

    if (requestInfo->info.payload)
    {
        output("Payload: ");
        printChars(requestInfo->info.payload, requestInfo->info.payloadSize);
        output("\n");
    }
    else
    {
        output("Payload is NULL\n");
    }

    if (!requestInfo->info.resourceUri)
    {
        output("ResourceUri is NULL\n");
        return;
    }

    output("ResourceUri: %s\n", requestInfo->info.resourceUri);

    if (strstr(requestInfo->info.resourceUri, SIM_REQ_CONFIG) != NULL)
    {
        int temp;
        output("ResourceUri Type: SIM_REQ_CONFIG\n");

        testConfig.operationType = (SimulatorTask)(requestInfo->info.payload[0] - CH_ZERO);

        switch (testConfig.operationType)
        {
            case SEND_MESSAGE:
                output("OperationType: SEND_MESSAGE\n");
                usleep (5000000);

                memset(&numConversion, 0, sizeof(numConversion));
                strncpy(numConversion, (const char *)&requestInfo->info.payload[2], sizeof(numConversion) - 1);
                numConversion[sizeof(numConversion) - 1] = 0;
                testConfig.numberOfTimes = atoi(numConversion); //4 byte

                memset(&numConversion, 0, sizeof(numConversion));
                strncpy(numConversion, (const char *)&requestInfo->info.payload[6], sizeof(numConversion) - 1);
                numConversion[sizeof(numConversion) - 1] = 0;
                testConfig.interval = atoi(numConversion); //4 byte

                memset(&numConversion, 0, sizeof(numConversion));
                strncpy(numConversion, (const char *)&requestInfo->info.payload[10], sizeof(numConversion) - 1);
                numConversion[sizeof(numConversion) - 1] = 0;
                testConfig.bufLength = atoi(numConversion); //4 byte
                memset(&testConfig.payload, 0, sizeof(char) * MAX_BUF_LEN);
                strncpy((char *)testConfig.payload, (const char *)&requestInfo->info.payload[14],
                        testConfig.bufLength);
                testConfig.payload[testConfig.bufLength + 1] = '\0';
                testConfig.payloadSize = strlen((const char *)testConfig.payload);
                testConfig.caMethod = requestInfo->method;
                testConfig.resourceUri = requestInfo->info.resourceUri;
                testConfig.messageId = requestInfo->info.messageId;

                if (requestInfo->info.token != NULL)
                {
                    testConfig.token = requestInfo->info.token;
                    testConfig.tokenLength = requestInfo->info.tokenLength;
                }
                else
                {
                    testConfig.token[0] = 0;
                    testConfig.tokenLength = 0;
                }

                returnMessage(endpoint, &testConfig);

                break;

            case STOP_SIM:
                output("OperationType: STOP_SIM\n");
                g_simulatorProcess = false;
                break;

            default:
                output("OperationType: Unknown\n");
                break;
        }
    }
    else if (strstr(requestInfo->info.resourceUri, SIM_REQ_ACK) != NULL)
    {
        output("resourceUri Type: SIM_REQ_ACK\n");

        CAMessageType_t messageType;

        if (requestInfo->info.type == CA_MSG_CONFIRM)
        {
            output("Message Type: CA_MSG_CONFIRM\n");

            if (g_messageId != requestInfo->info.messageId)
            {
                output("Ignoring first time\n");
                g_messageId = requestInfo->info.messageId;
                return;
            }

            messageType = CA_MSG_ACKNOWLEDGE;
        }
        else
        {
            output("Message Type: CA_MSG_NONCONFIRM\n");
            messageType = CA_MSG_NONCONFIRM;
        }
        int i;
        char payload[2000];
        for (i = 0; i < (int)requestInfo->info.payloadSize; i++)
        {
            payload[i] = requestInfo->info.payload[i];
        }
        payload[i] = 0;

        if (g_selectedTransport == CA_ADAPTER_IP)
        {
            char str[12];
            strcat(payload, "port:");
            sprintf(str, "%d", CAGetAssignedPortNumber(CA_ADAPTER_IP, CA_IPV4));
            strcat(payload, str);
            strcat(payload, ",secure-port:");
            sprintf(str, "%d", CAGetAssignedPortNumber(CA_ADAPTER_IP,
                    (CATransportFlags_t)(CA_SECURE | CA_IPV4)));
            strcat(payload, str);
        }
        else if (g_selectedTransport == CA_ADAPTER_TCP)
        {
            char str[12];
            strcat(payload, "port:");
            printf("client tcp port: %d", CAGetAssignedPortNumber(CA_ADAPTER_TCP, CA_IPV4));
            sprintf(str, "%d", CAGetAssignedPortNumber(CA_ADAPTER_TCP, CA_IPV4));
            strcat(payload, str);
            strcat(payload, ",secure-port:");
            printf("client tcp secure port: %d\n", CAGetAssignedPortNumber(CA_ADAPTER_TCP,
                    (CATransportFlags_t)(CA_SECURE | CA_IPV4)));
            sprintf(str, "%d", CAGetAssignedPortNumber(CA_ADAPTER_TCP,
                    (CATransportFlags_t)(CA_SECURE | CA_IPV4)));
            strcat(payload, str);
        }
        else
        {
            strcat(payload, "port:0");
            strcat(payload, ",secure-port:0");
        }

        output ("payload %s\n", payload);
        output("calling returnResponse ...\n");

        returnResponse(endpoint, requestInfo->info.resourceUri, (unsigned char *)payload,
                       strlen(payload), messageType, CA_VALID, requestInfo->info.messageId,
                       requestInfo->info.token, requestInfo->info.tokenLength, requestInfo->info.options,
                       requestInfo->info.numOptions);

        output("returnResponse called\n");
    }
    else
    {
        output("ResourceUri Type is Unknown\n");
    }

    output("requestHandler out\n");
}

void responseHandler(const CAEndpoint_t *endpoint, const CAResponseInfo_t *responseInfo)
{
    output("Something Wrong!!! Response Handler shouldn't be called for CA Simulator\n");
    output("Check whether message comes from CA testcases or somewhere else\n");

    if (!endpoint)
    {
        output("endpoint is NULL\n");
        return;
    }

    if (!responseInfo)
    {
        output("responseInfo is NULL\n");
        return;
    }

    output("IP %s, Port %d\n", endpoint->addr, endpoint->port);
}

void errorHandler(const CAEndpoint_t *endpoint, const CAErrorInfo_t *errorInfo)
{
    output("errorHandler in\n");

    if (!errorInfo)
    {
        output("ErrorInfo NULL\n");
        return;
    }

    if (!endpoint)
    {
        output("endpoint is NULL\n");
        return;
    }

    output("IP %s, Port %d\n", endpoint->addr, endpoint->port);

    output("result: %d\n", errorInfo->result);

    const CAInfo_t *info = &errorInfo->info;

    if (!info->token)
    {
        output("token is NULL\n");
    }

    output("tokenLength: %d\n", info->tokenLength);
    output("messageId: %d\n", (uint16_t) info->messageId);
    output("type: %d\n", info->type);
    output("payloadSize: %d\n", info->payloadSize);

    if (info->resourceUri)
    {
        output("resourceUri: %s\n", info->resourceUri);
    }
    else
    {
        output("resourceUri is NULL\n");
    }

    if (info->payload)
    {
        output("payload: ");
        printChars(info->payload, info->payloadSize);
        output("\n");
    }
    else
    {
        output("Payload is NULL\n");
    }

    if (CA_ADAPTER_NOT_ENABLED == errorInfo->result)
    {
        output("CA_ADAPTER_NOT_ENABLED, enable the adapter\n");
    }
    else if (CA_SEND_FAILED == errorInfo->result)
    {
        output("CA_SEND_FAILED, unable to send the message, check parameters\n");
    }
    else if (CA_MEMORY_ALLOC_FAILED == errorInfo->result)
    {
        output("CA_MEMORY_ALLOC_FAILED, insufficient memory\n");
    }
    else if (CA_SOCKET_OPERATION_FAILED == errorInfo->result)
    {
        output("CA_SOCKET_OPERATION_FAILED, socket operation failed\n");
    }
    else if (CA_STATUS_FAILED == errorInfo->result)
    {
        output("CA_STATUS_FAILED, message could not be delivered, internal error\n");
    }

    output("errorHandler out\n");
}

int registerMessageHandler()
{
    CARegisterHandler(requestHandler, responseHandler, errorHandler);

    return 1;
}

#ifdef ARDUINO

void setup()
{
    Serial.print("setup in");

    Serial.begin(115200);
    Serial.println("=======================");
    Serial.println("CA Simulator");
    Serial.println("=======================");
    Serial.println("Enter P to pause & R to Resume (if paused) simulator");
    Serial.println("=======================");

#ifdef ARDUINOETH
    g_selectedTransport = CA_ADAPTER_IP;
    // Note: ****Update the MAC address here with your shield's MAC address****
    uint8_t ETHERNET_MAC[] =
    {   0x90, 0xA2, 0xDA, 0x0F, 0xE0, 0xD8};
    uint8_t error = Ethernet.begin(ETHERNET_MAC);
    if (error == 0)
    {
        Serial.print("Failed: ");
        Serial.println(error);
        return;
    }
    Serial.print("Ethernet IP: ");
    Serial.println(Ethernet.localIP());
    Serial.print("Port: ");
    Serial.println(caglobals.ip.u4.port);
#endif

#ifdef ARDUINOWIFI
    g_selectedTransport = CA_ADAPTER_IP;

    const char ssid[] = "NETGEAR24"; // your network SSID (name)
    const char pass[] = "mightyrabbit219";// your network password

    int16_t status = WL_IDLE_STATUS; // the Wifi radio's status

    if (WiFi.status() == WL_NO_SHIELD)
    {
        Serial.println("ERROR:No Shield");
        return;
    }

    while (status != WL_CONNECTED)
    {
        Serial.print("connecting: ");
        Serial.println(ssid);
        // WiFi.begin api is weird. ssid should also be taken as const char *
        // Connect to WPA/WPA2 network:
        status = WiFi.begin((char *)ssid, pass);
    }
    Serial.print("Wifi IP:");
    Serial.println(WiFi.localIP());
#endif

#ifdef LE_ADAPTER
    g_selectedTransport = CA_ADAPTER_GATT_BTLE;
#endif

    CAResult_t result = CA_STATUS_OK;

    result = CAInitialize(g_selectedTransport);
    if (result == CA_STATUS_OK)
    {
        Serial.println("Initialization Successful");
    }

    result = CASelectNetwork(CA_ADAPTER_IP);
    if (result == CA_STATUS_OK)
    {
        Serial.println("Network Selection Successful");
    }

    startServer();

    registerMessageHandler();
    Serial.println("Callbacks registered");
}

void loop()
{
    char buffer[5];
    memset(buffer, 0, sizeof(buffer));
    int16_t len;
    int loop_status = 1;

    CAResult_t result = CA_STATUS_OK;

    if (g_simulatorProcess)
    {
        result = CAHandleRequestResponse();

        if (result == CA_STATUS_OK)
        {
            Serial.println("CAHandleRequestResponse Succeeded");
        }
        else
        {
            Serial.println("CAHandleRequestResponse Failed!");
        }

        delay(MAX_SLEEP_TIME * 1000);
    }

    if (Serial.available() > 0)
    {
        char readInput[MAX_BUF_LEN + 1];
        char *buffer = getData(readInput);
        switch (toupper(buffer[0]))
        {
            case 'P':
                g_simulatorProcess = false;
                Serial.println("Simulator paused");
                break;

            case 'R':
                g_simulatorProcess = true;
                Serial.println("Simulator resumed");
                break;
            case 'E':
                g_simulatorProcess = false;
                loop_status = 0;
                Serial.println("Simulator stopped");
                break;
        }
    }

    if (!loop_status)
    {
        exit(0);
    }
}

#endif

#if defined (__LINUX__) || defined (__ANDROID_NATIVE__) || defined (__TIZEN__) || defined (__WINDOWS__)
int main(int argc, char *argv[])
{
    clearDisplay();

    output("[CASimulator] IN\n");

#ifdef __TIZEN__
    g_mainloop = g_main_loop_new(NULL, FALSE);
    if (!g_mainloop)
    {
        output("g_main_loop_new failed\n");
        return -1;
    }

    if (pthread_create(&thread, NULL, (void *) &GMainLoopThread, NULL) < 0)
    {
        output("pthread_create failed in initialize\n");
        return -1;
    }
#endif

    caglobals.server = true;

    if (!initialize())
    {
        return -1;
    }

    if (!selectNetwork(argc, argv))
    {
        return -1;
    }

    if (!startServer())
    {
        return -1;
    }

    if (!registerMessageHandler())
    {
        return -1;
    }

#ifdef __WITH_DTLS__
    if (!setupSecurity(g_selectedTransport))
    {
        return -1;
    }
#endif

    if (g_selectedTransport == CA_ADAPTER_IP)
    {
        output("Port: %d;\n", CAGetAssignedPortNumber(CA_ADAPTER_IP, CA_IPV4));
        output("Secure Port: %d;\n", CAGetAssignedPortNumber(CA_ADAPTER_IP,
                (CATransportFlags_t)(CA_SECURE | CA_IPV4)));
    }
    else if (g_selectedTransport == CA_ADAPTER_TCP)
    {
        output("Mulitcast Port: %d;\n", CAGetAssignedPortNumber(CA_ADAPTER_IP, CA_IPV4));
        output("TCP Port: %d;\n", CAGetAssignedPortNumber(CA_ADAPTER_TCP, CA_IPV4));
        output("TCP Secure Port: %d;\n", CAGetAssignedPortNumber(CA_ADAPTER_TCP,
                (CATransportFlags_t)(CA_SECURE | CA_IPV4)));
    }

    processRequestResponse();

    CATerminate();

    output("[CASimulator] out\n");

#ifdef __TIZEN__
    g_main_loop_quit(g_mainloop);
#endif

    return 0;
}

#endif
