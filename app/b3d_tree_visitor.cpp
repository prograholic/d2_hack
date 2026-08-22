#include "b3d_tree_visitor.h"

#include <OgreMesh.h>
#include <OgreSubMesh.h>

#include <d2_hack/scene_node/scene_node_base.h>
#include <d2_hack/scene_node/switchable_scene_nodes.h>
#include <d2_hack/scene_node/collision_scene_nodes.h>
#include <d2_hack/scene_node/generator_scene_nodes.h>
#include <d2_hack/scene_node/renderable_scene_nodes.h>

#include <d2_hack/common/utils.h>
#include <d2_hack/common/log.h>
#include <d2_hack/common/resource_mgmt.h>
#include <d2_hack/common/numeric_conversion.h>

//#define B3D_NOT_IMPLEMENTED() D2_HACK_LOG("") << __FUNCSIG__ << ": NOT IMPLEMENTED"
#define B3D_NOT_IMPLEMENTED()


namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;


B3dTreeVisitor::B3dTreeVisitor(B3dSceneBuilder& sceneBuilder)
    : m_sceneBuilder(sceneBuilder)
{
}

B3dSceneBuilder& B3dTreeVisitor::GetSceneBuilder()
{
    return m_sceneBuilder;
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeEventEntry& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* /* sceneNode */)
{
    return CreateSceneNode<scene_node::EventEntrySceneNode>(parent, node.GetName());
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupUnknown2& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* /* sceneNode */)
{
    return CreateSceneNode<scene_node::GroupUnknown2>(parent, node.GetName(), node.GetBlockData());
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupRoadInfraObjects4& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::OgreSceneNode<NodeGroupRoadInfraObjects4::Value>>(parent, node.GetName(), sceneNode);
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupObjects5& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    D2_HACK_LOG(B3dTreeVisitor) << node.GetName();

    return CreateSceneNode<scene_node::OgreSceneNode<NodeGroupObjects5::Value>>(parent, node.GetName(), sceneNode);
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupVertexData7& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::GroupVertexData7>(parent, node.GetName(), sceneNode);
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleFaces8& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    auto res = CreateSceneNode<scene_node::OgreSceneNode<NodeSimpleFaces8::Value>>(parent, node.GetName(), sceneNode);

    VisitFaces(node);

    return res;
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupTrigger9& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* /* sceneNode */)
{
    return CreateSceneNode<scene_node::GroupTrigger9>(parent, node.GetName(), node.GetBlockData());
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupLodParameters10& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::OgreSceneNode<NodeGroupLodParameters10::Value>>(parent, node.GetName(), sceneNode);
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupUnknown12& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::OgreSceneNode<NodeGroupUnknown12::Value>>(parent, node.GetName(), sceneNode);
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleTrigger13& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::OgreSceneNode<NodeSimpleTrigger13::Value>>(parent, node.GetName(), sceneNode);
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleUnknown14& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::OgreSceneNode<NodeSimpleUnknown14::Value>>(parent, node.GetName(), sceneNode);
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleObjectConnector18& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    auto res = CreateSceneNode<scene_node::OgreSceneNode<NodeSimpleObjectConnector18::Value>>(parent, node.GetName(), sceneNode);

    GetSceneBuilder().ProcessObjectConnector(node);

    return res;
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupObjects19& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::OgreSceneNode<NodeGroupObjects19::Value>>(parent, node.GetName(), sceneNode);
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleFlatCollision20& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* /* sceneNode */)
{
    return CreateSceneNode<scene_node::SimpleFlatCollision20>(parent, node.GetName(), node.GetBlockData());
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupObjects21& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* /* sceneNode */)
{
    return CreateSceneNode<scene_node::SceneNodeEvent21>(parent, node.GetName(), node.GetBlockData());
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleVolumeCollision23& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* /* sceneNode */)
{
    return CreateSceneNode<scene_node::SimpleVolumeCollision23>(parent, node.GetName(), node.GetBlockData());
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleUnknown25& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::OgreSceneNode<NodeSimpleUnknown25::Value>>(parent, node.GetName(), sceneNode);
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleFaces28& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    auto res = CreateSceneNode<scene_node::OgreSceneNode<NodeSimpleFaces28::Value>>(parent, node.GetName(), sceneNode);

    VisitFaces(node);

    return res;
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupUnknown29& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* /* sceneNode */)
{
    return CreateSceneNode<scene_node::GroupUnknown29>(parent, node.GetName(), node.GetBlockData());
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimplePortal30& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::OgreSceneNode<NodeSimplePortal30::Value>>(parent, node.GetName(), sceneNode);
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupLightingObjects33& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::OgreSceneNode<NodeGroupLightingObjects33::Value>>(parent, node.GetName(), sceneNode);
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleUnknown34& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::OgreSceneNode<NodeSimpleUnknown34::Value>>(parent, node.GetName(), sceneNode);
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleFaces35& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    auto res = CreateSceneNode<scene_node::OgreSceneNode<NodeSimpleFaces35::Value>>(parent, node.GetName(), sceneNode);

    VisitFaces(node);

    return res;
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupVertexData37& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::GroupVertexData37>(parent, node.GetName(), sceneNode);
}


scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleGeneratedObjects40& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    auto ogreMaterialProvider = GetSceneBuilder().GetOgreMaterialProvider();
    auto generatorName = common::ResourceNameToStringView(node.GetBlockData().name);
    if (generatorName == "$$GeneratorOfTerrain")
    {
        auto newName = std::format("terrain.{}", node.GetName());
        return CreateSceneNode<scene_node::TerrainSceneNode>(parent, newName, sceneNode, GetSceneBuilder().GetSceneManager());
    }
    else if (generatorName == "$$TreeGenerator1")
    {
        auto b3dId = GetSceneBuilder().GetB3dId();
        Ogre::SceneManager* sceneManager = GetSceneBuilder().GetSceneManager();
        auto newName = std::format("tree.{}", node.GetName());
        return CreateSceneNode<scene_node::TreeGeneratorSceneNode>(parent, newName, node.GetBlockData(), b3dId, sceneNode, sceneManager, ogreMaterialProvider);
    }
    else
    {
        D2_HACK_LOG(B3dTreeVisitor::CreateNode) << "skipping unsupported generator: " << generatorName;
        return CreateSceneNode<scene_node::OgreSceneNode<NodeSimpleGeneratedObjects40::Value>>(parent, node.GetName(), sceneNode);
    }
}


/////////////////////////////////////////////////////////////////////////////////////


template <typename FacesNode>
void B3dTreeVisitor::VisitFaces(FacesNode& node)
{
    const auto& block = node.GetBlockData();
    for (const auto& face : block.faces)
    {
        m_sceneBuilder.CreateMesh(node.GetName(), face.meshInfo, node.GetOriginalRoot()->GetMaterialNameByIndex(face.materialIndex));
    }
}


///////////////////////////////////////////////////////////////////////////////////////






WheelBasedMoveableObjectVisitor::WheelBasedMoveableObjectVisitor(std::string_view b3dId,
                                                                 std::string_view blockName,
                                                                 Ogre::MeshManager* meshManager,
                                                                 resource::archive::res::OgreMaterialProvider* ogreMaterialProvider)
    : m_b3dId(b3dId)
    , m_blockName(blockName)
    , m_mesh(CreateMesh(b3dId, blockName, meshManager))
    , m_ogreMaterialProvider(ogreMaterialProvider)
    , m_meshManager(meshManager)
{
}

VisitResult WheelBasedMoveableObjectVisitor::Visit(const std::shared_ptr<NodeEventEntry>& node, VisitMode visitMode)
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

VisitResult WheelBasedMoveableObjectVisitor::Visit(const std::shared_ptr<resource::data::b3d::NodeGroupUnknown2>& /* node */, VisitMode /* visitMode */)
{
    return VisitResult::Continue;
}

VisitResult WheelBasedMoveableObjectVisitor::Visit(const std::shared_ptr<NodeGroupObjects5>& node, VisitMode visitMode)
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

VisitResult WheelBasedMoveableObjectVisitor::Visit(const std::shared_ptr<NodeSimpleFaces8>& node, VisitMode visitMode)
{
    return VisitFaces(*node, visitMode);
}

VisitResult WheelBasedMoveableObjectVisitor::Visit(const std::shared_ptr<NodeSimpleUnknown14>& /* node */, VisitMode /* visitMode */)
{
    return VisitResult::Continue;
}

VisitResult WheelBasedMoveableObjectVisitor::Visit(const std::shared_ptr<NodeSimpleObjectConnector18>& node, VisitMode visitMode)
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

