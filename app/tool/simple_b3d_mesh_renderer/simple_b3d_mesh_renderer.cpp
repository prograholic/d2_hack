#include "simple_b3d_mesh_renderer.h"

#include <OgreMesh.h>
#include <OgreSubMesh.h>
#include <OgreMeshManager.h>
#include <OgreHardwareBufferManager.h>
#include <OgreEntity.h>


#include <d2_hack/codec/data/b3d_reader.h>

namespace d2_hack
{
namespace app
{

using namespace codec::data::b3d;

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
    Ogre::SceneNode* lightSceneNode = rootNode->createChildSceneNode();
    lightSceneNode->attachObject(light);
    lightSceneNode->setPosition(20.0f, 80.0f, 150.0f);


    Ogre::SceneNode* b3dSceneNode = rootNode->createChildSceneNode("b3d");
    

    LoadB3d(D2_ROOT_DIR "/ENV/aa.b3d", b3dSceneNode);
    LoadB3d(D2_ROOT_DIR "/ENV/ab.b3d", b3dSceneNode);
    LoadB3d(D2_ROOT_DIR "/ENV/ac.b3d", b3dSceneNode);

    b3dSceneNode->pitch(Ogre::Radian(Ogre::Degree(-90)));
}


struct B3dMeshListener : public VoidB3dListener
{
    explicit B3dMeshListener(const char* b3dName)
        : m_b3dName(b3dName)
    {
    }

    std::string m_b3dName;
    Ogre::SceneManager* m_sceneManager = nullptr;
    Ogre::SceneNode* m_rootNode = nullptr;
    Ogre::MeshManager* m_meshManager = nullptr;

    Ogre::MeshPtr m_currentMesh;
    common::IndexList m_currentIndices;
    std::uint32_t m_currentMaterialIndex = std::numeric_limits<std::uint32_t>::max();

    Materials m_materials;

    virtual void OnMaterials(Materials&& materials) override
    {
        m_materials = std::move(materials);
    }

    virtual void OnBlockBegin(const block_data::BlockHeader& blockHeader) override
    {
        
        if ((blockHeader.type == block_data::GroupIndexAndTexturesBlock37) || (blockHeader.type == block_data::GroupVertexBlock7))
        {
            std::string name = GetResourceName(blockHeader.name) + GetUniqueId();
            if (!m_meshManager->getByName(name, "D2"))
            {
                m_currentMesh = m_meshManager->createManual(name, "D2");
            }
        }
    }

    virtual void OnBlockEnd(const block_data::BlockHeader& blockHeader) override
    {
        if (blockHeader.type == block_data::GroupIndexAndTexturesBlock37)
        {
            if (m_currentMesh)
            {
                Ogre::Entity* meshEntity = m_sceneManager->createEntity(m_currentMesh->getName() + "::entity", m_currentMesh);

                Ogre::SceneNode* meshSceneNode = m_rootNode->createChildSceneNode(m_currentMesh->getName() + "::scene_node");
                meshSceneNode->attachObject(meshEntity);

                m_currentMesh.reset();
            }
        }
        else if (blockHeader.type == block_data::SimpleFaceDataBlock35)
        {
            if (m_currentMesh)
            {
                Ogre::SubMesh* sub = m_currentMesh->createSubMesh();
                sub->useSharedVertices = true;

                if (m_currentIndices.empty())
                {
                    sub->operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;
                    __debugbreak();
                }
                else
                {
                    Ogre::HardwareIndexBufferSharedPtr ibuf = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
                        Ogre::HardwareIndexBuffer::IT_32BIT,
                        m_currentIndices.size(),
                        Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

                    ibuf->writeData(0, ibuf->getSizeInBytes(), m_currentIndices.data(), true);

                    sub->indexData->indexBuffer = ibuf;
                    sub->indexData->indexCount = m_currentIndices.size();
                    sub->indexData->indexStart = 0;

                    const std::string materialName = GetMaterialName(m_currentMaterialIndex);

                    sub->setMaterialName(materialName, "D2");
                }
            }

            m_currentIndices.clear();
            m_currentMaterialIndex = std::numeric_limits<std::uint32_t>::max();
        }
    }

    virtual void OnBlock(const block_data::GroupVertexData37& /* block */) override
    {
        //
    }

