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
    assert(m_meshStack.empty());
    assert(m_transformQueue.empty());
    assert(m_currentLods.empty());
    assert(m_sceneNodes.empty());
}


void B3dSceneBuilder::ProcessLod(const block_data::GroupLodParameters10& block, VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        m_currentLods.push(block);
    }
    else
    {
        m_currentLods.pop();
    }
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

Ogre::MeshPtr B3dSceneBuilder::ProcessMesh(const std::string& blockName, VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        std::string name = GetNameImpl(blockName, "mesh", false);
        std::string group = "D2";
        if (m_meshManager->resourceExists(name, group))
        {
            name = GetNameImpl(blockName, "mesh", true);
        }

        D2_HACK_LOG(B3dTreeVisitor::ProcessMesh(PreOrder)) << "Starting processing mesh with name: " << name;

        Ogre::MeshPtr newMesh = m_meshManager->createManual(name, group);
        m_meshStack.push(newMesh);

        return newMesh;
    }
    else
    {
        Ogre::MeshPtr mesh = m_meshStack.top();
        m_meshStack.pop();

        const std::string entityName = mesh->getName() + ".entity";
        Ogre::Entity* entity = m_sceneManager->createEntity(entityName, mesh);

        m_sceneNodes.top()->attachObject(entity);

        D2_HACK_LOG(B3dTreeVisitor::ProcessMesh(PostOrder)) << "Finished processing mesh with name: " << mesh->getName();

        return mesh;
    }
}

