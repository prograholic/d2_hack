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
    //LoadB3d("ab", b3dSceneNode);
    //LoadB3d("ac", b3dSceneNode);

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

    Ogre::MeshPtr m_currentMesh;
    Ogre::SubMesh* m_currentSubMesh = nullptr;
    common::IndexList m_currentIndices;
    std::uint32_t m_currentMaterialIndex = std::numeric_limits<std::uint32_t>::max();
    std::map<std::string, Ogre::Matrix3> m_transformMap;
    std::queue<std::string> m_blockNames;
    std::queue<Ogre::Real> m_currentLods;

    Materials m_materials;

    virtual void OnMaterials(Materials&& materials) override
    {
        m_materials = std::move(materials);
    }

    virtual void OnBlockBegin(const block_data::BlockHeader& blockHeader) override
    {
        m_blockNames.push(GetResourceName(blockHeader.name));
    }

    virtual void OnBlockEnd(const block_data::BlockHeader& blockHeader) override
    {
        m_blockNames.pop();
        if (m_blockNames.empty())
        {
            assert(m_currentSubMesh == nullptr);
            m_currentMesh.reset();
        }

        if (blockHeader.type == block_data::GroupVertexBlock7)
        {
            CleanupEmptySubMesh();
        }
        else if (blockHeader.type == block_data::GroupLodParametersBlock10)
        {
            m_currentLods.pop();
        }
        else if (blockHeader.type == block_data::SimpleFaceDataBlock35)
        {
            if (m_currentSubMesh)
            {
                SetIndices(m_currentSubMesh, m_currentIndices);
            }
            m_currentIndices.clear();
        }
        else if (blockHeader.type == block_data::GroupIndexAndTexturesBlock37)
        {
            CleanupEmptySubMesh();
        }
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

    virtual void OnBlock(const block_data::GroupObjects5& /* block */) override
    {
        ProcessTopLevelGroupObjects();
    }

    virtual void OnBlock(const block_data::GroupVertex7& /* block */) override
    {
        ProcessNewSubMesh();
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

    virtual void OnBlock(const block_data::SimpleTrigger13& /* block */) override
    {
        B3D_NOT_IMPLEMENTED();
    }

    virtual void OnBlock(const block_data::SimpleObjectConnector18& /* block */) override
    {
        B3D_NOT_IMPLEMENTED();
    }

    virtual void OnBlock(const block_data::GroupObjects19& /* block */) override
    {
        ProcessTopLevelGroupObjects();
    }

    virtual void OnBlock(const block_data::SimpleFlatCollision20& /* block */) override
    {
        B3D_NOT_IMPLEMENTED();
    }

    virtual void OnBlock(const block_data::GroupObjects21& /* block */) override
    {
        ProcessTopLevelGroupObjects();
    }

    virtual void OnBlock(const block_data::SimpleVolumeCollision23& /* block */) override
    {
        B3D_NOT_IMPLEMENTED();
    }

    virtual void OnBlock(const block_data::GroupTransformMatrix24& block) override
    {
        Ogre::Matrix3 matrix;
        matrix.FromAxes(block.x, block.y, block.z);
        m_transformMap[m_blockNames.back()] = matrix;
    }

    virtual void OnBlock(const block_data::SimpleFaces28& /* block */) override
    {
        // no need to implement
    }

    virtual void OnBlock(const block_data::SimplePortal30& /* block */) override
    {
        B3D_NOT_IMPLEMENTED();
    }

    virtual void OnBlock(const block_data::GroupLightingObjects33& /* block */) override
    {
        B3D_NOT_IMPLEMENTED();
    }

    virtual void OnBlock(const block_data::SimpleFaceData35& block) override
    {
        ProcessMaterialIndex(block.materialIndex);
    }

    virtual void OnBlock(const block_data::GroupVertexData37& /* block */) override
    {
        ProcessNewSubMesh();
    }

    virtual void OnBlock(const block_data::SimpleGeneratedObjects40& /* block */) override
    {
        B3D_NOT_IMPLEMENTED();
    }

    virtual void OnData(block_data::Face8&& data) override
    {
        ProcessMaterialIndex(data.materialIndex);
    }

    virtual void OnData(block_data::Mesh35&& /* data */) override
    {
        // no need to implement
    }

    virtual void OnData(common::PositionWithNormalList&& data) override
    {
        if (m_currentSubMesh)
        {
            assert(m_currentSubMesh->vertexData == nullptr);
            m_currentSubMesh->vertexData = OGRE_NEW Ogre::VertexData;

            m_currentSubMesh->vertexData->vertexCount = data.size();
            Ogre::VertexDeclaration* decl = m_currentSubMesh->vertexData->vertexDeclaration;
            Ogre::VertexBufferBinding* bind = m_currentSubMesh->vertexData->vertexBufferBinding;

            size_t offset = 0;

            decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

            decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_NORMAL);
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

            Ogre::AxisAlignedBox bbox = m_currentMesh->getBounds();

            for (const auto& vertex : data)
            {
                bbox.merge(vertex.position);
            }

            Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                offset,
                m_currentSubMesh->vertexData->vertexCount,
                Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

            vbuf->writeData(0, vbuf->getSizeInBytes(), data.data(), true);
            bind->setBinding(0, vbuf);

            m_currentMesh->_setBounds(bbox, true);
        }
    }

    virtual void OnData(common::IndexList&& data) override
    {
        if (m_currentSubMesh)
        {
            m_currentIndices.insert(m_currentIndices.end(), data.begin(), data.end());
        }
    }

    virtual void OnData(common::PositionWithTexCoordNormalList&& data) override
    {
        if (m_currentSubMesh)
        {
            assert(m_currentSubMesh->vertexData == nullptr);
            m_currentSubMesh->vertexData = OGRE_NEW Ogre::VertexData;

            m_currentSubMesh->vertexData->vertexCount = data.size();
            Ogre::VertexDeclaration* decl = m_currentSubMesh->vertexData->vertexDeclaration;
            Ogre::VertexBufferBinding* bind = m_currentSubMesh->vertexData->vertexBufferBinding;

            size_t offset = 0;

            decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

            decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

            decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

            Ogre::AxisAlignedBox bbox = m_currentMesh->getBounds();

            for (const auto& vertex : data)
            {
                bbox.merge(vertex.position);
            }

            Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                offset,
                m_currentSubMesh->vertexData->vertexCount,
                Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

            vbuf->writeData(0, vbuf->getSizeInBytes(), data.data(), true);
            bind->setBinding(0, vbuf);

            m_currentMesh->_setBounds(bbox, true);
        }
    }

    virtual void OnData(common::PositionWithTexCoordList&& data) override
    {
        if (m_currentSubMesh)
        {
            assert(m_currentSubMesh->vertexData == nullptr);
            m_currentSubMesh->vertexData = OGRE_NEW Ogre::VertexData;

            m_currentSubMesh->vertexData->vertexCount = data.size();
            Ogre::VertexDeclaration* decl = m_currentSubMesh->vertexData->vertexDeclaration;
            Ogre::VertexBufferBinding* bind = m_currentSubMesh->vertexData->vertexBufferBinding;

            size_t offset = 0;

            decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

            decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

            Ogre::AxisAlignedBox bbox = m_currentMesh->getBounds();

            for (const auto& vertex : data)
            {
                bbox.merge(vertex.position);
            }

            Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                offset,
                m_currentSubMesh->vertexData->vertexCount,
                Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

            vbuf->writeData(0, vbuf->getSizeInBytes(), data.data(), true);
            bind->setBinding(0, vbuf);

            m_currentMesh->_setBounds(bbox, true);
        }
    }

    virtual void OnData(common::PositionList&& data) override
    {
        if (m_currentSubMesh)
        {
            assert(m_currentSubMesh->vertexData == nullptr);
            m_currentSubMesh->vertexData = OGRE_NEW Ogre::VertexData;

            m_currentSubMesh->vertexData->vertexCount = data.size();
            Ogre::VertexDeclaration* decl = m_currentSubMesh->vertexData->vertexDeclaration;
            Ogre::VertexBufferBinding* bind = m_currentSubMesh->vertexData->vertexBufferBinding;

            size_t offset = 0;

            decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
            Ogre::AxisAlignedBox bbox = m_currentMesh->getBounds();

            for (const auto& position : data)
            {
                bbox.merge(position);
            }

            Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                offset,
                m_currentSubMesh->vertexData->vertexCount,
                Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

            vbuf->writeData(0, vbuf->getSizeInBytes(), data.data(), true);
            bind->setBinding(0, vbuf);

            m_currentMesh->_setBounds(bbox, true);
        }
    }

    virtual void OnData(common::IndexWithPositionTexCoordList&& /* data */) override
    {
        B3D_NOT_IMPLEMENTED();
    }

    virtual void OnData(common::IndexWithPositionList&& /* data */) override
    {
        B3D_NOT_IMPLEMENTED();
    }

    virtual void OnData(std::vector<Ogre::Real>&& /* data */) override
    {
        B3D_NOT_IMPLEMENTED();
    }

    virtual void OnData(common::IndexWithTexCoordList&& /* data */) override
    {
        B3D_NOT_IMPLEMENTED();
    }

    virtual void OnData(block_data::Face28Entry&& /* data */) override
    {
        B3D_NOT_IMPLEMENTED();
    }

    virtual void OnData(std::vector<block_data::Face28Entry::Unknown>&& /* data */) override
    {
        B3D_NOT_IMPLEMENTED();
    }

    virtual void OnData(std::vector<block_data::GroupVertexData37::Unknown514>&& /* data */) override
    {
        B3D_NOT_IMPLEMENTED();
    }

    std::string GetMaterialName(const std::uint32_t materialIndex) const
    {
        return GetResourceName(m_materials[materialIndex]);
    }


    std::string GetB3dResourceId(const std::string& name) const
    {
        return common::GetResourceName(m_b3dId, name);
    }

    void ProcessNewSubMesh()
    {
        assert(!m_currentSubMesh);

        const auto& subMeshMap = m_currentMesh->getSubMeshNameMap();
        if (subMeshMap.find(m_blockNames.back()) == subMeshMap.end())
        {
            m_currentSubMesh = m_currentMesh->createSubMesh(m_blockNames.back());
            m_currentSubMesh->useSharedVertices = false;
        }
    }

    void ProcessMaterialIndex(std::uint32_t materialIndex)
    {
        if (m_currentSubMesh)
        {
            const std::string materialName = GetB3dResourceId(GetMaterialName(materialIndex));

            m_currentSubMesh->setMaterialName(materialName, "D2");
        }
    }

    void ProcessTopLevelGroupObjects()
    {
        if (m_blockNames.size() == 1)
        {
            assert(!m_currentMesh);
            const std::string name = GetB3dResourceId(m_blockNames.back());
            m_currentMesh = m_meshManager->createManual(name, "D2");

            Ogre::Entity* meshEntity = m_sceneManager->createEntity(name + ".entity", m_currentMesh);
            Ogre::SceneNode* meshSceneNode = m_rootNode->createChildSceneNode(name + ".scene_node");
            meshSceneNode->attachObject(meshEntity);
        }
    }

    void CleanupEmptySubMesh()
    {
        if (m_currentSubMesh)
        {
            if (m_currentSubMesh->vertexData == nullptr)
            {
                const auto& subMeshes = m_currentMesh->getSubMeshes();

                for (unsigned short i = 0; i != subMeshes.size(); ++i)
                {
                    if (subMeshes[i] == m_currentSubMesh)
                    {
                        m_currentMesh->destroySubMesh(i);
                        break;
                    }
                }
            }
        }
        m_currentSubMesh = nullptr;
    }

    static void SetIndices(Ogre::SubMesh* subMesh, const common::IndexList& indices)
    {
        subMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

        Ogre::HardwareIndexBufferSharedPtr ibuf = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
            Ogre::HardwareIndexBuffer::IT_32BIT,
            indices.size(),
            Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

        ibuf->writeData(0, ibuf->getSizeInBytes(), indices.data(), true);

        subMesh->indexData->indexBuffer = ibuf;
        subMesh->indexData->indexCount = indices.size();
        subMesh->indexData->indexStart = 0;
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

        for (Ogre::Node* child : children)
        {
            D2_HACK_LOG("SimpleB3dMeshRenderer::keyPressed") << "[" << child->getName() << "]";
        }
    }



    return BaseApplication::keyPressed(evt);
}

} // namespace app
} // namespace d2_hack
