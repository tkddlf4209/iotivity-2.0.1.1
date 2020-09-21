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

#ifndef INCLUDE_TESTCASE_SMREMOTEHELPER_H_
#define INCLUDE_TESTCASE_SMREMOTEHELPER_H_

#include "RemoteScene.h"
#include "RemoteSceneAction.h"
#include "RemoteSceneCollection.h"
#include "RemoteSceneList.h"
#include "SMCommon.h"

#define SCENE_SERVER "./sceneserver"
#define RESOURCE_TYPE_SCENELIST "oic.wk.sceneList"

constexpr char REMOTE_SCENE_LIST_NAME[] { "Home" };
constexpr int SCENE_RESULT_OK = 200;
const int CALLBACK_WAIT_MIN = 1;
const int CALLBACK_WAIT_MAX = 5;

using namespace std;
using namespace OIC::Service;
using namespace OC;
namespace PH = std::placeholders;

class SMRemoteHelper {
private:
    static std::mutex s_mutex;
    std::shared_ptr<RemoteSceneList> m_pHelperRemoteSceneList;

public:
    RCSRemoteResourceObject::Ptr g_pFoundSceneList;
    RCSRemoteResourceObject::Ptr g_pFoundLightResource;
    RCSRemoteResourceObject::Ptr g_pFoundFanResource;
    std::vector<RCSRemoteResourceObject::Ptr> g_vecFoundResourceList;
    std::unique_ptr<RCSDiscoveryManager::DiscoveryTask> m_pDiscoveryTask;
    const std::string m_relativetUri;
    const std::vector<std::string> m_resourceTypes;

public:
    /**
     * @brief     constructor for SMRemoteHelper class
     */
    SMRemoteHelper();

    /**
     * @brief     function for discovering resources
     */
    void discoverResource();

    /**
     * @brief     callback function to be called when resource is discovered
     */
    void onRemoteResourceDiscovered(
            std::shared_ptr<RCSRemoteResourceObject> foundResource);

    /**
     * @brief     function for stopping resource discovery
     */
    void stopDiscovery();

    /**
     * @brief     function for creating remote scene list instance
     * @return    remote scene list pointer
     */
    std::shared_ptr<RemoteSceneList> createRemoteSceneListInstance();

    /**
     * @brief     callback function to be called when remote scene list is created
     */
    void onRemoteSceneListCreated(RemoteSceneList::Ptr remoteSceneList,
            int eCode);
};
#endif
