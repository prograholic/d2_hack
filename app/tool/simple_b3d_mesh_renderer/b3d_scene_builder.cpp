#include "b3d_scene_builder.h"

#include <d2_hack/common/log.h>
#include <d2_hack/common/utils.h>
#include <d2_hack/common/resource_mgmt.h>
#include <d2_hack/common/numeric_conversion.h>

//#define B3D_NOT_IMPLEMENTED() D2_HACK_LOG("") << __FUNCSIG__ << ": NOT IMPLEMENTED"
#define B3D_NOT_IMPLEMENTED()


namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;
using namespace resource::archive::res;


B3dSceneBuilder::B3dSceneBuilder(const std::string& b3dId,
                                 Ogre::SceneManager* sceneManager,
                                 Ogre::SceneNode* ogreRootNode,
                                 Ogre::MeshManager* meshManager,
                                 OgreMaterialProvider* ogreMaterialProvider,
                                 scene_node::SceneNodeBaseList& rootSceneNodes)
    : m_b3dId(b3dId)
    , m_sceneManager(sceneManager)
    , m_ogreRootNode(ogreRootNode)
    , m_meshManager(meshManager)
    , m_ogreMaterialProvider(ogreMaterialProvider)
    , m_rootSceneNodes(rootSceneNodes)
{
}


B3dSceneBuilder::~B3dSceneBuilder()
{
    assert(m_ogreSceneNodes.empty());
    assert(m_sceneNodesStack.empty());
}

std::string B3dSceneBuilder::GetB3dId() const
{
    return m_b3dId;
}

Ogre::SceneManager* B3dSceneBuilder::GetSceneManager() const
{
    return m_sceneManager;
}

Ogre::MeshManager* B3dSceneBuilder::GetMeshManager() const
{
    return m_meshManager;
}

Ogre::SceneNode* B3dSceneBuilder::GetCurrentOgreSceneNode() const
{
    return m_ogreSceneNodes.empty() ? m_ogreRootNode : m_ogreSceneNodes.top();
}

resource::archive::res::OgreMaterialProvider* B3dSceneBuilder::GetOgreMaterialProvider() const
{
    return m_ogreMaterialProvider;
}

void B3dSceneBuilder::ProcessLight(const resource::data::b3d::NodeGroupLightingObjects33& node, VisitMode visitMode)
{
    // TODO: Ogre::SceneNode already created???
    Ogre::SceneNode* ogreSceneNode = ProcessOgreSceneNode(node.GetName(), visitMode);
    if (visitMode == VisitMode::PreOrder)
    {
        std::string full_name = GetNameImpl(node.GetName(), "light", false);
        if (m_sceneManager->hasLight(full_name))
        {
            full_name = GetNameImpl(node.GetName(), "light", true);
        }

        Ogre::Light* light = m_sceneManager->createLight(full_name);
        Ogre::SceneNode* lightNode = m_sceneManager->createSceneNode(full_name + "_scene_node");
        lightNode->attachObject(light);
        lightNode->setPosition(node.GetBlockData().position);
        ogreSceneNode->addChild(lightNode);

        // TODO: setup light
        B3D_NOT_IMPLEMENTED();
    }
}


void B3dSceneBuilder::ProcessObjectConnector(const resource::data::b3d::NodeSimpleObjectConnector18& node)
{
    Ogre::SceneNode* ogreSceneNode = m_ogreSceneNodes.empty() ? m_ogreRootNode : m_ogreSceneNodes.top();

    for (const auto& transform : node.GetBlockData().transformation)
    {
        ogreSceneNode->rotate(Ogre::Quaternion{ transform.matrix });
        ogreSceneNode->translate(transform.position);
    }
}

void B3dSceneBuilder::ProcessObjectConnector(const resource::data::b3d::NodeSimpleObjectConnector1& /* node */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

Ogre::SceneNode* B3dSceneBuilder::ProcessOgreSceneNode(const std::string& name, VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        std::string full_name = GetNameImpl(name, "scene_node", false);
        if (m_sceneManager->hasSceneNode(full_name))
        {
            full_name = GetNameImpl(name, "scene_node", true);
        }

        Ogre::SceneNode* ogreSceneNode = m_sceneManager->createSceneNode(full_name);
        Ogre::SceneNode* parent = m_ogreSceneNodes.empty() ? m_ogreRootNode : m_ogreSceneNodes.top();
        parent->addChild(ogreSceneNode);

        m_ogreSceneNodes.push(ogreSceneNode);

        //D2_HACK_LOG(B3dTreeVisitor::ProcessSceneNode(PreOrder)) << "name: " << name << ", parent: " << parent->getName() << ", scene nodes count: " << m_sceneNodes.size();

        return ogreSceneNode;
    }
    else
    {
        Ogre::SceneNode* ogreSceneNode = m_ogreSceneNodes.top();
        m_ogreSceneNodes.pop();

        return ogreSceneNode;
    }
}