    virtual void OnBlock(const block_data::SimpleFaceData35& block) override
    {
        m_currentMaterialIndex = block.materialIndex;
    }

    virtual void OnData(common::PositionWithNormalList&& data) override
    {
        if (m_currentMesh)
        {
            m_currentMesh->sharedVertexData = OGRE_NEW Ogre::VertexData;

            m_currentMesh->sharedVertexData->vertexCount = data.size();
            Ogre::VertexDeclaration* decl = m_currentMesh->sharedVertexData->vertexDeclaration;
            Ogre::VertexBufferBinding* bind = m_currentMesh->sharedVertexData->vertexBufferBinding;

            size_t offset = 0;

            decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

            decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_NORMAL);
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

            Ogre::AxisAlignedBox bbox;

            for (const auto& vertex : data)
            {
                bbox.merge(vertex.position);
            }

            Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                offset,
                m_currentMesh->sharedVertexData->vertexCount,
                Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

            vbuf->writeData(0, vbuf->getSizeInBytes(), data.data(), true);
            bind->setBinding(0, vbuf);
            m_currentMesh->_setBounds(bbox, true);
        }
    }

    virtual void OnData(common::PositionWithTexCoordList&& data) override
    {
        if (m_currentMesh)
        {
            m_currentMesh->sharedVertexData = OGRE_NEW Ogre::VertexData;

            m_currentMesh->sharedVertexData->vertexCount = data.size();
            Ogre::VertexDeclaration* decl = m_currentMesh->sharedVertexData->vertexDeclaration;
            Ogre::VertexBufferBinding* bind = m_currentMesh->sharedVertexData->vertexBufferBinding;

            size_t offset = 0;

            decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

            decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

            Ogre::AxisAlignedBox bbox;

            for (const auto& vertex : data)
            {
                bbox.merge(vertex.position);
            }

            Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                offset,
                m_currentMesh->sharedVertexData->vertexCount,
                Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

            vbuf->writeData(0, vbuf->getSizeInBytes(), data.data(), true);
            bind->setBinding(0, vbuf);
            m_currentMesh->_setBounds(bbox, true);
        }
    }

    virtual void OnData(common::PositionWithTexCoordNormalList&& data) override
    {
        if (m_currentMesh)
        {
            m_currentMesh->sharedVertexData = OGRE_NEW Ogre::VertexData;

            m_currentMesh->sharedVertexData->vertexCount = data.size();
            Ogre::VertexDeclaration* decl = m_currentMesh->sharedVertexData->vertexDeclaration;
            Ogre::VertexBufferBinding* bind = m_currentMesh->sharedVertexData->vertexBufferBinding;

            size_t offset = 0;

            decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

            decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

            decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

            Ogre::AxisAlignedBox bbox;

            for (const auto& vertex : data)
            {
                bbox.merge(vertex.position);
            }

            Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                offset,
                m_currentMesh->sharedVertexData->vertexCount,
                Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

            vbuf->writeData(0, vbuf->getSizeInBytes(), data.data(), true);
            bind->setBinding(0, vbuf);
            m_currentMesh->_setBounds(bbox, true);
        }
    }

    virtual void OnData(common::IndexList&& data) override
    {
        if (m_currentMesh)
        {
            m_currentIndices.insert(m_currentIndices.end(), data.begin(), data.end());
        }
    }

    std::string GetMaterialName(const std::uint32_t materialIndex) const
    {
        return GetResourceName(m_materials[materialIndex]);
    }


    std::string GetUniqueId() const
    {
        return "::" + m_b3dName + "::";
    }
};

void SimpleB3dMeshRenderer::LoadB3d(const char* b3dName, Ogre::SceneNode* b3dSceneNode)
{
    B3dMeshListener listener{b3dName};
    listener.m_sceneManager = m_sceneManager;
    listener.m_rootNode = b3dSceneNode;
    listener.m_meshManager = mRoot->getMeshManager();


    std::ifstream inputFile{b3dName, std::ios_base::binary};
    if (!inputFile)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_FILE_NOT_FOUND, "failed to open file");
    }

    Ogre::FileStreamDataStream dataStream(&inputFile, false);
    B3dReader reader;
    reader.Read(dataStream, listener);
}

} // namespace app
} // namespace d2_hack
