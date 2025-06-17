#include <d2_hack/app/base_b3d_application.h>

#include <d2_hack/common/log.h>
#include <d2_hack/resource/data/b3d_reader.h>
#include <d2_hack/resource/data/b3d_tree_optimization.h>

namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;

BaseB3dApplication::BaseB3dApplication(const std::string& appName)
    : BaseApplication(appName)
    , m_cars()
    , m_rooms()
    , m_worldContext()
{
}

void BaseB3dApplication::CreateB3dScene(const B3dRegistry& b3dRegistry, const CarNameList& carNames, Ogre::SceneNode* b3dSceneNode)
{
    PrintNodesStats("start");

    {
        B3dForest b3dForest = ReadB3d(b3dRegistry);
        PrintNodesStats("after_ReadB3d");

        transformation::Transform(b3dForest);
        PrintNodesStats("after_Transform");

        transformation::Optimize(b3dForest);
        PrintNodesStats("after_Optimize");

        for (auto& tree : b3dForest.forest)
        {
            CreateRoomNodes(*tree, b3dSceneNode);
        }
        PrintNodesStats("after_CreateRoomNodes");

        CreateCarNodes(*b3dForest.trucks, carNames, b3dSceneNode);
        PrintNodesStats("after_CreateCarNodes");
    }

    PrintNodesStats("end");

    b3dSceneNode->pitch(Ogre::Radian(Ogre::Degree(-90)), Ogre::Node::TransformSpace::TS_WORLD);
}

void BaseB3dApplication::ProcessCameraMovement()
{
    Ogre::Vector3f currentPlayerPosition = m_cameraSceneNode->_getDerivedPosition();
    Ogre::Vector3f movement = currentPlayerPosition - m_worldContext.playerPosition;
    m_worldContext.playerPosition = currentPlayerPosition;

    if (movement != Ogre::Vector3f::ZERO)
    {
        OnCameraMoved(m_worldContext, movement);
    }
}

bool BaseB3dApplication::keyPressed(const OgreBites::KeyboardEvent& evt)
{
    ProcessCameraMovement();

    if (evt.keysym.sym == '=')
    {
        m_cameraManager->setTopSpeed(m_cameraManager->getTopSpeed() * 2);
    }
    else if (evt.keysym.sym == '-')
    {
        m_cameraManager->setTopSpeed(m_cameraManager->getTopSpeed() / 2);
    }

    return BaseApplication::keyPressed(evt);
}

void BaseB3dApplication::PrintNodesStats(const char* prefix)
{
    static int callCount = 0;
    callCount += 1;
    D2_HACK_LOG(PrintNodesStats) << "NodeBase(" << callCount << ", " << prefix << "): " << common::NodeBase::GetNodeBaseCount();
    D2_HACK_LOG(PrintNodesStats) << "B3dNode(" << callCount << ", " << prefix << "): " << B3dNode::GetB3dNodeCount();
    D2_HACK_LOG(PrintNodesStats) << "SceneNode(" << callCount << ", " << prefix << "): " << scene_node::SceneNodeBase::GetSceneNodeBaseCount();
}

void BaseB3dApplication::CreateRoomNodes(const B3dTree& tree, Ogre::SceneNode* b3dSceneNode)
{
    for (const auto& rootNode : tree.rootNodes)
    {
        if (rootNode->GetNodeCategory() == NodeCategory::RoomNode)
        {
            if (!rootNode->GetChildNodeList().empty())
            {
                m_rooms.emplace_back(std::make_unique<B3dRoom>(rootNode, tree.id, m_sceneManager, b3dSceneNode, mRoot->getMeshManager(), m_ogreMaterialProvider.get()));
            }
            else
            {
                D2_HACK_LOG(CreateRootNodes) << "Skipping empty room: `" << rootNode->GetName() << "`";
            }
        }
        else
        {
            D2_HACK_LOG(CreateRootNodes) << "Skipping uncategorized root node: `" << rootNode->GetName() << "`";
        }
    }

}

void BaseB3dApplication::CreateCarNodes(const B3dTree& tree, const CarNameList& carNames, Ogre::SceneNode* b3dSceneNode)
{
    for (size_t i = 0; i != carNames.size(); ++i)
    {
        CreateCarNode(tree, carNames[i], Ogre::Vector3{ 3.5f * i, 0, 0 }, b3dSceneNode);
    }
}

void BaseB3dApplication::OnCameraMoved(const scene_node::WorldContext& worldContext, const Ogre::Vector3f& movement)
{
    for (const auto& room : m_rooms)
    {
        room->OnCameraMoved(worldContext, movement);
    }

    for (const auto& car : m_cars)
    {
        car->OnCameraMoved(worldContext, movement);
    }
}

void BaseB3dApplication::CreateCarNode(const B3dTree& tree, const std::string_view& carName, const Ogre::Vector3& location, Ogre::SceneNode* b3dSceneNode)
{
    for (const auto& rootNode : tree.rootNodes)
    {
        if (rootNode->GetName() == carName)
        {
            auto carSceneNode = b3dSceneNode->createChildSceneNode(location);
            m_cars.emplace_back(std::make_unique<B3dCar>(rootNode, tree.id, m_sceneManager, carSceneNode, mRoot->getMeshManager(), m_ogreMaterialProvider.get()));
            break;
        }
    }
}

} // namespace app
} // namespace d2_hack