void B3dSceneBuilder::AddVertexData(const Ogre::MeshPtr& mesh, const common::PositionWithTexCoordList& data)
{
    std::unique_ptr<Ogre::VertexData> vertexData{ OGRE_NEW Ogre::VertexData };

    vertexData->vertexCount = data.size();
    Ogre::VertexDeclaration* decl = vertexData->vertexDeclaration;
    Ogre::VertexBufferBinding* bind = vertexData->vertexBufferBinding;

    size_t offset = 0;

    decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

    decl->addElement(1, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

    Ogre::AxisAlignedBox bbox = mesh->getBounds();

    for (const auto& vertex : data)
    {
        bbox.merge(vertex.position);
    }

    Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
        offset,
        vertexData->vertexCount,
        Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

    vbuf->writeData(0, vbuf->getSizeInBytes(), data.data(), true);
    
    bind->setBinding(0, vbuf);
    bind->setBinding(1, vbuf);

    mesh->sharedVertexData = vertexData.release();

    mesh->_setBounds(bbox, true);
}

void B3dSceneBuilder::AddVertexData(const Ogre::MeshPtr& mesh, const common::PositionWithNormalList& data)
{
    std::unique_ptr<Ogre::VertexData> vertexData{ OGRE_NEW Ogre::VertexData };

    vertexData->vertexCount = data.size();
    Ogre::VertexDeclaration* decl = vertexData->vertexDeclaration;
    Ogre::VertexBufferBinding* bind = vertexData->vertexBufferBinding;

    size_t offset = 0;

    decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

    decl->addElement(1, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

    Ogre::AxisAlignedBox bbox = mesh->getBounds();

    for (const auto& vertex : data)
    {
        bbox.merge(vertex.position);
    }

    Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
        offset,
        vertexData->vertexCount,
        Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

    vbuf->writeData(0, vbuf->getSizeInBytes(), data.data(), true);
    
    bind->setBinding(0, vbuf);
    bind->setBinding(1, vbuf);

    mesh->sharedVertexData = vertexData.release();

    mesh->_setBounds(bbox, true);
}

void B3dSceneBuilder::AddVertexData(const Ogre::MeshPtr& mesh, const common::PositionWithTexCoordNormalList& data)
{
    std::unique_ptr<Ogre::VertexData> vertexData{ OGRE_NEW Ogre::VertexData };

    vertexData->vertexCount = data.size();
    Ogre::VertexDeclaration* decl = vertexData->vertexDeclaration;
    Ogre::VertexBufferBinding* bind = vertexData->vertexBufferBinding;

    size_t offset = 0;

    decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

    decl->addElement(1, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

    decl->addElement(2, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

    Ogre::AxisAlignedBox bbox = mesh->getBounds();

    for (const auto& vertex : data)
    {
        bbox.merge(vertex.position);
    }

    Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
        offset,
        vertexData->vertexCount,
        Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

    vbuf->writeData(0, vbuf->getSizeInBytes(), data.data(), true);

    bind->setBinding(0, vbuf);
    bind->setBinding(1, vbuf);
    bind->setBinding(2, vbuf);

    mesh->sharedVertexData = vertexData.release();

    mesh->_setBounds(bbox, true);
}

void B3dSceneBuilder::AddVertexData(const Ogre::MeshPtr& mesh, const std::vector<block_data::GroupVertexData37::Unknown514>& data)
{
    std::unique_ptr<Ogre::VertexData> vertexData{ OGRE_NEW Ogre::VertexData };

    vertexData->vertexCount = data.size();
    Ogre::VertexDeclaration* decl = vertexData->vertexDeclaration;
    Ogre::VertexBufferBinding* bind = vertexData->vertexBufferBinding;

    size_t offset = 0;

    decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

    decl->addElement(1, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

    decl->addElement(2, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT4);


    Ogre::AxisAlignedBox bbox = mesh->getBounds();

    for (const auto& vertex : data)
    {
        bbox.merge(vertex.position);
    }

    Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
        offset,
        vertexData->vertexCount,
        Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

    vbuf->writeData(0, vbuf->getSizeInBytes(), data.data(), true);

    bind->setBinding(0, vbuf);
    bind->setBinding(1, vbuf);
    bind->setBinding(2, vbuf);

    mesh->sharedVertexData = vertexData.release();

    mesh->_setBounds(bbox, true);
}


void B3dSceneBuilder::AddVertexData(const Ogre::MeshPtr& mesh, const std::vector<block_data::GroupVertexData37::Unknown258Or515>& data)
{
    std::unique_ptr<Ogre::VertexData> vertexData{ OGRE_NEW Ogre::VertexData };

    vertexData->vertexCount = data.size();
    Ogre::VertexDeclaration* decl = vertexData->vertexDeclaration;
    Ogre::VertexBufferBinding* bind = vertexData->vertexBufferBinding;

    size_t offset = 0;

    decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

    decl->addElement(1, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

    decl->addElement(2, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);


    Ogre::AxisAlignedBox bbox = mesh->getBounds();

    for (const auto& vertex : data)
    {
        bbox.merge(vertex.position);
    }

    Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
        offset,
        vertexData->vertexCount,
        Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

    vbuf->writeData(0, vbuf->getSizeInBytes(), data.data(), true);

    bind->setBinding(0, vbuf);
    bind->setBinding(1, vbuf);
    bind->setBinding(2, vbuf);

    mesh->sharedVertexData = vertexData.release();

    mesh->_setBounds(bbox, true);
}


Ogre::SubMesh* B3dSceneBuilder::CreateSubMesh(std::uint32_t materialIndex)
{
    const std::string materialName = GetMaterialName(materialIndex);

    auto mesh = m_meshStack.top();

    Ogre::SubMesh* subMesh = mesh->createSubMesh();
    subMesh->useSharedVertices = true;
    subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

    subMesh->setMaterialName(materialName);

    D2_HACK_LOG(CreateSubmesh) << "New submesh for mesh " << mesh->getName() << ", material name: " << materialName;

    return subMesh;
}


void B3dSceneBuilder::SetSubMeshData(Ogre::SubMesh* subMesh, const std::vector<block_data::Face28::Unknown>& data, const common::IndexList& indices)
{
    ManageSubMeshIndexBuffer(subMesh, indices);

    (void)data;

    //ResetTexCoords(subMesh, data);
}

void B3dSceneBuilder::SetSubMeshData(Ogre::SubMesh* subMesh, const common::IndexList& /* data */, const common::IndexList& indices)
{
    ManageSubMeshIndexBuffer(subMesh, indices);
}

void B3dSceneBuilder::SetSubMeshData(Ogre::SubMesh* subMesh, const common::IndexWithTexCoordList& data, const common::IndexList& indices)
{
    ManageSubMeshIndexBuffer(subMesh, indices);

    ResetTexCoords(subMesh, data);
}

void B3dSceneBuilder::SetSubMeshData(Ogre::SubMesh* subMesh, const common::IndexWithNormalList& data, const common::IndexList& indices)
{
    ManageSubMeshIndexBuffer(subMesh, indices);

    ResetNormals(subMesh, data);
}

void B3dSceneBuilder::SetSubMeshData(Ogre::SubMesh* subMesh, const common::IndexWithTexCoordNormalList& data, const common::IndexList& indices)
{
    ManageSubMeshIndexBuffer(subMesh, indices);

    ResetTexCoords(subMesh, data);
    ResetNormals(subMesh, data);
}

void B3dSceneBuilder::SetSubMeshData(Ogre::SubMesh* subMesh, const std::vector<block_data::Face35::Unknown49>& /* data */, const common::IndexList& indices)
{
    ManageSubMeshIndexBuffer(subMesh, indices);
}

void B3dSceneBuilder::SetSubMeshData(Ogre::SubMesh* subMesh, const std::vector<block_data::Face8::Unknown177>& /* data */, const common::IndexList& indices)
{
    ManageSubMeshIndexBuffer(subMesh, indices);
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

void B3dSceneBuilder::ManageSubMeshIndexBuffer(Ogre::SubMesh* subMesh, const common::IndexList& indices)
{
    auto ibuf = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
        Ogre::HardwareIndexBuffer::IT_32BIT,
        indices.size(),
        Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

    ibuf->writeData(0, ibuf->getSizeInBytes(), indices.data(), true);

    subMesh->indexData->indexBuffer = ibuf;
    subMesh->indexData->indexCount = ibuf->getNumIndexes();
    subMesh->indexData->indexStart = 0;
}

Ogre::VertexData* B3dSceneBuilder::GetSubMeshNonSharedVertexBuffer(Ogre::SubMesh* subMesh)
{
    if (subMesh->useSharedVertices)
    {
        subMesh->vertexData = m_meshStack.top()->sharedVertexData->clone(true);
        subMesh->useSharedVertices = false;
    }

    return subMesh->vertexData;
}

unsigned short B3dSceneBuilder::ResetElementBySemantic(Ogre::VertexData* vertexData, Ogre::VertexElementSemantic sem, unsigned short index)
{
    auto vertexElement = vertexData->vertexDeclaration->findElementBySemantic(sem, index);
    unsigned short texCoordSource = 0;
    if (vertexElement)
    {
        texCoordSource = vertexElement->getSource();
        vertexData->vertexDeclaration->removeElement(Ogre::VES_TEXTURE_COORDINATES);
    }
    else
    {
        texCoordSource = vertexData->vertexBufferBinding->getNextIndex();
    }

    return texCoordSource;
}

template <typename SubMeshDataList>
void B3dSceneBuilder::ResetTexCoords(Ogre::SubMesh* subMesh, const SubMeshDataList& data)
{
    common::TexCoordList texCoords;
    for (const auto& item : data)
    {
        texCoords.resize(std::max(texCoords.size(), static_cast<size_t>(item.index) + 1));
        texCoords[item.index] = item.texCoord;
    }

    Ogre::VertexData* vertexData = GetSubMeshNonSharedVertexBuffer(subMesh);

    vertexData->vertexCount = texCoords.size();
    Ogre::VertexDeclaration* decl = vertexData->vertexDeclaration;
    Ogre::VertexBufferBinding* bind = vertexData->vertexBufferBinding;

    unsigned short texCoordSource = ResetElementBySemantic(vertexData, Ogre::VES_TEXTURE_COORDINATES);

    size_t offset = 0;

    decl->addElement(texCoordSource, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

    Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
        offset,
        vertexData->vertexCount,
        Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

    vbuf->writeData(0, vbuf->getSizeInBytes(), texCoords.data(), true);

    bind->setBinding(texCoordSource, vbuf);
}


template <typename SubMeshDataList>
void B3dSceneBuilder::ResetNormals(Ogre::SubMesh* subMesh, const SubMeshDataList& data)
{
    common::PositionList normals;
    for (const auto& item : data)
    {
        normals.resize(std::max(normals.size(), static_cast<size_t>(item.index) + 1));
        normals[item.index] = item.normal;
    }

    Ogre::VertexData* vertexData = GetSubMeshNonSharedVertexBuffer(subMesh);

    vertexData->vertexCount = normals.size();
    Ogre::VertexDeclaration* decl = vertexData->vertexDeclaration;
    Ogre::VertexBufferBinding* bind = vertexData->vertexBufferBinding;

    unsigned short normalSource = ResetElementBySemantic(vertexData, Ogre::VES_NORMAL);

    size_t offset = 0;

    decl->addElement(normalSource, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

    Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
        offset,
        vertexData->vertexCount,
        Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

    vbuf->writeData(0, vbuf->getSizeInBytes(), normals.data(), true);

    bind->setBinding(normalSource, vbuf);
}

} // namespace app
} // namespace d2_hack
