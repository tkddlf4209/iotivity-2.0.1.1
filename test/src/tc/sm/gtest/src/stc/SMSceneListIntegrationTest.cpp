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

#include "SMHelper.h"

class SMSceneListIntegrationTest_stc: public ::testing::Test {
public:
    SMHelper* m_pSMHelper;
    SceneList* m_pSceneList;
    std::shared_ptr<SceneCollection> m_pSceneCollection;

    SMSceneListIntegrationTest_stc() {
        m_pSMHelper = nullptr;
        m_pSceneList = nullptr;
        m_pSceneCollection = nullptr;
    }

    virtual void SetUp() {
        IOTIVITYTEST_LOG(INFO, "SetUp IN");

        CommonTestUtil::runCommonTCSetUpPart();
        m_pSMHelper = new SMHelper();
    }

    virtual void TearDown() {
        IOTIVITYTEST_LOG(INFO, "TearDown IN");

        delete m_pSMHelper;
        m_pSMHelper = nullptr;

        CommonTestUtil::runCommonTCTearDownPart();
    }
};

/**
 * @since 2016-02-29
 * @see void Configure(const PlatformConfig& config);
 * @see static SceneList* getInstance();
 * @objective Test addNewSceneCollection and getSceneCollections function positively and check
 *             if added scene collection is get
 * @target SceneCollection::Ptr addNewSceneCollection();
 * @target void setName(const std::string& name);
 * @target std::vector<SceneCollection::Ptr> getSceneCollections() const;
 * @target std::string getName() const;
 * @test_data name = "Kitchen"
 * @pre_condition call getInstance method
 * @procedure     1. call addNewSceneCollection and check if scene collection pointer is
 *                         returned
 *                 2. call setName for newly added collection
 *                 3. call getSceneCollections and check if list of scene collections is
 *                         returned
 *                 4. check if added sceneCollection name and get sceneCollection name is same
 * @post_condition none
 * @expected the two scene collection name should be same
 */
#if defined(__LINUX__)
TEST_F(SMSceneListIntegrationTest_stc, AddAndGetSceneCollections_GSRV_P)
{
    m_pSceneList = m_pSMHelper->getSceneListInstance();

    if(m_pSceneList == NULL_PTR)
    {
        SET_FAILURE("scenelist pointer is not found");
        return;
    }

    std::vector<SceneCollection::Ptr> sceneCollections;
    std::shared_ptr<SceneCollection> receivedSceneCollection;

    try
    {
        m_pSceneCollection = m_pSceneList->addNewSceneCollection();
        ASSERT_NE(m_pSceneCollection,NULL_PTR) << "scene collection pointer is not found";

        m_pSceneCollection->setName(SCENE_COLLECTION_NAME);

        sceneCollections = m_pSceneList->getSceneCollections();
        ASSERT_EQ(sceneCollections.size(),INT_ONE) << "list of scene collections is not found";

        receivedSceneCollection = sceneCollections.at(INT_ZERO);
    }
    catch (exception& e)
    {
        SET_FAILURE("Exception occurred: " + std::string(e.what()));
        return;
    }

    if((receivedSceneCollection->getName()) != (m_pSceneCollection->getName()))
    {
        SET_FAILURE("Added scene collection is not get");
    }
}
#endif

/**
 * @since 2016-07-27
 * @see void Configure(const PlatformConfig& config);
 * @see static SceneList* getInstance();
 * @objective Test getSceneCollections function negatively without adding scene collcetions and check
 *             if no scene collections are returned
 * @target std::vector<SceneCollection::Ptr> getSceneCollections() const;
 * @test_data none
 * @pre_condition call getInstance method
 * @procedure call getSceneCollections and check if returned list of scene collections is empty
 * @post_condition none
 * @expected empty list should be returned
 */
#if defined(__LINUX__)
TEST_F(SMSceneListIntegrationTest_stc, GetSceneCollectionsWithoutAddingSceneCollection_DSCC_N)
{
    m_pSceneList = m_pSMHelper->getSceneListInstance();

    if(m_pSceneList == NULL_PTR)
    {
        SET_FAILURE("scenelist pointer is not found");
        return;
    }

    std::vector<SceneCollection::Ptr> sceneCollections;

    try
    {
        sceneCollections = m_pSceneList->getSceneCollections();
        ASSERT_EQ(sceneCollections.size(),INT_ZERO) << "list of scene collections should be empty";
    }
    catch (exception& e)
    {
        SET_FAILURE("Exception occurred inside getSceneCollections: " + std::string(e.what()));
    }
}
#endif

/**
 * @since 2016-02-29
 * @see void Configure(const PlatformConfig& config);
 * @see static SceneList* getInstance();
 * @objective Test setName and getName function positively for checking if setName is get
 * @target void setName(const std::string& sceneListName);
 * @target std::string getName() const;
 * @test_data sceneListName = "Home"
 * @pre_condition call getInstance method
 * @procedure         1. call setName
 *                     2. call getName and check if returned name is same as the set name
 * @post_condition none
 * @expected received name and set name should match and no exceptions occur
 */
#if defined(__LINUX__)
TEST_F(SMSceneListIntegrationTest_stc, SceneListSetAndGetName_GSRV_P)
{
    m_pSceneList = m_pSMHelper->getSceneListInstance();

    if(m_pSceneList == NULL_PTR)
    {
        SET_FAILURE("scenelist pointer is not found");
        return;
    }

    std::string receivedName = "";

    try
    {
        m_pSceneList->setName(SCENE_LIST_NAME);
        receivedName = m_pSceneList->getName();
    }
    catch (exception& e)
    {
        SET_FAILURE("Exception occurred: " + std::string(e.what()));
        return;
    }

    if(receivedName != SCENE_LIST_NAME)
    {
        SET_FAILURE("Get name didn't receive the set name");
    }
}
#endif

/**
 * @since 2016-07-27
 * @see void Configure(const PlatformConfig& config);
 * @see static SceneList* getInstance();
 * @objective Test getName function negatively without setting name and
 *     check if getName returns default scenelist name
 * @target std::string getName() const;
 * @test_data none
 * @pre_condition call getInstance method
 * @procedure call getName and check if no name is returned
 * @post_condition none
 * @expected default scenelist name is returned and no exceptions occur
 */
#if defined(__LINUX__)
TEST_F(SMSceneListIntegrationTest_stc, SceneListGetNameWithoutSettingName_DSCC_N)
{
    m_pSceneList = m_pSMHelper->getSceneListInstance();

    if(m_pSceneList == NULL_PTR)
    {
        SET_FAILURE("scenelist pointer is not found");
        return;
    }

    std::string receivedName = "";

    try
    {
        receivedName = m_pSceneList->getName();
    }
    catch (exception& e)
    {
        SET_FAILURE("Exception occurred inside getName: " + std::string(e.what()));
        return;
    }

    const std::string defaultSceneListName = "list of scene Collections";
    if(receivedName != defaultSceneListName)
    {
        SET_FAILURE("getName should return default name");
    }
}
#endif
