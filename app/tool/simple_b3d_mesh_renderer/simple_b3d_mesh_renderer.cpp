#include "simple_b3d_mesh_renderer.h"

#include <OgreMesh.h>
#include <OgreSubMesh.h>
#include <OgreMeshManager.h>
#include <OgreHardwareBufferManager.h>
#include <OgreEntity.h>


#include <d2_hack/resource/data/b3d_reader.h>
#include <d2_hack/common/resource_mgmt.h>
#include <d2_hack/common/log.h>

namespace d2_hack
{
namespace app
{

#define B3D_NOT_IMPLEMENTED() D2_HACK_LOG("") << __FUNCSIG__ << ": NOT IMPLEMENTED"
//#define B3D_NOT_IMPLEMENTED()

using namespace resource::data::b3d;

std::string GetResourceName(const common::ResourceName& resName)
{
    const char* data = reinterpret_cast<const char*>(resName.data());
    return std::string(data, data + strnlen(data, resName.size()));
}


SimpleB3dMeshRenderer::SimpleB3dMeshRenderer()
    : BaseApplication()
{
}

void SimpleB3dMeshRenderer::CreateScene()
{
    m_sceneManager->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

    Ogre::Light* light = m_sceneManager->createLight("MainLight");
    Ogre::SceneNode* rootNode = m_sceneManager->getRootSceneNode();
    Ogre::SceneNode* lightSceneNode = rootNode->createChildSceneNode("light.scene_node");
    lightSceneNode->attachObject(light);
    lightSceneNode->setPosition(20.0f, 80.0f, 150.0f);


    Ogre::SceneNode* b3dSceneNode = rootNode->createChildSceneNode("b3d.scene_node");

    LoadB3d("aa", b3dSceneNode);
    LoadB3d("ab", b3dSceneNode);
    LoadB3d("ac", b3dSceneNode);
    LoadB3d("ad", b3dSceneNode);
    LoadB3d("ae", b3dSceneNode);
    LoadB3d("af", b3dSceneNode);
    LoadB3d("ag", b3dSceneNode);
    LoadB3d("ah", b3dSceneNode);
    LoadB3d("aj", b3dSceneNode);
    LoadB3d("ak", b3dSceneNode);
    LoadB3d("al", b3dSceneNode);
    LoadB3d("am", b3dSceneNode);
    LoadB3d("ap", b3dSceneNode);
    LoadB3d("aq", b3dSceneNode);
    LoadB3d("ar", b3dSceneNode);
    LoadB3d("as", b3dSceneNode);
    LoadB3d("at", b3dSceneNode);
    LoadB3d("au", b3dSceneNode);
    LoadB3d("av", b3dSceneNode);
    LoadB3d("aw", b3dSceneNode);
    LoadB3d("ax", b3dSceneNode);

    b3dSceneNode->pitch(Ogre::Radian(Ogre::Degree(-90)));
}

struct AssertB3dAction
{
    static void OnBlockBegin(const block_data::BlockHeader& /* blockHeader */)
    {
        assert(0 && "OnBlockBegin is not implemented");
    }

    static void OnBlockEnd(const block_data::BlockHeader& /* blockHeader */)
    {
        assert(0 && "OnBlockEnd is not implemented");
    }

    static void OnNestedBlockBegin(std::uint32_t /* nestedBlockNumber */)
    {
        assert(0 && "OnNestedBlockBegin is not implemented");
    }

    static void OnNestedBlockEnd(std::uint32_t /* nestedBlockNumber */)
    {
        assert(0 && "OnNestedBlockEnd is not implemented");
    }

    static void OnMaterials(Materials&& /* materials */)
    {
        assert(0 && "OnMaterials is not implemented");
    }

    template <typename T>
    static void OnBlock(const T& /* block */)
    {
        assert(0 && "OnBlock is not implemented");
    }

    template <typename T>
    static void OnData(T&& /* data */)
    {
        assert(0 && "OnData is not implemented");
    }
};


struct B3dMeshListener : public SimpleActionB3dListener<AssertB3dAction>
{
    explicit B3dMeshListener(const char* b3dId, const std::string& b3dName)
        : m_b3dId(b3dId)
        , m_b3dName(b3dName)
    {
    }

    std::string m_b3dId;
    std::string m_b3dName;
    Ogre::SceneManager* m_sceneManager = nullptr;
    Ogre::SceneNode* m_rootNode = nullptr;
    Ogre::MeshManager* m_meshManager = nullptr;

    std::queue<Ogre::MeshPtr> m_meshQueue;

