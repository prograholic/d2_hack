#include <d2_hack/app/base_b3d_application.h>

#include <OgreEntity.h>

#include <d2_hack/resource/data/b3d_reader.h>
#include <d2_hack/resource/data/b3d_tree_optimization.h>

#include <d2_hack/common/log.h>

#include "room_visitor.h"
#include "car_visitor.h"

namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;

BaseB3dApplication::BaseB3dApplication(const std::string& appName)
    : BaseApplication(appName)
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

bool BaseB3dApplication::keyPressed(const OgreBites::KeyboardEvent& evt)
{
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

    RoomVisitor visitor{ b3dId, roomId, mRoot->getMeshManager(), m_sceneManager, m_ogreMaterialProvider.get() };
    auto visitResult = VisitNode(room, visitor);
    (void)visitResult;

    auto entity = m_sceneManager->createEntity(visitor.GetMesh());
    b3dSceneNode->attachObject(entity);

    for (auto& sceneNode : visitor.GetRootSceneNodes())
    {
        sceneNode->Initialize(b3dSceneNode);
    }

    Trees trees;
    for (const auto& treeData : visitor.GetTreeData())
    {
        auto treeEntity = m_sceneManager->createEntity(treeData.mesh);
        treeEntity->setMaterial(m_ogreMaterialProvider->CreateOrRetrieveMaterial(treeData.materialName, common::DefaultResourceGroup));
        auto treeSceneNode = b3dSceneNode->createChildSceneNode();
        treeSceneNode->attachObject(treeEntity);

        treeSceneNode->pitch(Ogre::Radian(Ogre::Degree(90)), Ogre::Node::TransformSpace::TS_WORLD);
        treeSceneNode->translate(treeData.location);

        for (auto& sceneNode : treeData.rootNodes)
        {
            sceneNode->Initialize(treeSceneNode);
        }

        trees.emplace_back(treeData.name, treeData.rootNodes);
    }

    return std::make_unique<B3dRoom>(roomId, visitor.GetRootSceneNodes(), trees);
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
        return CreateCar(forest.trucks->id, movObjId, moveableObject, moveableSceneNode);
    }
    if (trucks.find(movObjId) != trucks.end())
    {
        return CreateTruck(forest.trucks->id, movObjId, moveableObject, moveableSceneNode);
    }
    if (semiTrailes.find(movObjId) != semiTrailes.end())
    {
        return CreateSemiTrailer(forest.trucks->id, movObjId, moveableObject, moveableSceneNode);
    }
    if (customMoveableObjects.find(movObjId) != customMoveableObjects.end())
    {
        return CreateCustomMoveableObject(forest.trucks->id, moveableObject, moveableSceneNode);
    }

    OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, std::format("Cannot deduce moveable object type for {}", movObjId));
}


