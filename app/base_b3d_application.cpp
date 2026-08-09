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
    if (moveableObject->GetChildNodeList().empty())
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, std::format("Cannot create empty moveable object {}", moveableObject->GetName()));
    }

    static const std::set<std::string_view> cars
    {
        "BmwM5police",
        "BmwM5",
        "Cayman",
        "Offroad",
        "Pickup",
        "Patrol",
        "Gazelle",
        "Gazelle1C",
        "Sobol",
        "RenaultR",
        "KamazR",
        "ScaniaR",
        "ZilR",
        "MercedesR",
        "VolvoR",
        "DafR",
        "StormR",
        "PBmwM5",
        "POffroad",
        "PPickup",
        "PPatrol",
        "PGazelle",
        "PSobol",
        "PMarera",
        "PMegan",
        "PMini",
        "POka",
        "PVan",
        "PBus",
        "PVolga",
        "PFiat",
        "PAvensis",
        "Mini",
        "Marera",
        "Bus",
        "Megan",
        "Oka",
        "Van",
        "Avensis",
        "Volga",
        "Fiat",
    };
    static const std::set<std::string_view> trucks
    {
        "Zil",
        "Kamaz",
        "Freightliner",
        "Scania",
        "Renault",
        "Kenworth",
        "Mack",
        "Peterbilt",
        "Daf",
        "Mercedes",
        "Volvo",
        "Storm",
        "International"
    };
    static const std::set<std::string_view> semiTrailes
    {
        "STrailerP",
        "STrailerT",
        "STrailerM",
        "STrailerStorm",
    };
    static const std::set<std::string_view> customMoveableObjects
    {
        "k50",
        "Katok"
    };

    auto moveableSceneNode = b3dSceneNode->createChildSceneNode(location);

    if (cars.find(movObjId) != cars.end())
    {
        return CreateCar(forest.trucks->id, moveableObject, moveableSceneNode);
    }
    if (trucks.find(movObjId) != trucks.end())
    {
        return CreateTruck(forest.trucks->id, moveableObject, moveableSceneNode);
    }
    if (semiTrailes.find(movObjId) != semiTrailes.end())
    {
        return CreateSemiTrailer(forest.trucks->id, moveableObject, moveableSceneNode);
    }
    if (customMoveableObjects.find(movObjId) != customMoveableObjects.end())
    {
        return CreateCustomMoveableObject(forest.trucks->id, moveableObject, moveableSceneNode);
    }

    OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, std::format("Cannot deduce moveable object type for {}", movObjId));
}


class WheelBasedMovableObjectObserver : public SceneNodeObserver
{
public:

    virtual void OnSceneNode(scene_node::SceneNodeBasePtr node) override
    {
#define MANAGE_WHEEL(id) \
        if (node->GetName().ends_with("wheel" #id))\
        { \
            m_wheels["wheel" #id] = GetWheel(node, "wheel" #id); \
        }

        MANAGE_WHEEL(0);
        MANAGE_WHEEL(1);
        MANAGE_WHEEL(2);
        MANAGE_WHEEL(3);
        MANAGE_WHEEL(4);
        MANAGE_WHEEL(5);
        MANAGE_WHEEL(6);
        MANAGE_WHEEL(7);

#undef MANAGE_WHEEL



    }

    Wheels GetWheels()
    {
        return m_wheels;
    }

    Light GetLeftStopLight()
    {
        return GetLight(m_leftStopLight, "left stop");
    }

    Light GetRightStopLight()
    {
        return GetLight(m_rightStopLight, "right stop");
    }

    Light GetLeftBackLight()
    {
        return GetLight(m_leftBackLight, "left back");
    }

    Light GetRightBackLight()
    {
        return GetLight(m_rightBackLight, "right back");
    }

    static Light GetLight(scene_node::SceneNodeBasePtr lightNode, const char* /* nodeName   */)
    {
        //if (!lightNode)
        //{
        //    OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, std::format("Cannot get light for {}", nodeName));
        //}

        return Light{ lightNode };
    }

    static Wheel GetWheel(scene_node::SceneNodeBasePtr wheelNode, const char* nodeName)
    {
        if (!wheelNode)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, std::format("Cannot get wheel for {}", nodeName));
        }

        return Wheel{ wheelNode };
    }

private:
    Wheels m_wheels;
    scene_node::SceneNodeBasePtr m_leftStopLight;
    scene_node::SceneNodeBasePtr m_rightStopLight;
    scene_node::SceneNodeBasePtr m_leftBackLight;
    scene_node::SceneNodeBasePtr m_rightBackLight;


};

class CarBaseObserver : public WheelBasedMovableObjectObserver
{
public:
    virtual void OnSceneNode(scene_node::SceneNodeBasePtr node) override
    {
        WheelBasedMovableObjectObserver::OnSceneNode(node);

        if (node->GetName() == "refer_HeadLightKey")
        {
            m_leftFrontLight = node;
            m_rightFrontLight = node;
        }
    }

