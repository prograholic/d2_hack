#include "b3d_listener.h"

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
#include <d2_hack/common/log.h>

//#define B3D_NOT_IMPLEMENTED() D2_HACK_LOG("") << __FUNCSIG__ << ": NOT IMPLEMENTED"
#define B3D_NOT_IMPLEMENTED()


namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;

std::string GetResourceName(const common::ResourceName& resName)
{
    const char* data = reinterpret_cast<const char*>(resName.data());
    return std::string(data, data + strnlen(data, resName.size()));
}


B3dMeshListener::B3dMeshListener(const char* b3dId,
                                 const std::string& b3dName,
                                 Ogre::SceneManager* sceneManager,
                                 Ogre::SceneNode* rootNode,
                                 Ogre::MeshManager* meshManager)
    : m_b3dId(b3dId)
    , m_b3dName(b3dName)
    , m_sceneManager(sceneManager)
    , m_rootNode(rootNode)
    , m_meshManager(meshManager)

{
}

B3dMeshListener::~B3dMeshListener()
{
    assert(m_meshQueue.empty());
    assert(m_transformQueue.empty());
    assert(m_blockNames.empty());
    assert(m_currentLods.empty());
    assert(m_sceneNodes.empty());
    assert(m_data35TypeQueue.empty());
}

void B3dMeshListener::OnMaterials(Materials&& materials)
{
    m_materials = std::move(materials);
}

///////////////////////////////////////////////////////////////////////////

void B3dMeshListener::OnBlockBegin(const block_data::BlockHeader& blockHeader)
{
    m_blockNames.push(GetB3dResourceId(blockHeader.name));
}

void B3dMeshListener::OnBlockEnd(const block_data::BlockHeader& blockHeader)
{
    switch (blockHeader.type)
    {
    case block_data::EmptyBlock0:
        // no need to implement
        break;

    case block_data::GroupRoadInfraObjectsBlock4:
        EndSceneNode();
        break;

    case block_data::GroupObjectsBlock5:
        EndSceneNode();
        break;

    case block_data::GroupVertexBlock7:
        EndMesh();
        break;

    case block_data::SimpleFacesBlock8:
        // no need to implement
        break;

    case block_data::GroupTriggerBlock9:
        // possibly not implemented
        break;

    case block_data::GroupLodParametersBlock10:
        m_currentLods.pop();
        break;

    case block_data::GroupUnknownBlock12:
        // possibly not implemented
        break;

    case block_data::SimpleTriggerBlock13:
        // possibly not implemented
        break;

    case block_data::SimpleUnknownBlock14:
        // possibly not implemented
        break;

    case block_data::SimpleObjectConnectorBlock18:
        // no need to implement
        break;

    case block_data::GroupObjectsBlock19:
        EndSceneNode();
        break;

    case block_data::SimpleFlatCollisionBlock20:
        // no need to implement
        break;

    case block_data::GroupObjectsBlock21:
        EndSceneNode();
        break;

    case block_data::SimpleVolumeCollisionBlock23:
        // possibly not implemented
        break;

    case block_data::GroupTransformMatrixBlock24:
        ProcessTransformQueue();
        break;

    case block_data::SimpleFacesBlock28:
        // no need to implement
        break;

    case block_data::GroupUnknownBlock29:
        // possibly not implemented
        break;

    case block_data::SimplePortalBlock30:
        // possibly not implemented
        break;

    case block_data::GroupLightingObjectBlock33:
        EndSceneNode();
        break;

    case block_data::SimpleFaceDataBlock35:
        m_data35TypeQueue.pop();
        break;

    case block_data::GroupIndexAndTexturesBlock37:
        EndMesh();
        break;

    case block_data::SimpleGeneratedObjectsBlock40:
        // possibly not implemented
        break;

    default:
        assert(0 && "Not implemented");
    };

    m_blockNames.pop();
}

void B3dMeshListener::OnNestedBlockBegin(std::uint32_t /* nestedBlockNumber */)
{
    // no need to implement
}

void B3dMeshListener::OnNestedBlockEnd(std::uint32_t /* nestedBlockNumber */)
{
    // no need to implement
}

void B3dMeshListener::OnBlock(const block_data::Empty0& /* block */)
{
    // no need to implement
}

void B3dMeshListener::OnBlock(const block_data::GroupRoadInfraObjects4& /* block */)
{
    BeginSceneNode();
}


void B3dMeshListener::OnBlock(const block_data::GroupObjects5& /* block */)
{
    BeginSceneNode();
}