static Ogre::Vector3 GetCenterOffset(std::string_view name)
{
    static const std::map<std::string_view, Ogre::Vector3, std::less<>> OffsetForNames
    {
        {"Zil",  Ogre::Vector3(0, 0.205f, 0.955f)},
        {"Kamaz",  Ogre::Vector3(0, 0.135f, 1.185f)},
        {"Freightliner",  Ogre::Vector3(0, 0.105f, 0.865f)},
        {"Scania",  Ogre::Vector3(0, -0.045f, 0.9f)},
        {"Renault",  Ogre::Vector3(0, 0.0600001f, 1.2f)},
        {"Kenworth",  Ogre::Vector3(0, -0.1f, 1.14f)},
        {"Mack",  Ogre::Vector3(0, 0.18f, 1.11f)},
        {"Peterbilt",  Ogre::Vector3(0, 0.03f, 1.33f)},
        {"Daf",  Ogre::Vector3(0, 0.185f, 1.11f)},
        {"Mercedes",  Ogre::Vector3(0, 0.195f, 1.21f)},
        {"Volvo",  Ogre::Vector3(0, 0.1f, 1.13f)},
        {"Storm",  Ogre::Vector3(0, 0, 0.75f)},
        {"International",  Ogre::Vector3(0, 0.95f, 1.26f)},
        {"BmwM5police",  Ogre::Vector3(0, 0.15f, 0.705f)},
        {"BmwM5",  Ogre::Vector3(0, 0.15f, 0.705f)},
        {"Cayman",  Ogre::Vector3(0, 0.02f, 0.38f)},
        {"Offroad",  Ogre::Vector3(0, -0.11f, 0.23f)},
        {"Pickup",  Ogre::Vector3(0, -0.11f, 0.23f)},
        {"Patrol",  Ogre::Vector3(0, 0.1f, 0.735f)},
        {"Gazelle",  Ogre::Vector3(0, -0.005f, 1.0f)},
        {"Gazelle1C",  Ogre::Vector3(0, -0.005f, 1.0f)},
        {"Sobol",  Ogre::Vector3(0, 0.105f, 1.14f)},
        {"RenaultR",  Ogre::Vector3(0, -2.545f, 1.41f)},
        {"KamazR",  Ogre::Vector3(0, -1.48f, 1.26f)},
        {"ScaniaR",  Ogre::Vector3(0, -1.735f, 1.21f)},
        {"ZilR",  Ogre::Vector3(0, -2.4f, 1.11f)},
        {"MercedesR",  Ogre::Vector3(0, -2.2125f, 1.31f)},
        {"VolvoR",  Ogre::Vector3(0, -2.271f, 1.268f)},
        {"DafR",  Ogre::Vector3(0, -1.86f, 1.32f)},
        {"StormR",  Ogre::Vector3(0, 0, 1.325f)},
        {"STrailerP",  Ogre::Vector3(0, 0, 0.05f)},
        {"STrailerT",  Ogre::Vector3(0, -0.92f, 1.15f)},
        {"STrailerM",  Ogre::Vector3(0, 0, 1.31f)},
        {"STrailerStorm",  Ogre::Vector3(0, 0.0250001f, 1.15f)},
        {"Ka50",  Ogre::Vector3(0, -0.525f, 1.0f)},
        {"PBmwM5",  Ogre::Vector3(0, 0.15f, 0.5147f)},
        {"POffroad",  Ogre::Vector3(0, -0.11f, 0.1045f)},
        {"PPickup",  Ogre::Vector3(0, -0.11f, 0.1045f)},
        {"PPatrol",  Ogre::Vector3(0, 0.1f, 0.5745f)},
        {"PGazelle",  Ogre::Vector3(0, -0.005f, 0.8849f)},
        {"PSobol",  Ogre::Vector3(0, 0.105f, 1.0249f)},
        {"PMarera",  Ogre::Vector3(0, 0, 0.6918f)},
        {"PMegan",  Ogre::Vector3(0, -0.0599999f, 0.5922f)},
        {"PMini",  Ogre::Vector3(0, -0.0455999f, 0.5033f)},
        {"POka",  Ogre::Vector3(0, -0.08f, 0.51895f)},
        {"PVan",  Ogre::Vector3(0, -0.00999999f, 0.72395f)},
        {"PBus",  Ogre::Vector3(0, 0.19f, 1.5233f)},
        {"PVolga",  Ogre::Vector3(0, -0.29f, 0.5872f)},
        {"PFiat",  Ogre::Vector3(0, 0.09f, 0.54895f)},
        {"PAvensis",  Ogre::Vector3(0, -0.0549999f, 0.5772f)},
        {"Mini",  Ogre::Vector3(0, -0.0455999f, 0.5928f)},
        {"Marera",  Ogre::Vector3(0, 0, 0.785f)},
        {"Bus",  Ogre::Vector3(0, 0.19f, 1.56f)},
        {"Katok",  Ogre::Vector3(0, -0.02f, 0.82f)},
        {"Megan",  Ogre::Vector3(0, -0.0599999f, 0.67f)},
        {"Oka",  Ogre::Vector3(0, -0.08f, 0.58f)},
        {"Van",  Ogre::Vector3(0, -0.00999999f, 0.78f)},
        {"Avensis",  Ogre::Vector3(0, -0.0549999f, 0.655f)},
        {"Volga",  Ogre::Vector3(0, -0.29f, 0.685f)},
        {"Fiat",  Ogre::Vector3(0, 0.09f, 0.61f)}
    };

    return OffsetForNames.at(name);
}