    Light GetLeftFrontLight()
    {
        return GetLight(m_leftFrontLight, "left front");
    }
        
    Light GetRightFrontLight()
    {
        return GetLight(m_rightFrontLight, "right front");
    }


private:
    scene_node::SceneNodeBasePtr m_leftFrontLight;
    scene_node::SceneNodeBasePtr m_rightFrontLight;
};

B3dCarPtr BaseB3dApplication::CreateCar(std::string_view b3dId, const B3dNodePtr& moveableObject, Ogre::SceneNode* moveableSceneNode)
{
    scene_node::SceneNodeBaseList rootNodes;

    B3dSceneBuilderContext context{ m_sceneManager, moveableSceneNode, mRoot->getMeshManager(), m_ogreMaterialProvider.get() };
    CarBaseObserver observer;
    B3dSceneBuilder builder{ b3dId, context, rootNodes, &observer };
    B3dTreeVisitor visitor{ builder };

    auto visitResult = VisitNode(moveableObject, visitor);
    (void)visitResult;

    return std::make_unique<B3dCar>(
        std::move(rootNodes),
        std::move(observer.GetWheels()),
        observer.GetLeftStopLight(),
        observer.GetRightStopLight(),
        observer.GetLeftBackLight(),
        observer.GetRightBackLight(),
        observer.GetLeftFrontLight(),
        observer.GetRightFrontLight());
}

B3dTruckPtr BaseB3dApplication::CreateTruck(std::string_view b3dId, const B3dNodePtr& moveableObject, Ogre::SceneNode* moveableSceneNode)
{
    scene_node::SceneNodeBaseList rootNodes;

    B3dSceneBuilderContext context{ m_sceneManager, moveableSceneNode, mRoot->getMeshManager(), m_ogreMaterialProvider.get() };
    CarBaseObserver observer;
    B3dSceneBuilder builder{ b3dId, context, rootNodes, &observer };
    B3dTreeVisitor visitor{ builder };

    auto visitResult = VisitNode(moveableObject, visitor);
    (void)visitResult;

    return std::make_unique<B3dTruck>(
        std::move(rootNodes),
        std::move(observer.GetWheels()),
        observer.GetLeftStopLight(),
        observer.GetRightStopLight(),
        observer.GetLeftBackLight(),
        observer.GetRightBackLight(),
        observer.GetLeftFrontLight(),
        observer.GetRightFrontLight());
}

B3dSemiTrailerPtr BaseB3dApplication::CreateSemiTrailer(std::string_view b3dId, const B3dNodePtr& moveableObject, Ogre::SceneNode* moveableSceneNode)
{
    scene_node::SceneNodeBaseList rootNodes;

    B3dSceneBuilderContext context{ m_sceneManager, moveableSceneNode, mRoot->getMeshManager(), m_ogreMaterialProvider.get() };
    WheelBasedMovableObjectObserver observer;
    B3dSceneBuilder builder{ b3dId, context, rootNodes, &observer };
    B3dTreeVisitor visitor{ builder };

    auto visitResult = VisitNode(moveableObject, visitor);
    (void)visitResult;

    return std::make_unique<B3dSemiTrailer>(
        std::move(rootNodes),
        std::move(observer.GetWheels()),
        observer.GetLeftStopLight(),
        observer.GetRightStopLight(),
        observer.GetLeftBackLight(),
        observer.GetRightBackLight());
}

MoveableObjectPtr BaseB3dApplication::CreateCustomMoveableObject(std::string_view b3dId, const resource::data::b3d::B3dNodePtr& moveableObject, Ogre::SceneNode* moveableSceneNode)
{
    scene_node::SceneNodeBaseList rootNodes;

    B3dSceneBuilderContext context{ m_sceneManager, moveableSceneNode, mRoot->getMeshManager(), m_ogreMaterialProvider.get() };

    B3dSceneBuilder builder{ b3dId, context, rootNodes };
    B3dTreeVisitor visitor{ builder };

    auto visitResult = VisitNode(moveableObject, visitor);
    (void)visitResult;

    if (moveableObject->GetName() == "k50")
    {
        return std::make_unique<B3dHelicopter>(std::move(rootNodes));
    }
    else
    {
        assert(moveableObject->GetName() == "Katok");
        return std::make_unique<B3dKatok>(std::move(rootNodes));
    }
}

} // namespace app
} // namespace d2_hack
