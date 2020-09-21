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
 ******************************************************************/

#ifndef COMMON_TESTCASE_SAMPLERESOURCE_H_
#define COMMON_TESTCASE_SAMPLERESOURCE_H_

#include "ResourceHelper.h"
#include "ResourceServer.h"

using namespace std;
namespace PH = std::placeholders;

class SampleResource: public ResourceServer
{
protected:
    int m_recursiveDelay;
    int m_scheduledDelay;
    bool m_isCancelCalled;
    bool m_isObserveRegistered;
    ObservationIds m_listOfObservers;
    shared_ptr< OCResourceResponse > m_pResponse;
    vector< string > m_resourceList;
    map< string, string > m_accessmodifier;
    std::shared_ptr< SampleResource > m_pSensorTwin;



public:
    SampleResource(void);

    SampleResource(std::string resourceUri,
            std::string resourceTypeName,
            std::string resourceInterface,
            uint8_t resourceProperty = OC_ACTIVE);

    SampleResource(std::string resourceUri,
            std::string resourceTypeName,
            std::string resourceInterface,
            uint8_t resourceProperty,
            OCRepresentation resourceRepresentation);

    virtual ~SampleResource(void);

    void setAsReadOnly(string key);

    bool isReadonly(string key);

    void setSensorTwin(std::shared_ptr< SampleResource > p_sensorResource);

    virtual void onResourceServerStarted(bool &isRegisteredForPresence, int &presenceInterval);

    virtual void handleObserveRequest(QueryParamsMap &queryParamsMap,
            std::shared_ptr< OCResourceRequest > request,
            std::shared_ptr< OCResourceResponse > response);

    virtual void handleDeleteRequest(QueryParamsMap &queryParamsMap,
            OCRepresentation incomingRepresentation, std::shared_ptr< OCResourceRequest > request,
            std::shared_ptr< OCResourceResponse > response);

    virtual void handlePostRequest(QueryParamsMap &queryParamsMap,
            OCRepresentation incomingRepresentation, std::shared_ptr< OCResourceRequest > request,
            std::shared_ptr< OCResourceResponse > response);

    virtual void handleGetRequest(QueryParamsMap &queryParamsMap,
            std::shared_ptr< OCResourceRequest > request,
            std::shared_ptr< OCResourceResponse > response);

    virtual void handleGetRequest(QueryParamsMap &queryParamsMap,
            std::shared_ptr< OCResourceRequest > request,
            std::shared_ptr< OCResourceResponse > response,
            OCRepresentation rep);

    virtual void handlePutRequest(QueryParamsMap &queryParamsMap,
            OCRepresentation incomingRepresentation, std::shared_ptr< OCResourceRequest > request,
            std::shared_ptr< OCResourceResponse > response);

    virtual void handleInitRequest(QueryParamsMap &queryParamsMap,
            std::shared_ptr< OCResourceRequest > request,
            std::shared_ptr< OCResourceResponse > response);

    virtual OCRepresentation getResourceRepresentation(OCRepresentation &resourceRep);

    OCStackResult addArrayAttribute(string key, OCRepresentation arrayRep);

    void notifyObservers();
    void notifyObservers(void *param);
    bool updateRepresentation(string key, AttributeValue value);
    bool updateRepresentation(string key, OCRepresentation incomingRep);
    bool updateBatchRepresentation(string key, OCRepresentation incomingRep, bool &isError);

protected:
    void handleRecursiveActionSet();

    void handleScheduledActionSet();

    bool updateRepresentation(string key, OCRepresentation incomingRep,
            shared_ptr< OCResourceResponse > response);

    void createResource(string initialUri, OCRepresentation incomingRepresentation,
            shared_ptr< OCResourceResponse > response);

    void supportCreateAndOthersForPUT(QueryParamsMap &queryParamsMap,
            OCRepresentation incomingRepresentation, std::shared_ptr< OCResourceRequest > request,
            std::shared_ptr< OCResourceResponse > response);
};

#endif /* COMMON_TESTCASE_SAMPLERESOURCE_H_ */