template <typename ObjectType>
std::unique_ptr<ObjectType> BaseB3dApplication::CreateWheelBasedObject(std::string_view b3dId,
                                                                       std::string_view objectId,
                                                                       const resource::data::b3d::B3dNodePtr& moveableObject,
                                                                       Ogre::SceneNode* moveableSceneNode)
{
    scene_node::SceneNodeBaseList rootNodes;

    WheelBasedMoveableObjectVisitor visitor{ b3dId, objectId, GetCenterOffset(objectId), mRoot->getMeshManager(), m_ogreMaterialProvider.get()};

    auto visitResult = VisitNode(moveableObject, visitor);
    (void)visitResult;

    auto entity = m_sceneManager->createEntity(std::string{objectId}, visitor.GetMesh());
    moveableSceneNode->attachObject(entity);

    for (auto& sceneNode : visitor.GetRootSceneNodes())
    {
        sceneNode->Initialize(moveableSceneNode);
    }

    Wheels wheels;
    for (const auto& wheelData : visitor.GetWheelData())
    {
        auto wheelEntity = m_sceneManager->createEntity(wheelData.mesh);
        auto wheelSceneNode = moveableSceneNode->getParentSceneNode()->createChildSceneNode();
        wheelSceneNode->attachObject(wheelEntity);

        for (const auto& transform : wheelData.transformations)
        {
            wheelSceneNode->rotate(Ogre::Quaternion{ transform.matrix });
            wheelSceneNode->translate(transform.position);
        }

        for (auto& sceneNode : wheelData.rootNodes)
        {
            sceneNode->Initialize(wheelSceneNode);
        }

        wheels.insert(std::make_pair(wheelData.id, Wheel(wheelData.name, wheelData.rootNodes)));
    }

    AttachEntityToBullet(1000.0f, entity, visitor.GetHitBox());

    return std::make_unique<ObjectType>(objectId, visitor.GetRootSceneNodes(), std::move(wheels));
}

B3dCarPtr BaseB3dApplication::CreateCar(std::string_view b3dId, std::string_view carId, const B3dNodePtr& moveableObject, Ogre::SceneNode* moveableSceneNode)
{
    return CreateWheelBasedObject<B3dCar>(b3dId, carId, moveableObject, moveableSceneNode);
}

B3dTruckPtr BaseB3dApplication::CreateTruck(std::string_view b3dId, std::string_view truckId, const B3dNodePtr& moveableObject, Ogre::SceneNode* moveableSceneNode)
{
    return CreateWheelBasedObject<B3dTruck>(b3dId, truckId, moveableObject, moveableSceneNode);
}

B3dSemiTrailerPtr BaseB3dApplication::CreateSemiTrailer(std::string_view b3dId, std::string_view semiTrailerId, const B3dNodePtr& moveableObject, Ogre::SceneNode* moveableSceneNode)
{
    return CreateWheelBasedObject<B3dSemiTrailer>(b3dId, semiTrailerId, moveableObject, moveableSceneNode);
}

MoveableObjectPtr BaseB3dApplication::CreateCustomMoveableObject(std::string_view b3dId, const resource::data::b3d::B3dNodePtr& moveableObject, Ogre::SceneNode* moveableSceneNode)
{
    static const std::string_view ka50 = "Ka50";
    std::string_view name = moveableObject->GetName();
    if (name == "k50")
    {
        // NOTE: all subnodes uses `Ka50` instead of `k50`
        name = ka50;
    }

    MoveableObjectVisitor visitor{b3dId, name, GetCenterOffset(name), mRoot->getMeshManager(), m_ogreMaterialProvider.get()};

    auto visitResult = VisitNode(moveableObject, visitor);
    (void)visitResult;

    auto entity = m_sceneManager->createEntity(std::string{name}, visitor.GetMesh());
    moveableSceneNode->attachObject(entity);


    for (auto& sceneNode : visitor.GetRootSceneNodes())
    {
        sceneNode->Initialize(moveableSceneNode);
    }

    AttachEntityToBullet(5000.0f, entity, visitor.GetHitBox());

    if (name == ka50)
    {
        return std::make_unique<B3dHelicopter>(name, visitor.GetRootSceneNodes());
    }
    else
    {
        assert(moveableObject->GetName() == "Katok");
        return std::make_unique<B3dKatok>("Katok", visitor.GetRootSceneNodes());
    }
}

void BaseB3dApplication::AttachEntityToBullet(float mass, Ogre::Entity* entity, const HitBox& hitBox)
{
    auto mesh = entity->getMesh();

    auto oldBbox = mesh->getBounds();
    auto newBbox = hitBox.GetBoundingBox();

    D2_HACK_LOG(BaseB3dApplication::AttachEntityToBullet) << "\"" << entity->getName() << "\":  Ogre::" << newBbox.getCenter();// << ", old center: " << oldBbox.getCenter();

    mesh->_setBounds(newBbox, false);

    m_dynWorld->addRigidBody(mass, entity, Ogre::Bullet::CT_BOX);

    mesh->_setBounds(oldBbox, false);
}

} // namespace app
} // namespace d2_hack
