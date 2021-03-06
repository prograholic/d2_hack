#include "b3d_scene_builder.h"

D2_HACK_DISABLE_WARNING_BEGIN(4100)
D2_HACK_DISABLE_WARNING_BEGIN(4275)
D2_HACK_DISABLE_WARNING_BEGIN(4251)

#include <OgreHardwareBufferManager.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreMesh.h>
#include <OgreSubMesh.h>
#include <OgreMeshManager.h>
#include <OgreEntity.h>

D2_HACK_DISABLE_WARNING_END() // 4251
D2_HACK_DISABLE_WARNING_END() // 4275
D2_HACK_DISABLE_WARNING_END() // 4100


#include <d2_hack/common/resource_mgmt.h>
#include <d2_hack/common/utils.h>
#include <d2_hack/common/log.h>

//#define B3D_NOT_IMPLEMENTED() D2_HACK_LOG("") << __FUNCSIG__ << ": NOT IMPLEMENTED"
#define B3D_NOT_IMPLEMENTED()


namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;

B3dSceneBuilder::B3dSceneBuilder(const char* b3dId,
                                 const std::string& b3dName,
                                 Ogre::SceneManager* sceneManager,
                                 Ogre::SceneNode* rootNode,
                                 Ogre::MeshManager* meshManager,
                                 const common::Materials& materials)
    : m_b3dId(b3dId)
    , m_b3dName(b3dName)
    , m_sceneManager(sceneManager)
    , m_rootNode(rootNode)
    , m_meshManager(meshManager)
    , m_materials(materials)
{
}


B3dSceneBuilder::~B3dSceneBuilder()
{
    assert(m_transformQueue.empty());
    assert(m_sceneNodes.empty());
}

void B3dSceneBuilder::ProcessTransformQueue(const std::string& name, const block_data::GroupTransformMatrix24& block, VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        m_transformQueue.push_back(block);
    }
    else
    {
        TransformList transformList;
        for (const auto& item : m_transformQueue)
        {
            Transform transform;

            transform.matrix.FromAxes(item.x, item.y, item.z);
            transform.position = item.position;

            transformList.push_back(transform);
        }

        m_transformMap[name] = transformList;
        m_transformQueue.pop_back();
    }
}

void B3dSceneBuilder::ProcessLight(const std::string& name, const block_data::GroupLightingObjects33& block, VisitMode visitMode)
{
    Ogre::SceneNode* sceneNode = ProcessSceneNode(name, visitMode);
    if (visitMode == VisitMode::PreOrder)
    {
        std::string full_name = GetNameImpl(name, "light", false);
        if (m_sceneManager->hasLight(full_name))
        {
            full_name = GetNameImpl(name, "light", true);
        }

        Ogre::Light* light = m_sceneManager->createLight(full_name);
        Ogre::SceneNode* lightNode = m_sceneManager->createSceneNode(full_name + "_scene_node");
        lightNode->attachObject(light);
        lightNode->setPosition(block.position);
        sceneNode->addChild(lightNode);

        // TODO: setup light
        B3D_NOT_IMPLEMENTED();
    }
}


void B3dSceneBuilder::ProcessObjectConnector(const block_data::SimpleObjectConnector18& block, VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        std::string sceneName = GetNameImpl(common::ResourceNameToString(block.object), "scene_node", false);

        Ogre::SceneNode* parentSceneNode = m_sceneNodes.top();

        Ogre::SceneNode* sceneNode = m_sceneManager->getSceneNode(sceneName, false);
        if (sceneNode)
        {
            sceneNode->getParentSceneNode()->removeChild(sceneNode);
            parentSceneNode->addChild(sceneNode);
            auto transformList = m_transformMap[common::ResourceNameToString(block.space)];
            for (const auto& transform : transformList)
            {
                sceneNode->translate(transform.position);
                sceneNode->rotate(Ogre::Quaternion{transform.matrix});
            }
        }
    }
}

Ogre::SceneNode* B3dSceneBuilder::ProcessSceneNode(const std::string& name, VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        std::string full_name = GetNameImpl(name, "scene_node", false);
        if (m_sceneManager->hasSceneNode(full_name))
        {
            full_name = GetNameImpl(name, "scene_node", true);
        }

        Ogre::SceneNode* sceneNode = m_sceneManager->createSceneNode(full_name);
        Ogre::SceneNode* parent = m_sceneNodes.empty() ? m_rootNode : m_sceneNodes.top();
        parent->addChild(sceneNode);

        m_sceneNodes.push(sceneNode);

        D2_HACK_LOG(B3dTreeVisitor::ProcessSceneNode(PreOrder)) << "name: " << name << ", parent: " << parent->getName() << ", scene nodes count: " << m_sceneNodes.size();

        return sceneNode;
    }
    else
    {
        Ogre::SceneNode* sceneNode = m_sceneNodes.top();
        m_sceneNodes.pop();

        return sceneNode;
    }
}

void B3dSceneBuilder::CreateMesh(const std::string& blockName, const common::SimpleMeshInfo& meshInfo, std::uint32_t materialIndex)
{
    std::string name = GetNameImpl(blockName, "mesh", false);
    std::string group = "D2";
    if (m_meshManager->resourceExists(name, group))
    {
        name = GetNameImpl(blockName, "mesh", true);
    }

    D2_HACK_LOG(B3dTreeVisitor::CreateMesh) << "Starting processing mesh with name: " << name;

    Ogre::MeshPtr mesh = m_meshManager->createManual(name, group);

    SetMeshInfo(mesh, meshInfo, materialIndex);

    const std::string entityName = mesh->getName() + ".entity";
    Ogre::Entity* entity = m_sceneManager->createEntity(entityName, mesh);

    m_sceneNodes.top()->attachObject(entity);
}

std::string B3dSceneBuilder::GetB3dResourceId(const std::string& name) const
{
    return common::GetResourceName(m_b3dId, name);
}

std::string B3dSceneBuilder::GetB3dResourceId(const common::ResourceName& name) const
{
    return GetB3dResourceId(common::ResourceNameToString(name));
}

std::string B3dSceneBuilder::GetMaterialName(const std::uint32_t materialIndex) const
{
    return GetB3dResourceId(common::ResourceNameToString(m_materials[materialIndex]));
}

std::string B3dSceneBuilder::GetNameImpl(const std::string& blockName, const std::string& subName, bool forceUnique) const
{
    std::string name = GetB3dResourceId(blockName) + "_" + subName;

    if (forceUnique)
    {
        static int cnt = 0;
        name += ("_" + std::to_string(cnt));
        ++cnt;
    }

    return name;
}

Ogre::SubMesh* B3dSceneBuilder::CreateSubMesh(const Ogre::MeshPtr& mesh, std::uint32_t materialIndex)
{
    const std::string materialName = GetMaterialName(materialIndex);

    Ogre::SubMesh* subMesh = mesh->createSubMesh();
    subMesh->useSharedVertices = true;
    subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

    subMesh->setMaterialName(materialName);

    D2_HACK_LOG(CreateSubmesh) << "New submesh for mesh " << mesh->getName() << ", material name: " << materialName;

    return subMesh;
}

void B3dSceneBuilder::SetMeshInfo(const Ogre::MeshPtr& mesh, const common::SimpleMeshInfo& meshInfo, std::uint32_t materialIndex)
{
    mesh->sharedVertexData = new Ogre::VertexData{};

    CreateSubMesh(mesh, materialIndex);

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
    vertexData->vertexCount = positions.size();
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