    struct Transform
    {
        Ogre::Matrix3 matrix;
        Ogre::Vector3 position;
    };
    typedef std::list<Transform> TransformList;

    std::map<std::string, TransformList> m_transformMap;
    std::list<block_data::GroupTransformMatrix24> m_transformQueue;

    std::queue<std::string> m_blockNames;

    std::queue<Ogre::Real> m_currentLods;

    std::queue<Ogre::SceneNode*> m_sceneNodes;

    Materials m_materials;

    std::queue<std::uint32_t> m_data35TypeQueue;

    virtual void OnMaterials(Materials&& materials) override
    {
        m_materials = std::move(materials);
    }

    ///////////////////////////////////////////////////////////////////////////

    virtual void OnBlockBegin(const block_data::BlockHeader& blockHeader) override
    {
        m_blockNames.push(GetB3dResourceId(blockHeader.name));
    }

    virtual void OnBlockEnd(const block_data::BlockHeader& blockHeader) override
    {
        switch (blockHeader.type)
        {
        case block_data::EmptyBlock0:
            // no need to implement
            break;

        case block_data::GroupRoadInfraObjectsBlock4:
            ProcessSceneNode();
            break;

        case block_data::GroupObjectsBlock5:
            ProcessSceneNode();
            break;

        case block_data::GroupVertexBlock7:
            ProcessMesh();
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

        case block_data::SimpleObjectConnectorBlock18:
            // no need to implement
            break;

        case block_data::GroupObjectsBlock19:
            ProcessSceneNode();
            break;

        case block_data::SimpleFlatCollisionBlock20:
            // no need to implement
            break;

        case block_data::GroupObjectsBlock21:
            ProcessSceneNode();
            break;

        case block_data::SimpleVolumeCollisionBlock23:
            // possibly not implemented
            break;

        case block_data::GroupTransformMatrixBlock24:
            ProcessTransformQueue();
            break;

        case block_data::SimplePortalBlock30:
            // possibly not implemented
            break;

        case block_data::GroupLightingObjectBlock33:
            // no need to implement
            break;

        case block_data::SimpleFaceDataBlock35:
            m_data35TypeQueue.pop();
            break;

        case block_data::GroupIndexAndTexturesBlock37:
            ProcessMesh();
            break;

        case block_data::SimpleGeneratedObjectsBlock40:
            // possibly not implemented
            break;

        default:
            assert(0 && "Not implemented");
        };

        m_blockNames.pop();
    }

    virtual void OnNestedBlockBegin(std::uint32_t /* nestedBlockNumber */) override
    {
        // no need to implement
    }

    virtual void OnNestedBlockEnd(std::uint32_t /* nestedBlockNumber */) override
    {
        // no need to implement
    }

    virtual void OnBlock(const block_data::Empty0& /* block */) override
    {
        // no need to implement
    }

    virtual void OnBlock(const block_data::GroupRoadInfraObjects4& /* block */) override
    {
        CreateSceneNode();
    }


    virtual void OnBlock(const block_data::GroupObjects5& /* block */) override
    {
        CreateSceneNode();
    }

    virtual void OnBlock(const block_data::GroupVertex7& /* block */) override
    {
        CreateMesh();
    }

    virtual void OnBlock(const block_data::SimpleFaces8& /* block */) override
    {
        // no need to implement
    }

    virtual void OnBlock(const block_data::GroupTrigger9& /* block */) override
    {
        B3D_NOT_IMPLEMENTED();
    }

    virtual void OnBlock(const block_data::GroupLodParameters10& block) override
    {
        m_currentLods.push(block.distanceToPlayer);
    }

    virtual void OnBlock(const block_data::GroupUnknown12& /* block */) override
    {
        B3D_NOT_IMPLEMENTED();
    }

    virtual void OnBlock(const block_data::SimpleTrigger13& /* block */) override
    {
        B3D_NOT_IMPLEMENTED();
    }

    virtual void OnBlock(const block_data::SimpleObjectConnector18& block) override
    {
        std::string sceneName = GetNameImpl(GetB3dResourceId(block.object), "scene_node", false);

        Ogre::SceneNode* sceneNode = m_sceneManager->getSceneNode(sceneName, false);
        if (sceneNode)
        {
            auto transformList = m_transformMap[GetB3dResourceId(block.space)];
            //for (const auto& transform : transformList)
            //{
            //    sceneNode->translate(transform.matrix, transform.position);
            //}
        }
    }

    virtual void OnBlock(const block_data::GroupObjects19& /* block */) override
    {
        CreateSceneNode();
    }

