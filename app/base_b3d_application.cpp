#include <d2_hack/app/base_b3d_application.h>

#include <d2_hack/common/log.h>
#include <d2_hack/resource/data/b3d_reader.h>
#include <d2_hack/resource/data/b3d_tree_optimization.h>

#include "b3d_scene_builder.h"
#include "b3d_tree_visitor.h"

namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;

BaseB3dApplication::BaseB3dApplication(const std::string& appName)
    : BaseApplication(appName)
    , m_worldContext()
{
}

void BaseB3dApplication::CreateB3dScene(const B3dRegistry& b3dRegistry, Ogre::SceneNode* b3dSceneNode)
{
    PrintNodesStats("start");
    {
        B3dForest b3dForest = ReadB3d(b3dRegistry);
        PrintNodesStats("after_ReadB3d");

        transformation::Transform(b3dForest);
        PrintNodesStats("after_Transform");

        transformation::Optimize(b3dForest);
        PrintNodesStats("after_Optimize");

        CreateRooms(b3dForest, b3dSceneNode);
        PrintNodesStats("after_CreateRoomNodes");

        CreateMoveableObjects(b3dForest, b3dSceneNode);
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

B3dRoomPtr BaseB3dApplication::CreateRoom(const B3dForest& forest, const std::string_view& roomId, Ogre::SceneNode* b3dSceneNode)
{
    B3dNodePtr room;
    std::string_view b3dId;

    for (const auto& tree : forest.forest)
    {
        for (const auto& rootNode : tree->rootNodes)
        {
            if (rootNode->GetName() == roomId)
            {
                room = rootNode;
                b3dId = tree->id;
                break;
            }
        }
    }

    if (!room)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, std::format("Cannot find node by name {}", roomId));
    }

    if (room->GetNodeCategory() != NodeCategory::RoomNode)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, std::format("Incorrect node category {} for {}", static_cast<int>(room->GetNodeCategory()), room->GetName()));
    }
    if (room->GetChildNodeList().empty())
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, std::format("Cannot create empty room {}", room->GetName()));
    }

    scene_node::SceneNodeBaseList rootNodes;

    B3dSceneBuilderContext context{m_sceneManager, b3dSceneNode, mRoot->getMeshManager(), m_ogreMaterialProvider.get()};
    B3dSceneBuilder sceneBuilder{b3dId, context, rootNodes};
    B3dTreeVisitor visitor{sceneBuilder};
    auto visitResult = VisitNode(room, visitor);
    (void)visitResult;

    return std::make_unique<B3dRoom>(std::move(rootNodes));
}

#if 0
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
#endif // 0

MoveableObjectPtr BaseB3dApplication::CreateMoveableObject(const B3dForest& forest, const std::string_view& movObjId, const Ogre::Vector3& location, Ogre::SceneNode* b3dSceneNode)
{
    B3dNodePtr moveableObject;
    for (const auto& rootNode : forest.trucks->rootNodes)
    {
        if (rootNode->GetName() == movObjId)
        {
            moveableObject = rootNode;
            break;
        }
    }

    if (!moveableObject)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, std::format("Cannot find node by name {}", movObjId));
    }

    //if (moveableObject->GetNodeCategory() != NodeCategory::CarNode)
    //{
    //    OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, std::format("Incorrect node category {} for {}", static_cast<int>(moveableObject->GetNodeCategory()), moveableObject->GetName()));
    //}
    if (moveableObject->GetChildNodeList().empty())
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, std::format("Cannot create empty moveable object {}", moveableObject->GetName()));
    }

    static std::set<std::string_view> cars{"Zil"};
    static std::set<std::string_view> trucks{"FreightLiner"};
    static std::set<std::string_view> semiTrailes{"STrailerP"};
    static std::set<std::string_view> customMoveableObjects{ "k50", "Katok"};

    auto moveableSceneNode = b3dSceneNode->createChildSceneNode(location);

    if (cars.find(movObjId) != cars.end())
    {
        return CreateCar(forest.trucks->id, moveableObject, moveableSceneNode);
    }
    if (trucks.find(movObjId) != cars.end())
    {
        return CreateTruck(forest.trucks->id, moveableObject, moveableSceneNode);
    }

    OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, std::format("Cannot deduce moveable object type for {}", movObjId));
}

B3dCarPtr BaseB3dApplication::CreateCar(std::string_view b3dId, const B3dNodePtr& moveableObject, Ogre::SceneNode* moveableSceneNode)
{
    scene_node::SceneNodeBaseList rootNodes;
    std::vector<Wheel> rearWheels;
    Light leftStopLight;
    Light rightStopLight;
    Light leftBackLight;
    Light rightBackLight;
    Wheel leftFrontWheel;
    Wheel rightFrontWheel;
    Light leftFrontLight;
    Light rightFrontLight;

    B3dSceneBuilderContext context{ m_sceneManager, moveableSceneNode, mRoot->getMeshManager(), m_ogreMaterialProvider.get() };

    B3dSceneBuilder builder{ b3dId, context, rootNodes };
    B3dTreeVisitor visitor{ builder };

    auto visitResult = VisitNode(moveableObject, visitor);
    (void)visitResult;

    return std::make_unique<B3dCar>(std::move(rootNodes), std::move(rearWheels), leftStopLight, rightStopLight, leftBackLight, rightBackLight, leftFrontWheel, rightFrontWheel, leftFrontLight, rightFrontLight);
}

B3dTruckPtr BaseB3dApplication::CreateTruck(std::string_view b3dId, const B3dNodePtr& moveableObject, Ogre::SceneNode* moveableSceneNode)
{
    scene_node::SceneNodeBaseList rootNodes;
    std::vector<Wheel> rearWheels;
    Light leftStopLight;
    Light rightStopLight;
    Light leftBackLight;
    Light rightBackLight;
    Wheel leftFrontWheel;
    Wheel rightFrontWheel;
    Light leftFrontLight;
    Light rightFrontLight;

    B3dSceneBuilderContext context{ m_sceneManager, moveableSceneNode, mRoot->getMeshManager(), m_ogreMaterialProvider.get() };

    B3dSceneBuilder builder{ b3dId, context, rootNodes };
    B3dTreeVisitor visitor{ builder };

    auto visitResult = VisitNode(moveableObject, visitor);
    (void)visitResult;

    return std::make_unique<B3dTruck>(std::move(rootNodes), std::move(rearWheels), leftStopLight, rightStopLight, leftBackLight, rightBackLight, leftFrontWheel, rightFrontWheel, leftFrontLight, rightFrontLight);
}

} // namespace app
} // namespace d2_hack