void B3dSceneBuilder::CreateMesh(const std::string& blockName, const common::SimpleMeshInfo& meshInfo, const std::string& materialName)
{
    const std::string meshName = GetNameImpl(blockName + "." + materialName, "mesh", false);
    const std::string entityName = meshName + ".entity";

    Ogre::MeshPtr mesh;
    if (m_meshManager->resourceExists(meshName, common::DefaultResourceGroup))
    {
        D2_HACK_LOG(B3dTreeVisitor::CreateMesh) << "Mesh `" << meshName << "` already exists, reuse it";
        mesh = m_meshManager->getByName(meshName, common::DefaultResourceGroup);
    }
    else
    {
        mesh = m_meshManager->createManual(meshName, common::DefaultResourceGroup);

        auto material = m_ogreMaterialProvider->CreateOrRetrieveMaterial(materialName, common::DefaultResourceGroup);
        SetMeshInfo(mesh, meshInfo, material);
    }

    Ogre::Entity* entity = m_sceneManager->createEntity(mesh);

    m_ogreSceneNodes.top()->createChildSceneNode()->attachObject(entity);
}

scene_node::SceneNodeBasePtr B3dSceneBuilder::GetParentSceneNode()
{
    return m_sceneNodesStack.empty() ? scene_node::SceneNodeBasePtr{} : m_sceneNodesStack.top();
}

void B3dSceneBuilder::PushToSceneNodeStack(const scene_node::SceneNodeBasePtr& node)
{
    if (m_sceneNodesStack.empty())
    {
        m_rootSceneNodes.push_back(node);
    }

    m_sceneNodesStack.push(node);
}

void B3dSceneBuilder::PopFromSceneNodeStack()
{
    m_sceneNodesStack.pop();
}


/////////////////////////////////////////////////////////////


std::string B3dSceneBuilder::GetB3dResourceId(const std::string& name) const
{
    return common::GetResourceName(m_b3dId, name);
}

std::string B3dSceneBuilder::GetB3dResourceId(const common::ResourceName& name) const
{
    return GetB3dResourceId(common::ResourceNameToString(name));
}

std::string B3dSceneBuilder::GetNameImpl(const std::string& blockName, const std::string& subName, bool forceUnique) const
{
    std::string name = GetB3dResourceId(blockName) + "_" + subName;

    if (forceUnique)
    {
        auto counter = common::GetNextUnnamedObjectCounter();
        name += ("_" + std::to_string(counter));
    }

    return name;
}

Ogre::SubMesh* B3dSceneBuilder::CreateSubMesh(const Ogre::MeshPtr& mesh, const Ogre::MaterialPtr& material)
{
    Ogre::SubMesh* subMesh = mesh->createSubMesh();
    subMesh->useSharedVertices = true;
    subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

    subMesh->setMaterial(material);

    //D2_HACK_LOG(CreateSubmesh) << "New submesh for mesh " << mesh->getName() << ", material name: " << materialName;

    return subMesh;
}

void B3dSceneBuilder::SetMeshInfo(const Ogre::MeshPtr& mesh, const common::SimpleMeshInfo& meshInfo, const Ogre::MaterialPtr& material)
{
    mesh->sharedVertexData = new Ogre::VertexData{};

    CreateSubMesh(mesh, material);

    unsigned short bufferIndex = 0;
    if (!meshInfo.positions.empty())
    {
        ManagePositions(mesh->sharedVertexData, meshInfo.positions, bufferIndex);
        bufferIndex += 1;

        Ogre::AxisAlignedBox bbox = mesh->getBounds();
        for (const auto& position : meshInfo.positions)
        {
            bbox.merge(position);
        }
        mesh->_setBounds(bbox, true);
    }

    if (!meshInfo.texCoords.empty())
    {
        ManageTexCoords(mesh->sharedVertexData, meshInfo.texCoords, bufferIndex);
        bufferIndex += 1;
    }

    if (!meshInfo.normals.empty())
    {
        ManageNormals(mesh->sharedVertexData, meshInfo.normals, bufferIndex);
        bufferIndex += 1;
    }
}

void B3dSceneBuilder::ManagePositions(Ogre::VertexData* vertexData, const common::PositionList& positions, unsigned short bufferIndex)
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

void B3dSceneBuilder::ManageTexCoords(Ogre::VertexData* vertexData, const common::TexCoordList& texCoords, unsigned short bufferIndex)
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

void B3dSceneBuilder::ManageNormals(Ogre::VertexData* vertexData, const common::NormalList& normals, unsigned short bufferIndex)
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

} // namespace app
} // namespace d2_hack