    virtual void OnBlock(const block_data::SimpleFlatCollision20& /* block */) override
    {
        B3D_NOT_IMPLEMENTED();
    }

    virtual void OnBlock(const block_data::GroupObjects21& /* block */) override
    {
        CreateSceneNode();
    }

    virtual void OnBlock(const block_data::SimpleVolumeCollision23& /* block */) override
    {
        B3D_NOT_IMPLEMENTED();
    }

    virtual void OnBlock(const block_data::GroupTransformMatrix24& block) override
    {
        m_transformQueue.push_back(block);
    }

    virtual void OnBlock(const block_data::SimplePortal30& /* block */) override
    {
        B3D_NOT_IMPLEMENTED();
    }

    virtual void OnBlock(const block_data::GroupLightingObjects33& block) override
    {
        CreateSceneNode();

        Ogre::Light* light = m_sceneManager->createLight(m_blockNames.back() + ".light");
        Ogre::SceneNode* lightNode = m_sceneManager->createSceneNode(m_blockNames.back() + ".light.scene_node");
        lightNode->attachObject(light);
        lightNode->setPosition(block.position);
        m_sceneNodes.back()->addChild(lightNode);

        // TODO: setup light
        B3D_NOT_IMPLEMENTED();
    }

    virtual void OnBlock(const block_data::SimpleFaceData35& block) override
    {
        m_data35TypeQueue.push(block.type);
    }

    virtual void OnBlock(const block_data::GroupVertexData37& /* block */) override
    {
        CreateMesh();
    }

    virtual void OnBlock(const block_data::SimpleGeneratedObjects40& /* block */) override
    {
        B3D_NOT_IMPLEMENTED();
    }

    ///////////////////////////////////////////////////////////////////////////

