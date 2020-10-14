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

        D2_HACK_LOG(B3dTreeVisitor::ProcessMesh(PostOrder)) << "Starting processing mesh with name: " << name;

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

    decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
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

    decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
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

    decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

    decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
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

    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
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

    mesh->sharedVertexData = vertexData.release();

    mesh->_setBounds(bbox, true);
}


void B3dSceneBuilder::AddVertexData(const Ogre::MeshPtr& mesh, const std::vector<block_data::GroupVertexData37::Unknown258>& data)
{
    std::unique_ptr<Ogre::VertexData> vertexData{ OGRE_NEW Ogre::VertexData };

    vertexData->vertexCount = data.size();
    Ogre::VertexDeclaration* decl = vertexData->vertexDeclaration;
    Ogre::VertexBufferBinding* bind = vertexData->vertexBufferBinding;

    size_t offset = 0;

    decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT1);


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

    mesh->sharedVertexData = vertexData.release();

    mesh->_setBounds(bbox, true);
}


Ogre::SubMesh* B3dSceneBuilder::CreateSubMesh(const SubMeshInfo& subMeshInfo)
{
    const std::string materialName = GetMaterialName(subMeshInfo.materialIndex);

    auto mesh = m_meshStack.top();

    Ogre::SubMesh* subMesh = mesh->createSubMesh();
    subMesh->useSharedVertices = subMeshInfo.useSharedVertices;
    subMesh->operationType = subMeshInfo.operationType;

    subMesh->setMaterialName(materialName);

    D2_HACK_LOG(CreateSubmesh) << "New submesh for mesh " << mesh->getName() << ", material name: " << materialName;

    return subMesh;
}


void B3dSceneBuilder::SetSubMeshData(Ogre::SubMesh* subMesh, const std::vector<block_data::Face28::Unknown>& data)
{
    common::TexCoordList texCoords;
    for (const auto& item : data)
    {
        texCoords.push_back(item.texCoord);
    }

    {
        std::unique_ptr<Ogre::VertexData> vertexData{ OGRE_NEW Ogre::VertexData };

        vertexData->vertexCount = texCoords.size();
        Ogre::VertexDeclaration* decl = vertexData->vertexDeclaration;
        Ogre::VertexBufferBinding* bind = vertexData->vertexBufferBinding;

        size_t offset = 0;

        decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
        offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

        Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
            offset,
            vertexData->vertexCount,
            Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

        vbuf->writeData(0, vbuf->getSizeInBytes(), texCoords.data(), true);
        bind->setBinding(0, vbuf);

        subMesh->vertexData = vertexData.release();
    }
}

void B3dSceneBuilder::SetSubMeshData(Ogre::SubMesh* subMesh, const common::IndexList& data)
{
    ManageSubMeshIndexBuffer(subMesh, data);
}

void B3dSceneBuilder::SetSubMeshData(Ogre::SubMesh* subMesh, const common::IndexWithTexCoordList& data)
{
    common::IndexList indices;
    common::TexCoordList texCoords;
    for (const auto& item : data)
    {
        indices.push_back(item.index);
        texCoords.push_back(item.texCoord);
    }

    ManageSubMeshIndexBuffer(subMesh, indices);

    {
        std::unique_ptr<Ogre::VertexData> vertexData{ OGRE_NEW Ogre::VertexData };

        vertexData->vertexCount = texCoords.size();
        Ogre::VertexDeclaration* decl = vertexData->vertexDeclaration;
        Ogre::VertexBufferBinding* bind = vertexData->vertexBufferBinding;

        size_t offset = 0;

        decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
        offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

        Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
            offset,
            vertexData->vertexCount,
            Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

        vbuf->writeData(0, vbuf->getSizeInBytes(), texCoords.data(), true);
        bind->setBinding(0, vbuf);

        subMesh->vertexData = vertexData.release();
    }
}

void B3dSceneBuilder::SetSubMeshData(Ogre::SubMesh* subMesh, const common::IndexWithPositionList& data)
{
    common::IndexList indices;
    common::PositionList positions;
    for (const auto& item : data)
    {
        indices.push_back(item.index);
        positions.push_back(item.position);
    }

    ManageSubMeshIndexBuffer(subMesh, indices);

    {
        std::unique_ptr<Ogre::VertexData> vertexData{ OGRE_NEW Ogre::VertexData };

        vertexData->vertexCount = positions.size();
        Ogre::VertexDeclaration* decl = vertexData->vertexDeclaration;
        Ogre::VertexBufferBinding* bind = vertexData->vertexBufferBinding;

        size_t offset = 0;

        decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
        offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

        Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
            offset,
            vertexData->vertexCount,
            Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

        vbuf->writeData(0, vbuf->getSizeInBytes(), positions.data(), true);
        bind->setBinding(0, vbuf);

        subMesh->vertexData = vertexData.release();
    }
}

void B3dSceneBuilder::SetSubMeshData(Ogre::SubMesh* subMesh, const common::IndexWithPositionTexCoordList& data)
{
    common::IndexList indices;
    common::PositionWithTexCoordList positions;
    for (const auto& item : data)
    {
        indices.push_back(item.index);
        positions.push_back({ item.position, item.texCoord });
    }

    ManageSubMeshIndexBuffer(subMesh, indices);

    {
        std::unique_ptr<Ogre::VertexData> vertexData{ OGRE_NEW Ogre::VertexData };

        vertexData->vertexCount = positions.size();
        Ogre::VertexDeclaration* decl = vertexData->vertexDeclaration;
        Ogre::VertexBufferBinding* bind = vertexData->vertexBufferBinding;

        size_t offset = 0;

        decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
        offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

        decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
        offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

        Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
            offset,
            vertexData->vertexCount,
            Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

        vbuf->writeData(0, vbuf->getSizeInBytes(), positions.data(), true);
        bind->setBinding(0, vbuf);

        subMesh->vertexData = vertexData.release();
    }
}

void B3dSceneBuilder::SetSubMeshData(Ogre::SubMesh* subMesh, const std::vector<block_data::Face35::Unknown49>& data)
{
    common::IndexList indices;
    for (const auto& item : data)
    {
        indices.push_back(item.index);
    }

    ManageSubMeshIndexBuffer(subMesh, indices);
}

void B3dSceneBuilder::ManageSubMeshIndexBuffer(Ogre::SubMesh* subMesh, const common::IndexList& indices)
{
    size_t prevSize = subMesh->indexData->indexBuffer ? subMesh->indexData->indexBuffer->getNumIndexes() : 0;
    size_t prevSizeInBytes = subMesh->indexData->indexBuffer ? subMesh->indexData->indexBuffer->getSizeInBytes() : 0;

    auto ibuf = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
        Ogre::HardwareIndexBuffer::IT_32BIT,
        prevSize + indices.size(),
        Ogre::HardwareBuffer::HBU_STATIC);

    std::vector<std::uint8_t> buff;
    buff.resize(ibuf->getSizeInBytes());

    if (prevSize && prevSizeInBytes)
    {
        subMesh->indexData->indexBuffer->readData(0, prevSizeInBytes, buff.data());
    }

    size_t restDataSize = ibuf->getSizeInBytes() - prevSizeInBytes;
    std::memcpy(buff.data() + prevSizeInBytes, indices.data(), restDataSize);

    ibuf->writeData(0, ibuf->getSizeInBytes(), buff.data(), true);

    subMesh->indexData->indexBuffer = ibuf;
    subMesh->indexData->indexCount = ibuf->getNumIndexes();
    subMesh->indexData->indexStart = 0;
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


} // namespace app
} // namespace d2_hack