void B3dMeshListener::OnBlock(const block_data::GroupVertex7& /* block */)
{
    BeginMesh(false);
}

void B3dMeshListener::OnBlock(const block_data::SimpleFaces8& /* block */)
{
    // no need to implement
}

void B3dMeshListener::OnBlock(const block_data::GroupTrigger9& /* block */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dMeshListener::OnBlock(const block_data::GroupLodParameters10& block)
{
    m_currentLods.push(block);
}

void B3dMeshListener::OnBlock(const block_data::GroupUnknown12& /* block */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dMeshListener::OnBlock(const block_data::SimpleTrigger13& /* block */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dMeshListener::OnBlock(const block_data::SimpleUnknown14& /* block */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dMeshListener::OnBlock(const block_data::SimpleObjectConnector18& block)
{
    std::string sceneName = GetNameImpl(GetB3dResourceId(block.object), "scene_node", false);

    Ogre::SceneNode* sceneNode = m_sceneManager->getSceneNode(sceneName, false);
    if (sceneNode)
    {
        //auto transformList = m_transformMap[GetB3dResourceId(block.space)];
        //for (const auto& transform : transformList)
        //{
        //    sceneNode->translate(transform.matrix, transform.position);
        //}
    }
}

void B3dMeshListener::OnBlock(const block_data::GroupObjects19& /* block */)
{
    BeginSceneNode();
}

void B3dMeshListener::OnBlock(const block_data::SimpleFlatCollision20& /* block */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dMeshListener::OnBlock(const block_data::GroupObjects21& /* block */)
{
    BeginSceneNode();
}

void B3dMeshListener::OnBlock(const block_data::SimpleVolumeCollision23& /* block */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dMeshListener::OnBlock(const block_data::GroupTransformMatrix24& block)
{
    m_transformQueue.push_back(block);
}

void B3dMeshListener::OnBlock(const block_data::SimpleFaces28& /* block */)
{
    // no need to implement
}

void B3dMeshListener::OnBlock(const block_data::GroupUnknown29& /* block */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dMeshListener::OnBlock(const block_data::SimplePortal30& /* block */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dMeshListener::OnBlock(const block_data::GroupLightingObjects33& block)
{
    BeginSceneNode();

    Ogre::Light* light = m_sceneManager->createLight(m_blockNames.top() + ".light");
    Ogre::SceneNode* lightNode = m_sceneManager->createSceneNode(m_blockNames.top() + ".light.scene_node");
    lightNode->attachObject(light);
    lightNode->setPosition(block.position);
    m_sceneNodes.top()->addChild(lightNode);

    // TODO: setup light
    B3D_NOT_IMPLEMENTED();
}

void B3dMeshListener::OnBlock(const block_data::SimpleFaceData35& block)
{
    m_data35TypeQueue.push(block.type);
}

void B3dMeshListener::OnBlock(const block_data::GroupVertexData37& /* block */)
{
    BeginMesh(true);
}

void B3dMeshListener::OnBlock(const block_data::SimpleGeneratedObjects40& /* block */)
{
    B3D_NOT_IMPLEMENTED();
}

///////////////////////////////////////////////////////////////////////////

void B3dMeshListener::OnData(common::PositionWithTexCoordNormalList&& data)
{
    Ogre::MeshPtr currentMesh = m_meshQueue.top();
    
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

    Ogre::AxisAlignedBox bbox = currentMesh->getBounds();

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

    currentMesh->sharedVertexData = vertexData.release();

    currentMesh->_setBounds(bbox, true);
}

void B3dMeshListener::OnData(common::PositionWithTexCoordList&& data)
{
    Ogre::MeshPtr currentMesh = m_meshQueue.top();

    std::unique_ptr<Ogre::VertexData> vertexData{ OGRE_NEW Ogre::VertexData };

    vertexData->vertexCount = data.size();
    Ogre::VertexDeclaration* decl = vertexData->vertexDeclaration;
    Ogre::VertexBufferBinding* bind = vertexData->vertexBufferBinding;

    size_t offset = 0;

    decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

    decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

    Ogre::AxisAlignedBox bbox = currentMesh->getBounds();

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

    currentMesh->sharedVertexData = vertexData.release();

    currentMesh->_setBounds(bbox, true);
}

void B3dMeshListener::OnData(common::PositionWithNormalList&& data)
{
    Ogre::MeshPtr currentMesh = m_meshQueue.top();

    std::unique_ptr<Ogre::VertexData> vertexData{ OGRE_NEW Ogre::VertexData };

    vertexData->vertexCount = data.size();
    Ogre::VertexDeclaration* decl = vertexData->vertexDeclaration;
    Ogre::VertexBufferBinding* bind = vertexData->vertexBufferBinding;

    size_t offset = 0;

    decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

    decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

    Ogre::AxisAlignedBox bbox = currentMesh->getBounds();

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

    currentMesh->sharedVertexData = vertexData.release();

    currentMesh->_setBounds(bbox, true);
}

void B3dMeshListener::OnData(block_data::Mesh35&& data)
{
    if (m_data35TypeQueue.back() == block_data::SimpleFaceData35::IndicesOnly3)
    {
        if (data.type == block_data::Mesh35::Indices16)
        {
            Ogre::SubMesh* subMesh = m_meshQueue.top()->createSubMesh();
            subMesh->useSharedVertices = true;
            subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

            subMesh->setMaterialName(GetMaterialName(data.materialIndex));
        }
        else if (data.type == block_data::Mesh35::Indices1)
        {
            Ogre::SubMesh* subMesh = m_meshQueue.top()->createSubMesh();
            subMesh->useSharedVertices = true;
            subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

            subMesh->setMaterialName(GetMaterialName(data.materialIndex));
        }
        else if (data.type == block_data::Mesh35::Indices3)
        {
            Ogre::SubMesh* subMesh = m_meshQueue.top()->createSubMesh();
            subMesh->useSharedVertices = true;
            subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

            subMesh->setMaterialName(GetMaterialName(data.materialIndex));
        }
        else if (data.type == block_data::Mesh35::Indices0)
        {
            Ogre::SubMesh* subMesh = m_meshQueue.top()->createSubMesh();
            subMesh->useSharedVertices = true;
            subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

            subMesh->setMaterialName(GetMaterialName(data.materialIndex));
        }
        else if (data.type == block_data::Mesh35::Indices17)
        {
            Ogre::SubMesh* subMesh = m_meshQueue.top()->createSubMesh();
            subMesh->useSharedVertices = true;
            subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

            subMesh->setMaterialName(GetMaterialName(data.materialIndex));
        }
        else
        {
            assert(0 && "not implemented");
        }
    }
    else if (m_data35TypeQueue.back() == block_data::SimpleFaceData35::Unknown2)
    {
        if (data.type == block_data::Mesh35::Indices1)
        {
            Ogre::SubMesh* subMesh = m_meshQueue.top()->createSubMesh();
            subMesh->useSharedVertices = true;
            subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

            subMesh->setMaterialName(GetMaterialName(data.materialIndex));
        }
        else if (data.type == block_data::Mesh35::UnknownType3)
        {
            Ogre::SubMesh* subMesh = m_meshQueue.top()->createSubMesh();
            subMesh->useSharedVertices = false;
            subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

            subMesh->setMaterialName(GetMaterialName(data.materialIndex));
        }
        else if (data.type == block_data::Mesh35::UnknownType51)
        {
            Ogre::SubMesh* subMesh = m_meshQueue.top()->createSubMesh();
            subMesh->useSharedVertices = false;
            subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

            subMesh->setMaterialName(GetMaterialName(data.materialIndex));
        }
        else if (data.type == block_data::Mesh35::UnknownType49)
        {
            Ogre::SubMesh* subMesh = m_meshQueue.top()->createSubMesh();
            subMesh->useSharedVertices = true;
            subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

            subMesh->setMaterialName(GetMaterialName(data.materialIndex));
        }
        else
        {
            assert(0 && "not implemented");
        }
    }
    else if (m_data35TypeQueue.back() == block_data::SimpleFaceData35::Unknown1)
    {
        if (data.type == block_data::Mesh35::UnknownType48)
        {
            Ogre::SubMesh* subMesh = m_meshQueue.top()->createSubMesh();
            subMesh->useSharedVertices = false;
            subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

            subMesh->setMaterialName(GetMaterialName(data.materialIndex));
        }
        else if (data.type == block_data::Mesh35::UnknownType50)
        {
            Ogre::SubMesh* subMesh = m_meshQueue.top()->createSubMesh();
            subMesh->useSharedVertices = false;
            subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

            subMesh->setMaterialName(GetMaterialName(data.materialIndex));
        }
        else if (data.type == block_data::Mesh35::Indices0)
        {
            Ogre::SubMesh* subMesh = m_meshQueue.top()->createSubMesh();
            subMesh->useSharedVertices = true;
            subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

            subMesh->setMaterialName(GetMaterialName(data.materialIndex));
        }
        else if (data.type == block_data::Mesh35::Unknown2)
        {
            Ogre::SubMesh* subMesh = m_meshQueue.top()->createSubMesh();
            subMesh->useSharedVertices = false;
            subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

            subMesh->setMaterialName(GetMaterialName(data.materialIndex));
        }
        else
        {
            assert(0 && "not implemented");
        }
    }
    else
    {
        assert(0 && "not implemented");
    }
}

void B3dMeshListener::OnData(common::IndexList&& data)
{
    Ogre::SubMesh* subMesh = m_meshQueue.top()->getSubMeshes().back();

    Ogre::HardwareIndexBufferSharedPtr ibuf = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
        Ogre::HardwareIndexBuffer::IT_32BIT,
        data.size(),
        Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

    ibuf->writeData(0, ibuf->getSizeInBytes(), data.data(), true);

    subMesh->indexData->indexBuffer = ibuf;
    subMesh->indexData->indexCount = data.size();
    subMesh->indexData->indexStart = 0;
}

void B3dMeshListener::OnData(common::IndexWithTexCoordList&& data)
{
    Ogre::SubMesh* subMesh = m_meshQueue.top()->getSubMeshes().back();

    common::IndexList indices;
    common::TexCoordList texCoords;
    for (const auto& item : data)
    {
        indices.push_back(item.index);
        texCoords.push_back(item.texCoord);
    }

    {
        Ogre::HardwareIndexBufferSharedPtr ibuf = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
            Ogre::HardwareIndexBuffer::IT_32BIT,
            indices.size(),
            Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

        ibuf->writeData(0, ibuf->getSizeInBytes(), indices.data(), true);

        subMesh->indexData->indexBuffer = ibuf;
        subMesh->indexData->indexCount = indices.size();
        subMesh->indexData->indexStart = 0;
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

void B3dMeshListener::OnData(common::IndexWithPositionList&& data)
{
    Ogre::SubMesh* subMesh = m_meshQueue.top()->getSubMeshes().back();

    common::IndexList indices;
    common::PositionList positions;
    for (const auto& item : data)
    {
        indices.push_back(item.index);
        positions.push_back(item.position);
    }

    {
        Ogre::HardwareIndexBufferSharedPtr ibuf = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
            Ogre::HardwareIndexBuffer::IT_32BIT,
            indices.size(),
            Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

        ibuf->writeData(0, ibuf->getSizeInBytes(), indices.data(), true);

        subMesh->indexData->indexBuffer = ibuf;
        subMesh->indexData->indexCount = indices.size();
        subMesh->indexData->indexStart = 0;
    }

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

void B3dMeshListener::OnData(common::IndexWithPositionTexCoordList&& data)
{
    Ogre::SubMesh* subMesh = m_meshQueue.top()->getSubMeshes().back();

    common::IndexList indices;
    common::PositionWithTexCoordList positions;
    for (const auto& item : data)
    {
        indices.push_back(item.index);
        positions.push_back({ item.position, item.texCoord });
    }

    {
        Ogre::HardwareIndexBufferSharedPtr ibuf = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
            Ogre::HardwareIndexBuffer::IT_32BIT,
            indices.size(),
            Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

        ibuf->writeData(0, ibuf->getSizeInBytes(), indices.data(), true);

        subMesh->indexData->indexBuffer = ibuf;
        subMesh->indexData->indexCount = indices.size();
        subMesh->indexData->indexStart = 0;
    }

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

void B3dMeshListener::OnData(block_data::Face8&& data)
{
    if (data.type == block_data::Face8::FaceIndexType129)
    {
        Ogre::SubMesh* subMesh = m_meshQueue.top()->createSubMesh();
        subMesh->useSharedVertices = true;
        subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;

        subMesh->setMaterialName(GetMaterialName(data.materialIndex));
    }
    else if (data.type == block_data::Face8::FaceIndexType50)
    {
        Ogre::SubMesh* subMesh = m_meshQueue.top()->createSubMesh();
        subMesh->useSharedVertices = false;
        subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

        subMesh->setMaterialName(GetMaterialName(data.materialIndex));
    }
    else if (data.type == block_data::Face8::FaceIndexType178)
    {
        Ogre::SubMesh* subMesh = m_meshQueue.top()->createSubMesh();
        subMesh->useSharedVertices = false;
        subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;

        subMesh->setMaterialName(GetMaterialName(data.materialIndex));
    }
    else if (data.type == block_data::Face8::UnknownType144)
    {
        Ogre::SubMesh* subMesh = m_meshQueue.top()->createSubMesh();
        subMesh->useSharedVertices = true;
        subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;

        subMesh->setMaterialName(GetMaterialName(data.materialIndex));
    }
    else if (data.type == block_data::Face8::UnknownType16)
    {
        Ogre::SubMesh* subMesh = m_meshQueue.top()->createSubMesh();
        subMesh->useSharedVertices = true;
        subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;

        subMesh->setMaterialName(GetMaterialName(data.materialIndex));
    }
    else if (data.type == block_data::Face8::UnknownType1)
    {
        Ogre::SubMesh* subMesh = m_meshQueue.top()->createSubMesh();
        subMesh->useSharedVertices = true;
        subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;

        subMesh->setMaterialName(GetMaterialName(data.materialIndex));
    }
    else if (data.type == block_data::Face8::FaceIndexType48)
    {
        Ogre::SubMesh* subMesh = m_meshQueue.top()->createSubMesh();
        subMesh->useSharedVertices = false;
        subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;

        subMesh->setMaterialName(GetMaterialName(data.materialIndex));
    }
    else if (data.type == block_data::Face8::UnknownType0)
    {
        Ogre::SubMesh* subMesh = m_meshQueue.top()->createSubMesh();
        subMesh->useSharedVertices = true;
        subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;

        subMesh->setMaterialName(GetMaterialName(data.materialIndex));
    }
    else if (data.type == block_data::Face8::FaceIndexType3)
    {
        Ogre::SubMesh* subMesh = m_meshQueue.top()->createSubMesh();
        subMesh->useSharedVertices = true;
        subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;

        subMesh->setMaterialName(GetMaterialName(data.materialIndex));
    }
    else if (data.type == block_data::Face8::FaceIndexType176)
    {
        Ogre::SubMesh* subMesh = m_meshQueue.top()->createSubMesh();
        subMesh->useSharedVertices = false;
        subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;

        subMesh->setMaterialName(GetMaterialName(data.materialIndex));
    }
    else if (data.type == block_data::Face8::FaceIndexType2)
    {
        Ogre::SubMesh* subMesh = m_meshQueue.top()->createSubMesh();
        subMesh->useSharedVertices = true;
        subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;

        subMesh->setMaterialName(GetMaterialName(data.materialIndex));
    }
    else if (data.type == block_data::Face8::FaceIndexType128)
    {
        Ogre::SubMesh* subMesh = m_meshQueue.top()->createSubMesh();
        subMesh->useSharedVertices = true;
        subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;

        subMesh->setMaterialName(GetMaterialName(data.materialIndex));
    }
    else
    {
        assert(0 && "not implemented");
    }
}

void B3dMeshListener::OnData(common::CollisionPositionList&& /* data */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dMeshListener::OnData(common::CollisionUnknownList&& /* data */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dMeshListener::OnData(common::TriggerInfoList&& /* data */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dMeshListener::OnData(common::GeneratedObjectInfo&& /* data */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dMeshListener::OnData(block_data::Face28Entry&& data)
{
    if (data.type == block_data::Face28Entry::Unknown2)
    {
        Ogre::SubMesh* subMesh = m_meshQueue.top()->createSubMesh();
        subMesh->useSharedVertices = true;
        subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;

        subMesh->setMaterialName(GetMaterialName(data.materialIndex));
    }
    else
    {
        assert(0 && "not implemented");
    }
}

void B3dMeshListener::OnData(std::vector<block_data::Face28Entry::Unknown>&& data)
{
    Ogre::SubMesh* subMesh = m_meshQueue.top()->getSubMeshes().back();

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

void B3dMeshListener::OnData(std::vector<block_data::GroupVertexData37::Unknown258>&& data)
{
    Ogre::MeshPtr currentMesh = m_meshQueue.top();

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


    Ogre::AxisAlignedBox bbox = currentMesh->getBounds();

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

    currentMesh->sharedVertexData = vertexData.release();

    currentMesh->_setBounds(bbox, true);
}

void B3dMeshListener::OnData(std::vector<block_data::GroupVertexData37::Unknown514>&& data)
{
    Ogre::MeshPtr currentMesh = m_meshQueue.top();

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


    Ogre::AxisAlignedBox bbox = currentMesh->getBounds();

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

    currentMesh->sharedVertexData = vertexData.release();

    currentMesh->_setBounds(bbox, true);
}

void B3dMeshListener::OnData(std::vector<block_data::Mesh35::Unknown49>&& data)
{
    common::IndexList indices;
    for (const auto& item : data)
    {
        indices.push_back(item.index);
    }

    Ogre::SubMesh* subMesh = m_meshQueue.top()->getSubMeshes().back();

    Ogre::HardwareIndexBufferSharedPtr ibuf = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
        Ogre::HardwareIndexBuffer::IT_32BIT,
        indices.size(),
        Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

    ibuf->writeData(0, ibuf->getSizeInBytes(), indices.data(), true);

    subMesh->indexData->indexBuffer = ibuf;
    subMesh->indexData->indexCount = indices.size();
    subMesh->indexData->indexStart = 0;
}

///////////////////////////////////////////////////////////////////////////

std::string B3dMeshListener::GetB3dResourceId(const std::string& name) const
{
    return common::GetResourceName(m_b3dId, name);
}

std::string B3dMeshListener::GetB3dResourceId(const common::ResourceName& name) const
{
    return GetB3dResourceId(GetResourceName(name));
}

std::string B3dMeshListener::GetMaterialName(const std::uint32_t materialIndex) const
{
    return GetB3dResourceId(GetResourceName(m_materials[materialIndex]));
}

std::string B3dMeshListener::GetNameImpl(const std::string& blockName, const std::string& subName, bool forceUnique)
{
    std::string name = blockName;

    if (name.empty())
    {
        name = "unnamed";
    }
    name += ("_" + subName);

    if (forceUnique)
    {
        static int cnt = 0;
        name += ("_" + std::to_string(cnt));
        ++cnt;
    }

    return name;
}

std::string B3dMeshListener::GetName(const std::string& subname, bool forceUnique) const
{
    return GetNameImpl(m_blockNames.top(), subname, forceUnique);
}

void B3dMeshListener::ProcessTransformQueue()
{
    TransformList transformList;
    for (const auto& block : m_transformQueue)
    {
        Transform transform;

        transform.matrix.FromAxes(block.x, block.y, block.z);
        transform.position = block.position;

        transformList.push_back(transform);
    }

    m_transformMap[m_blockNames.top()] = transformList;
    m_transformQueue.pop_back();
}

void B3dMeshListener::BeginSceneNode()
{
    std::string name = GetName("scene_node", false);
    if (m_sceneManager->hasSceneNode(name))
    {
        name = GetName("scene_node", true);
    }

    Ogre::SceneNode* sceneNode = m_sceneManager->createSceneNode(name);
    Ogre::SceneNode* parent = m_sceneNodes.empty() ? m_rootNode : m_sceneNodes.top();
    parent->addChild(sceneNode);

    m_sceneNodes.push(sceneNode);

    D2_HACK_LOG(B3dMeshListener::BeginSceneNode) << "name: " << name << ", parent: " << parent->getName() << ", scene nodes count: " << m_sceneNodes.size();
}

void B3dMeshListener::EndSceneNode()
{
    Ogre::SceneNode* node = m_sceneNodes.top();

    if (node->getChildren().empty() && node->getAttachedObjects().empty())
    {
        D2_HACK_LOG(B3dMeshListener::EndSceneNode) << "removing unused scene node: " << m_sceneNodes.top()->getName();
        m_sceneManager->destroySceneNode(node);
    }

    D2_HACK_LOG(B3dMeshListener::EndSceneNode) << "name: " << m_sceneNodes.top()->getName() << ", scene nodes count: " << m_sceneNodes.size();
    m_sceneNodes.pop();
}

void B3dMeshListener::BeginMesh(bool shouldHasName)
{
    if (shouldHasName)
    {
        assert(!m_blockNames.top().empty());
    }
    m_meshQueue.push(m_meshManager->createManual(GetName("mesh", true), "D2"));
}

void B3dMeshListener::EndMesh()
{
    Ogre::MeshPtr currentMesh = m_meshQueue.top();
    m_meshQueue.pop();

    if (m_currentLods.size() < 2)
    {
        const std::string entityName = currentMesh->getName() + ".entity";
        Ogre::Entity* entity = m_sceneManager->createEntity(entityName, currentMesh);

        m_sceneNodes.top()->attachObject(entity);
    }
}

} // namespace app
} // namespace d2_hack
