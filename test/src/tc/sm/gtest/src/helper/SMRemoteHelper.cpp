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

#include "SMRemoteHelper.h"

std::mutex SMRemoteHelper::s_mutex;

SMRemoteHelper::SMRemoteHelper() {
    IOTIVITYTEST_LOG(DEBUG, "[SMRemoteHelper] IN");
    PlatformConfig config { OC::ServiceType::InProc, ModeType::Both, "0.0.0.0",
            0, OC::QualityOfService::LowQos };
    OCPlatform::Configure(config);
    m_resourceTypes = {RESOURCE_TYPE_SCENELIST, RESOURCE_TYPE_LIGHT, RESOURCE_TYPE_FAN};
    m_relativetUri = OC_RSRVD_WELL_KNOWN_URI;
}

void SMRemoteHelper::discoverResource() {
    std::string exceptionMsg = "";
    try {
        m_pDiscoveryTask =
                RCSDiscoveryManager::getInstance()->discoverResourceByTypes(
                        RCSAddress::multicast(), m_relativetUri, m_resourceTypes,
                        std::bind(&SMRemoteHelper::onRemoteResourceDiscovered,
                                this, PH::_1));
    } catch (const RCSPlatformException& e) {
        exceptionMsg = e.what();
        IOTIVITYTEST_LOG(ERROR, "%s", exceptionMsg.c_str());
    } catch (const RCSException& e) {
        exceptionMsg = e.what();
        IOTIVITYTEST_LOG(ERROR, "%s", exceptionMsg.c_str());
    }
    IOTIVITYTEST_LOG(INFO, "Wait 5 seconds until resources discovered.");
    CommonUtil::waitInSecond(MAX_SLEEP_TIME);
}

void SMRemoteHelper::stopDiscovery() {
    m_pDiscoveryTask->cancel();
}

void SMRemoteHelper::onRemoteResourceDiscovered(
        std::shared_ptr<RCSRemoteResourceObject> foundResource) {
    s_mutex.lock();
    IOTIVITYTEST_LOG(INFO, "onResourceDiscovered callback");

    std::string resourceURI = foundResource->getUri();
    std::string hostAddress = foundResource->getAddress();
    std::vector<std::string> vecRTs = foundResource->getTypes();

    IOTIVITYTEST_LOG(INFO, "Resource URI : %s", resourceURI.c_str());
    IOTIVITYTEST_LOG(INFO, "Resource Host : %s", hostAddress.c_str());

    // if the found resource is a scene list resource
    if (std::find(vecRTs.begin(), vecRTs.end(), RESOURCE_TYPE_SCENELIST)
            != vecRTs.end())
        g_pFoundSceneList = foundResource;

    // if the found resource is a light resource
    else if (std::find(vecRTs.begin(), vecRTs.end(), RESOURCE_TYPE_LIGHT)
            != vecRTs.end()) {
        g_pFoundLightResource = foundResource;
        g_vecFoundResourceList.push_back(foundResource);
    }

    // if the found resource is a fan resource
    else if (std::find(vecRTs.begin(), vecRTs.end(), RESOURCE_TYPE_FAN)
            != vecRTs.end()) {
        g_pFoundFanResource = foundResource;
        g_vecFoundResourceList.push_back(foundResource);
    }

    s_mutex.unlock();
}

void SMRemoteHelper::onRemoteSceneListCreated(
        RemoteSceneList::Ptr remoteSceneList, int eCode) {
    IOTIVITYTEST_LOG(INFO, "%s is called with error code: %d", __func__, eCode);

    if (eCode == SCENE_RESULT_OK) {
        m_pHelperRemoteSceneList = std::move(remoteSceneList);
    } else {
        IOTIVITYTEST_LOG(INFO, "Create remote scene list failed");
    }
}

std::shared_ptr<RemoteSceneList> SMRemoteHelper::createRemoteSceneListInstance() {
    RemoteSceneList::createInstance(g_pFoundSceneList,
            std::bind(&SMRemoteHelper::onRemoteSceneListCreated, this,
                    placeholders::_1, placeholders::_2));
    CommonUtil::waitInSecond(CALLBACK_WAIT_MAX);

    return m_pHelperRemoteSceneList;
}