    virtual void OnData(common::PositionWithTexCoordNormalList&& data) override
    {
        Ogre::MeshPtr currentMesh = m_meshQueue.back();
        
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

    virtual void OnData(common::PositionWithTexCoordList&& data) override
    {
        Ogre::MeshPtr currentMesh = m_meshQueue.back();

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

    virtual void OnData(common::PositionWithNormalList&& data) override
    {
        Ogre::MeshPtr currentMesh = m_meshQueue.back();

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

    virtual void OnData(block_data::Mesh35&& data) override
    {
        if (m_data35TypeQueue.back() == block_data::SimpleFaceData35::IndicesOnly3)
        {
            if (data.type == block_data::Mesh35::Indices16)
            {
                Ogre::SubMesh* subMesh = m_meshQueue.back()->createSubMesh();
                subMesh->useSharedVertices = true;
                subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

                subMesh->setMaterialName(GetMaterialName(data.materialIndex));
            }
            else if (data.type == block_data::Mesh35::Indices1)
            {
                Ogre::SubMesh* subMesh = m_meshQueue.back()->createSubMesh();
                subMesh->useSharedVertices = true;
                subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

                subMesh->setMaterialName(GetMaterialName(data.materialIndex));
            }
            else if (data.type == block_data::Mesh35::Indices3)
            {
                Ogre::SubMesh* subMesh = m_meshQueue.back()->createSubMesh();
                subMesh->useSharedVertices = true;
                subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

                subMesh->setMaterialName(GetMaterialName(data.materialIndex));
            }
            else if (data.type == block_data::Mesh35::Indices0)
            {
                Ogre::SubMesh* subMesh = m_meshQueue.back()->createSubMesh();
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
                Ogre::SubMesh* subMesh = m_meshQueue.back()->createSubMesh();
                subMesh->useSharedVertices = true;
                subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

                subMesh->setMaterialName(GetMaterialName(data.materialIndex));
            }
            else if (data.type == block_data::Mesh35::UnknownType3)
            {
                Ogre::SubMesh* subMesh = m_meshQueue.back()->createSubMesh();
                subMesh->useSharedVertices = false;
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
                Ogre::SubMesh* subMesh = m_meshQueue.back()->createSubMesh();
                subMesh->useSharedVertices = false;
                subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

                subMesh->setMaterialName(GetMaterialName(data.materialIndex));
            }
            else if (data.type == block_data::Mesh35::UnknownType50)
            {
                Ogre::SubMesh* subMesh = m_meshQueue.back()->createSubMesh();
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

    virtual void OnData(common::IndexList&& data) override
    {
        Ogre::SubMesh* subMesh = m_meshQueue.back()->getSubMeshes().back();

        Ogre::HardwareIndexBufferSharedPtr ibuf = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
            Ogre::HardwareIndexBuffer::IT_32BIT,
            data.size(),
            Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

        ibuf->writeData(0, ibuf->getSizeInBytes(), data.data(), true);

        subMesh->indexData->indexBuffer = ibuf;
        subMesh->indexData->indexCount = data.size();
        subMesh->indexData->indexStart = 0;
    }

    virtual void OnData(common::IndexWithTexCoordList&& data) override
    {
        Ogre::SubMesh* subMesh = m_meshQueue.back()->getSubMeshes().back();

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

    virtual void OnData(common::IndexWithPositionList&& data) override
    {
        Ogre::SubMesh* subMesh = m_meshQueue.back()->getSubMeshes().back();

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

    virtual void OnData(common::IndexWithPositionTexCoordList&& data) override
    {
        Ogre::SubMesh* subMesh = m_meshQueue.back()->getSubMeshes().back();

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

    virtual void OnData(block_data::Face8&& data) override
    {
        if (data.type == block_data::Face8::FaceIndexType129)
        {
            Ogre::SubMesh* subMesh = m_meshQueue.back()->createSubMesh();
            subMesh->useSharedVertices = true;
            subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;

            subMesh->setMaterialName(GetMaterialName(data.materialIndex));
        }
        else if (data.type == block_data::Face8::FaceIndexType50)
        {
            Ogre::SubMesh* subMesh = m_meshQueue.back()->createSubMesh();
            subMesh->useSharedVertices = false;
            subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

            subMesh->setMaterialName(GetMaterialName(data.materialIndex));
        }
        else if (data.type == block_data::Face8::FaceIndexType178)
        {
            Ogre::SubMesh* subMesh = m_meshQueue.back()->createSubMesh();
            subMesh->useSharedVertices = false;
            subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;

            subMesh->setMaterialName(GetMaterialName(data.materialIndex));
        }
        else if (data.type == block_data::Face8::UnknownType144)
        {
            Ogre::SubMesh* subMesh = m_meshQueue.back()->createSubMesh();
            subMesh->useSharedVertices = true;
            subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;

            subMesh->setMaterialName(GetMaterialName(data.materialIndex));
        }
        else if (data.type == block_data::Face8::UnknownType16)
        {
            Ogre::SubMesh* subMesh = m_meshQueue.back()->createSubMesh();
            subMesh->useSharedVertices = true;
            subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;

            subMesh->setMaterialName(GetMaterialName(data.materialIndex));
        }
        else if (data.type == block_data::Face8::UnknownType1)
        {
            Ogre::SubMesh* subMesh = m_meshQueue.back()->createSubMesh();
            subMesh->useSharedVertices = true;
            subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;

            subMesh->setMaterialName(GetMaterialName(data.materialIndex));
        }
        else if (data.type == block_data::Face8::FaceIndexType48)
        {
            Ogre::SubMesh* subMesh = m_meshQueue.back()->createSubMesh();
            subMesh->useSharedVertices = false;
            subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;

            subMesh->setMaterialName(GetMaterialName(data.materialIndex));
        }
        else if (data.type == block_data::Face8::UnknownType0)
        {
            Ogre::SubMesh* subMesh = m_meshQueue.back()->createSubMesh();
            subMesh->useSharedVertices = true;
            subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;

            subMesh->setMaterialName(GetMaterialName(data.materialIndex));
        }
        else if (data.type == block_data::Face8::FaceIndexType3)
        {
            Ogre::SubMesh* subMesh = m_meshQueue.back()->createSubMesh();
            subMesh->useSharedVertices = true;
            subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;

            subMesh->setMaterialName(GetMaterialName(data.materialIndex));
        }
        else
        {
            assert(0 && "not implemented");
        }
    }

    virtual void OnData(common::CollisionPositionList&& /* data */) override
    {
        B3D_NOT_IMPLEMENTED();
    }

    virtual void OnData(common::CollisionUnknownList&& /* data */) override
    {
        B3D_NOT_IMPLEMENTED();
    }

    virtual void OnData(common::TriggerInfoList&& /* data */) override
    {
        B3D_NOT_IMPLEMENTED();
    }

    virtual void OnData(common::GeneratedObjectInfo&& /* data */) override
    {
        B3D_NOT_IMPLEMENTED();
    }

    virtual void OnData(std::vector<block_data::GroupVertexData37::Unknown258>&& data) override
    {
        Ogre::MeshPtr currentMesh = m_meshQueue.back();

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
            bbox.merge(vertex.unknown0);
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

    ///////////////////////////////////////////////////////////////////////////

    std::string GetB3dResourceId(const std::string& name) const
    {
        return common::GetResourceName(m_b3dId, name);
    }

    std::string GetB3dResourceId(const common::ResourceName& name) const
    {
        return GetB3dResourceId(GetResourceName(name));
    }

    std::string GetMaterialName(const std::uint32_t materialIndex) const
    {
        return GetB3dResourceId(GetResourceName(m_materials[materialIndex]));
    }

    static std::string GetNameImpl(const std::string& blockName, const std::string& subName, bool forceUnique)
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

    std::string GetName(const std::string& subname, bool forceUnique) const
    {
        return GetNameImpl(m_blockNames.back(), subname, forceUnique);
    }

    void ProcessTransformQueue()
    {
        TransformList transformList;
        for (const auto& block : m_transformQueue)
        {
            Transform transform;

            transform.matrix.FromAxes(block.x, block.y, block.z);
            transform.position = block.position;

            transformList.push_back(transform);
        }

        m_transformMap[m_blockNames.back()] = transformList;
        m_transformQueue.pop_back();
    }

    void CreateSceneNode()
    {
        std::string name = GetName("scene_node", false);
        if (m_sceneManager->hasSceneNode(name))
        {
            name = GetName("scene_node", true);
        }

        Ogre::SceneNode* sceneNode = m_sceneManager->createSceneNode(name);

        if (m_sceneNodes.empty())
        {
            m_rootNode->addChild(sceneNode);
        }
        else
        {
            m_sceneNodes.back()->addChild(sceneNode);
        }

        m_sceneNodes.push(sceneNode);
    }

    void CreateMesh()
    {
        m_meshQueue.push(m_meshManager->createManual(GetName("mesh", true), "D2"));
    }

    void ProcessMesh()
    {
        Ogre::MeshPtr currentMesh = m_meshQueue.back();
        m_meshQueue.pop();

        const std::string entityName = currentMesh->getName() + ".entity";
        Ogre::Entity* entity = m_sceneManager->createEntity(entityName, currentMesh);

        m_sceneNodes.back()->attachObject(entity);
    }

    void ProcessSceneNode()
    {
        m_sceneNodes.pop();
    }
};

void SimpleB3dMeshRenderer::LoadB3d(const char* b3dId, Ogre::SceneNode* b3dSceneNode)
{
    std::string fullB3dName = D2_ROOT_DIR "/ENV/" + std::string(b3dId) + ".b3d";
    B3dMeshListener listener{b3dId, fullB3dName};
    listener.m_sceneManager = m_sceneManager;
    listener.m_rootNode = b3dSceneNode;
    listener.m_meshManager = mRoot->getMeshManager();


    std::ifstream inputFile{fullB3dName, std::ios_base::binary};
    if (!inputFile)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_FILE_NOT_FOUND, "failed to open file");
    }

    Ogre::FileStreamDataStream dataStream(&inputFile, false);
    B3dReader reader;
    reader.Read(dataStream, listener);
}

bool SimpleB3dMeshRenderer::keyPressed(const OgreBites::KeyboardEvent& evt)
{
    static int visible_count = 0;
    //D2_HACK_LOG("SimpleB3dMeshRenderer::keyPressed") << evt.type << ", " << evt.keysym.sym << ", " << evt.keysym.mod;
    if (evt.keysym.sym == 'l')
    {
        auto rootNode = m_sceneManager->getRootSceneNode();

        Ogre::Node* b3dSceneNode = rootNode->getChild("b3d.scene_node");

        const auto& children = b3dSceneNode->getChildren();
        //Ogre::SceneNode* curNode = static_cast<Ogre::SceneNode*>(children[visible_count % children.size()]);

        //curNode->flipVisibility();
        //D2_HACK_LOG("SimpleB3dMeshRenderer::keyPressed") << "flipped: [" << curNode->getName() << "]";

        //visible_count += 1;

        for (Ogre::Node* child: children)
        {
            //D2_HACK_LOG("SimpleB3dMeshRenderer::keyPressed") << "[" << child->getName() << "]";

            if (child->getName().find("AirportVyshka") != std::string::npos)
            {
                static_cast<Ogre::SceneNode*>(child)->flipVisibility();

                auto p = child->getPosition();
                p.x += 10;
                child->setPosition(p);
            }

        }
    }

    return BaseApplication::keyPressed(evt);
}

} // namespace app
} // namespace d2_hack
