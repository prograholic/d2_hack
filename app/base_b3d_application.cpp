#include <d2_hack/app/base_b3d_application.h>

#include <d2_hack/common/log.h>
#include <d2_hack/common/resource_mgmt.h>
#include <d2_hack/common/numeric_conversion.h>
#include <d2_hack/resource/data/b3d_reader.h>
#include <d2_hack/resource/data/b3d_tree_optimization.h>
#include <d2_hack/scene_node/switchable_scene_nodes.h>

#include <OgreMesh.h>
#include <OgreSubMesh.h>
#include <OgreHardwareBufferManager.h>
#include <OgreEntity.h>

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

struct WheelData
{
    scene_node::SceneNodeBaseList rootNodes;
    Ogre::MeshPtr mesh;
    std::string name;
};


class WheelBasedMoveableObjectVisitor : public RaiseExceptionVisitor
{
public:
    WheelBasedMoveableObjectVisitor(std::string_view b3dId, std::string_view blockName, Ogre::MeshManager* meshManager, resource::archive::res::OgreMaterialProvider* ogreMaterialProvider)
        : m_b3dId(b3dId)
        , m_blockName(blockName)
        , m_mesh(CreateMesh(b3dId, blockName, meshManager))
        , m_ogreMaterialProvider(ogreMaterialProvider)
        , m_meshManager(meshManager)
    {
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeEventEntry>& node, VisitMode visitMode) override
    {
        if (visitMode == VisitMode::PreOrder)
        {
            auto parentB3dSceneNode = GetParentSceneNode();
            auto b3dSceneNode = CreateSceneNode<scene_node::EventEntrySceneNode>(parentB3dSceneNode, node->GetName());
            PushToSceneNodeStack(b3dSceneNode);
        }
        else
        {
            PopFromSceneNodeStack();
        }

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupObjects5>& node, VisitMode visitMode) override
    {
        if (visitMode == VisitMode::PreOrder)
        {
            std::string wheelName = std::string{ m_blockName } + "wheel";
            if (node->GetName().starts_with(wheelName))
            {
                WheelBasedMoveableObjectVisitor wheelVisitor{ m_b3dId, node->GetName(), m_meshManager, m_ogreMaterialProvider };

                auto visitResult = VisitNode(node, wheelVisitor);
                (void)visitResult;

                WheelData wheelData;
                wheelData.rootNodes = wheelVisitor.GetRootSceneNodes();
                wheelData.mesh = wheelVisitor.GetMesh();
                wheelData.name = node->GetName();

                m_wheelRootSceneNodes.push_back(std::move(wheelData));

                return VisitResult::SkipChildren;
            }
            else
            {
                return VisitResult::Continue;
            }
        }
        else
        {
            return VisitResult::Continue;
        }
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleFaces8>& node, VisitMode visitMode) override
    {
        return VisitFaces(*node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleUnknown14>& /* node */, VisitMode /* visitMode */) override
    {
        return VisitResult::Continue;
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleObjectConnector18>& node, VisitMode visitMode) override
    {
        if (visitMode == VisitMode::PreOrder)
        {
            m_transformEntries.push_back(node->GetBlockData().transformation);
        }
        else
        {
            assert(visitMode == VisitMode::PostOrder);
            m_transformEntries.pop_back();
        }
        return VisitResult::Continue;
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupObjects21>& node, VisitMode visitMode) override
    {
        if (visitMode == VisitMode::PreOrder)
        {
            auto parentB3dSceneNode = GetParentSceneNode();
            auto b3dSceneNode = CreateSceneNode<scene_node::SceneNodeEvent21>(parentB3dSceneNode, node->GetName(), node->GetBlockData());
            PushToSceneNodeStack(b3dSceneNode);
        }
        else
        {
            PopFromSceneNodeStack();
        }

        return VisitResult::Continue;
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleVolumeCollision23>& /* node */, VisitMode /* visitMode */) override
    {
        return VisitResult::Continue;
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleFaces28>& node, VisitMode visitMode) override
    {
        return VisitFaces(*node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupLightingObjects33>& /* node */, VisitMode /* visitMode */) override
    {
        D2_HACK_LOG(ZilVisitor::VisitNodeGroupLightingObjects33) << "not implemented";
        
        return VisitResult::Continue;
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleFaces35>& node, VisitMode visitMode) override
    {
        return VisitFaces(*node, visitMode);
    }

    const scene_node::SceneNodeBaseList& GetRootSceneNodes() const
    {
        return m_rootSceneNodes;
    }

    Ogre::MeshPtr GetMesh() const
    {
        return m_mesh;
    }

    const std::vector<WheelData>& GetWheelData() const
    {
        return m_wheelRootSceneNodes;
    }

private:
    const std::string_view m_b3dId;
    const std::string_view m_blockName;

    resource::archive::res::OgreMaterialProvider* m_ogreMaterialProvider;
    Ogre::MeshManager* m_meshManager;
    
    Ogre::MeshPtr m_mesh;
    scene_node::SceneNodeBaseList m_rootSceneNodes;

    std::vector<WheelData> m_wheelRootSceneNodes;
    
    std::stack<scene_node::SceneNodeBasePtr> m_sceneNodesStack;
    std::vector<TransformList> m_transformEntries;

    void PushToSceneNodeStack(const scene_node::SceneNodeBasePtr& node)
    {
        if (m_sceneNodesStack.empty())
        {
            m_rootSceneNodes.push_back(node);
        }

        m_sceneNodesStack.push(node);
    }

    void PopFromSceneNodeStack()
    {
        m_sceneNodesStack.pop();
    }

    scene_node::SceneNodeBasePtr GetParentSceneNode()
    {
        return m_sceneNodesStack.empty() ? scene_node::SceneNodeBasePtr{} : m_sceneNodesStack.top();
    }

    static Ogre::MeshPtr CreateMesh(std::string_view b3dId, std::string_view blockName, Ogre::MeshManager* meshManager)
    {
        const std::string meshName = common::GetMeshName(b3dId, blockName);
        return meshManager->createManual(meshName, common::DefaultResourceGroup);
    }

    size_t CreateSubMesh(const Ogre::MaterialPtr& material)
    {
        size_t subMeshId = m_mesh->getNumSubMeshes();
        Ogre::SubMesh* subMesh = m_mesh->createSubMesh();
        subMesh->useSharedVertices = false;
        subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

        subMesh->setMaterial(material);

        //D2_HACK_LOG(CreateSubmesh) << "New submesh for mesh " << mesh->getName() << ", material name: " << materialName;

        return subMeshId;
    }

    size_t AddSimpleMeshInfo(const common::SimpleMeshInfo& meshInfo, const std::string& materialName)
    {
        auto material = m_ogreMaterialProvider->CreateOrRetrieveMaterial(materialName, common::DefaultResourceGroup);
        size_t subMeshId = CreateSubMesh(material);

        auto subMesh = m_mesh->getSubMesh(subMeshId);
        subMesh->vertexData = new Ogre::VertexData{};

        unsigned short bufferIndex = 0;
        if (!meshInfo.positions.empty())
        {
            common::PositionList transformedPositions;
            ApplyTransformations(meshInfo.positions, transformedPositions);

            ManagePositions(subMesh->vertexData, transformedPositions, bufferIndex);
            bufferIndex += 1;

            Ogre::AxisAlignedBox bbox = m_mesh->getBounds();
            for (const auto& position : transformedPositions)
            {
                bbox.merge(position);
            }
            m_mesh->_setBounds(bbox, true);
        }

        if (!meshInfo.texCoords.empty())
        {
            ManageTexCoords(subMesh->vertexData, meshInfo.texCoords, bufferIndex);
            bufferIndex += 1;
        }

        if (!meshInfo.normals.empty())
        {
            ManageNormals(subMesh->vertexData, meshInfo.normals, bufferIndex);
            bufferIndex += 1;
        }

        return subMeshId;
    }

    void ManagePositions(Ogre::VertexData* vertexData, const common::PositionList& positions, unsigned short bufferIndex)
    {
        vertexData->vertexCount = common::NumericCast<std::uint32_t>(positions.size());
        Ogre::VertexDeclaration* decl = vertexData->vertexDeclaration;
        Ogre::VertexBufferBinding* bind = vertexData->vertexBufferBinding;

        size_t offset = 0;
        decl->addElement(bufferIndex, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
        offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

        Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
            offset,
            vertexData->vertexCount,
            Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

        vbuf->writeData(0, vbuf->getSizeInBytes(), positions.data(), true);

        bind->setBinding(bufferIndex, vbuf);
    }

    void ManageTexCoords(Ogre::VertexData* vertexData, const common::TexCoordList& texCoords, unsigned short bufferIndex)
    {
        Ogre::VertexDeclaration* decl = vertexData->vertexDeclaration;
        Ogre::VertexBufferBinding* bind = vertexData->vertexBufferBinding;

        size_t offset = 0;
        decl->addElement(bufferIndex, 0, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
        offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

        Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
            offset,
            vertexData->vertexCount,
            Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

        vbuf->writeData(0, vbuf->getSizeInBytes(), texCoords.data(), true);

        bind->setBinding(bufferIndex, vbuf);
    }

    void ManageNormals(Ogre::VertexData* vertexData, const common::NormalList& normals, unsigned short bufferIndex)
    {
        Ogre::VertexDeclaration* decl = vertexData->vertexDeclaration;
        Ogre::VertexBufferBinding* bind = vertexData->vertexBufferBinding;

        size_t offset = 0;
        decl->addElement(bufferIndex, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
        offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

        Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
            offset,
            vertexData->vertexCount,
            Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

        vbuf->writeData(0, vbuf->getSizeInBytes(), normals.data(), true);

        bind->setBinding(bufferIndex, vbuf);
    }

    void ApplyTransformations(const common::PositionList& original, common::PositionList& transformed)
    {
        transformed = original;
        for (auto& position : transformed)
        {
            for (const auto& transformList : m_transformEntries)
            {
                for (const auto& transformEntry : transformList)
                {
                    Ogre::Matrix4 fullTransform = Ogre::Matrix4::IDENTITY;
                    fullTransform.set3x3Matrix(transformEntry.matrix);
                    fullTransform.setTrans(transformEntry.position);
                    position = fullTransform * position;
                }
            }
        }
    }

    template <typename FacesType>
    VisitResult VisitFaces(FacesType& node, VisitMode visitMode)
    {
        if (visitMode == VisitMode::PreOrder)
        {
            std::vector<size_t> subEntityIds;

            const auto& block = node.GetBlockData();
            for (const auto& face : block.faces)
            {
                auto subEntityId = AddSimpleMeshInfo(face.meshInfo, node.GetOriginalRoot()->GetMaterialNameByIndex(face.materialIndex));
                subEntityIds.push_back(subEntityId);
            }

            auto parentB3dSceneNode = GetParentSceneNode();
            auto b3dSceneNode = CreateSceneNode<scene_node::SubEntitiesSceneNode>(parentB3dSceneNode, node.GetName(), std::move(subEntityIds));
            PushToSceneNodeStack(b3dSceneNode);
        }
        else
        {
            PopFromSceneNodeStack();
        }

        return VisitResult::Continue;
    }
};


template <typename ObjectType>
std::unique_ptr<ObjectType> BaseB3dApplication::CreateWheelBasedObject(std::string_view b3dId,
                                                                       std::string_view objectId,
                                                                       const resource::data::b3d::B3dNodePtr& moveableObject,
                                                                       Ogre::SceneNode* moveableSceneNode)
{
    scene_node::SceneNodeBaseList rootNodes;

    WheelBasedMoveableObjectVisitor visitor{ b3dId, objectId, mRoot->getMeshManager(), m_ogreMaterialProvider.get() };

    auto visitResult = VisitNode(moveableObject, visitor);
    (void)visitResult;

    auto entity = m_sceneManager->createEntity(visitor.GetMesh());
    moveableSceneNode->attachObject(entity);

    for (auto& sceneNode : visitor.GetRootSceneNodes())
    {
        sceneNode->Initialize(entity);
    }

    Wheels wheels;
    for (const auto& wheelData : visitor.GetWheelData())
    {
        auto wheelEntity = m_sceneManager->createEntity(wheelData.mesh);
        auto wheelSceneNode = moveableSceneNode->createChildSceneNode();
        wheelSceneNode->attachObject(wheelEntity);

        for (auto& sceneNode : wheelData.rootNodes)
        {
            sceneNode->Initialize(wheelEntity);
        }

        wheels.insert(std::make_pair(wheelData.name, Wheel(wheelData.rootNodes)));
    }

    return std::make_unique<ObjectType>(visitor.GetRootSceneNodes(), std::move(wheels));
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