VisitResult WheelBasedMoveableObjectVisitor::Visit(const std::shared_ptr<NodeGroupObjects21>& node, VisitMode visitMode)
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

VisitResult WheelBasedMoveableObjectVisitor::Visit(const std::shared_ptr<NodeSimpleVolumeCollision23>& /* node */, VisitMode /* visitMode */)
{
    return VisitResult::Continue;
}

VisitResult WheelBasedMoveableObjectVisitor::Visit(const std::shared_ptr<NodeSimpleUnknown25>& /* node */, VisitMode /* visitMode */)
{
    return VisitResult::Continue;
}

VisitResult WheelBasedMoveableObjectVisitor::Visit(const std::shared_ptr<NodeSimpleFaces28>& node, VisitMode visitMode)
{
    return VisitFaces(*node, visitMode);
}

VisitResult WheelBasedMoveableObjectVisitor::Visit(const std::shared_ptr<NodeGroupLightingObjects33>& /* node */, VisitMode /* visitMode */)
{
    D2_HACK_LOG(ZilVisitor::VisitNodeGroupLightingObjects33) << "not implemented";

    return VisitResult::Continue;
}

VisitResult WheelBasedMoveableObjectVisitor::Visit(const std::shared_ptr<NodeSimpleFaces35>& node, VisitMode visitMode)
{
    return VisitFaces(*node, visitMode);
}

const scene_node::SceneNodeBaseList& WheelBasedMoveableObjectVisitor::GetRootSceneNodes() const
{
    return m_rootSceneNodes;
}

Ogre::MeshPtr WheelBasedMoveableObjectVisitor::GetMesh() const
{
    return m_mesh;
}

const std::vector<WheelData>& WheelBasedMoveableObjectVisitor::GetWheelData() const
{
    return m_wheelRootSceneNodes;
}

void WheelBasedMoveableObjectVisitor::PushToSceneNodeStack(const scene_node::SceneNodeBasePtr& node)
{
    if (m_sceneNodesStack.empty())
    {
        m_rootSceneNodes.push_back(node);
    }

    m_sceneNodesStack.push(node);
}

void WheelBasedMoveableObjectVisitor::PopFromSceneNodeStack()
{
    m_sceneNodesStack.pop();
}

scene_node::SceneNodeBasePtr WheelBasedMoveableObjectVisitor::GetParentSceneNode()
{
    return m_sceneNodesStack.empty() ? scene_node::SceneNodeBasePtr{} : m_sceneNodesStack.top();
}

Ogre::MeshPtr WheelBasedMoveableObjectVisitor::CreateMesh(std::string_view b3dId, std::string_view blockName, Ogre::MeshManager* meshManager)
{
    const std::string meshName = common::GetMeshName(b3dId, blockName);
    return meshManager->createManual(meshName, common::DefaultResourceGroup);
}

size_t WheelBasedMoveableObjectVisitor::CreateSubMesh(const Ogre::MaterialPtr& material)
{
    size_t subMeshId = m_mesh->getNumSubMeshes();
    Ogre::SubMesh* subMesh = m_mesh->createSubMesh();
    subMesh->useSharedVertices = false;
    subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

    subMesh->setMaterial(material);

    //D2_HACK_LOG(CreateSubmesh) << "New submesh for mesh " << mesh->getName() << ", material name: " << materialName;

    return subMeshId;
}

size_t WheelBasedMoveableObjectVisitor::AddSimpleMeshInfo(const common::SimpleMeshInfo& meshInfo, const std::string& materialName)
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

void WheelBasedMoveableObjectVisitor::ManagePositions(Ogre::VertexData* vertexData, const common::PositionList& positions, unsigned short bufferIndex)
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

void WheelBasedMoveableObjectVisitor::ManageTexCoords(Ogre::VertexData* vertexData, const common::TexCoordList& texCoords, unsigned short bufferIndex)
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

void WheelBasedMoveableObjectVisitor::ManageNormals(Ogre::VertexData* vertexData, const common::NormalList& normals, unsigned short bufferIndex)
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

void WheelBasedMoveableObjectVisitor::ApplyTransformations(const common::PositionList& original, common::PositionList& transformed)
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
VisitResult WheelBasedMoveableObjectVisitor::VisitFaces(FacesType& node, VisitMode visitMode)
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

} // namespace app
} // namespace d2_hack
