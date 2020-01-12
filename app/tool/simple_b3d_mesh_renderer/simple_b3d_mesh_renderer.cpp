#include "simple_b3d_mesh_renderer.h"

#include <boost/variant/get.hpp>

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
    //LoadB3d(D2_ROOT_DIR "/ENV/ab.b3d", b3dSceneNode);

    b3dSceneNode->pitch(Ogre::Radian(Ogre::Degree(-90)));
}

struct B3dMeshLoader : public Ogre::ManualResourceLoader
{
    const codec::data::b3d::block_data::Face8* m_face = nullptr;
    const codec::data::b3d::block_data::GroupVertex7* m_groupVertex = nullptr;

    codec::data::b3d::Materials m_materials;

    virtual void prepareResource(Ogre::Resource* resource) override
    {
        Ogre::Mesh* mesh = static_cast<Ogre::Mesh*>(resource);
        (void)mesh;
    }

    void LoadVertexData8(Ogre::Mesh* mesh)
    {
        using Face = codec::data::b3d::block_data::Face8;

        if (m_face->type == Face::FaceIndexType176)
        {
            mesh->sharedVertexData->vertexCount = m_face->faceDataList.size();
            Ogre::VertexDeclaration* decl = mesh->sharedVertexData->vertexDeclaration;
            Ogre::VertexBufferBinding* bind = mesh->sharedVertexData->vertexBufferBinding;

            size_t offset = 0;

            decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

            Ogre::AxisAlignedBox bbox;

            std::vector<Ogre::Vector3> data;
            data.reserve(m_face->faceDataList.size());
            for (const auto& faceDataEntry : m_face->faceDataList)
            {
                common::IndexWithPosition fi176 = boost::get<common::IndexWithPosition>(faceDataEntry);
                const Ogre::Vector3 position = fi176.position;
                data.push_back(position);

                bbox.merge(position);
            }

            Ogre::HardwareVertexBufferSharedPtr vbuf =
                Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                    offset, mesh->sharedVertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
            vbuf->writeData(0, vbuf->getSizeInBytes(), data.data(), true);
            bind->setBinding(0, vbuf);


            Ogre::SubMesh* sub = mesh->createSubMesh();
            sub->useSharedVertices = true;
            sub->operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;
            //sub->indexData->indexBuffer = ibuf;
            //sub->indexData->indexCount = 6;
            //sub->indexData->indexStart = 0;

            mesh->_setBounds(bbox, true);
        }
        else if (m_face->type == Face::FaceIndexType178)
        {
            mesh->sharedVertexData->vertexCount = m_face->faceDataList.size();
            Ogre::VertexDeclaration* decl = mesh->sharedVertexData->vertexDeclaration;
            Ogre::VertexBufferBinding* bind = mesh->sharedVertexData->vertexBufferBinding;

            size_t offset = 0;

            decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

            decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

            Ogre::AxisAlignedBox bbox;

            std::vector<common::PositionWithTexCoord> data;
            data.reserve(m_face->faceDataList.size());
            for (const auto& faceDataEntry : m_face->faceDataList)
            {
                const common::IndexWithPositionTexCoord fi178 = boost::get<common::IndexWithPositionTexCoord>(faceDataEntry);
                
                common::PositionWithTexCoord pt{fi178.position, fi178.texCoord};
                data.push_back(pt);

                bbox.merge(fi178.position);
            }

            Ogre::HardwareVertexBufferSharedPtr vbuf =
                Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                    offset, mesh->sharedVertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
            vbuf->writeData(0, vbuf->getSizeInBytes(), data.data(), true);
            bind->setBinding(0, vbuf);


            Ogre::SubMesh* sub = mesh->createSubMesh();
            sub->useSharedVertices = true;
            sub->operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;
            //sub->indexData->indexBuffer = ibuf;
            //sub->indexData->indexCount = 6;
            //sub->indexData->indexStart = 0;

            mesh->_setBounds(bbox, true);
        }
    }

    void LoadVertexData7(Ogre::Mesh* mesh)
    {
        mesh->sharedVertexData->vertexCount = m_groupVertex->vertices.size();
        Ogre::VertexDeclaration* decl = mesh->sharedVertexData->vertexDeclaration;
        Ogre::VertexBufferBinding* bind = mesh->sharedVertexData->vertexBufferBinding;

        size_t offset = 0;

        decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
        offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

        decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
        offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

        Ogre::AxisAlignedBox bbox;

        for (const auto& vertex : m_groupVertex->vertices)
        {
            bbox.merge(vertex.position);
        }

        Ogre::HardwareVertexBufferSharedPtr vbuf =
            Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                offset, mesh->sharedVertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
        vbuf->writeData(0, vbuf->getSizeInBytes(), m_groupVertex->vertices.data(), true);
        bind->setBinding(0, vbuf);

        using namespace codec::data::b3d::block_data;

        std::vector<common::Index> indices;
        std::uint32_t materialIndex = std::numeric_limits<std::uint32_t>::max();

        for (const auto& nestedBlock : m_groupVertex->nestedBlocks)
        {
            if (nestedBlock->header.type == 35)
            {
                const SimpleFaceData35& faceData = boost::get<const SimpleFaceData35&>(nestedBlock->data);

                for (const auto& meshData : faceData.meshList)
                {
                    materialIndex = meshData.materialIndex;

                    if (meshData.type == Mesh35::Indices1)
                    {
                        indices.reserve(meshData.meshDataList.size());
                        for (const auto& i : meshData.meshDataList)
                        {
                            indices.push_back(boost::get<common::Index>(i));
                        }
                    }
                    else
                    {
                        if ((meshData.type != Mesh35::UnknownType50) && (meshData.type != Mesh35::UnknownType48))
                        {
                            __debugbreak();
                        }
                    }
                }
            }
            else if (nestedBlock->header.type == 8)
            {
                const SimpleFaces8& faceData = boost::get<const SimpleFaces8&>(nestedBlock->data);
                for (const auto& face : faceData.faces)
                {
                    materialIndex = face.unknown2;
                    switch (face.type)
                    {
                    case Face8::UnknownType0:
                    case Face8::UnknownType1:
                    case Face8::UnknownType16:
                    case Face8::FaceIndexType129:
                    case Face8::UnknownType144:
                    {
                        for (const auto& fi : face.faceDataList)
                        {
                            indices.push_back(boost::get<common::Index>(fi));
                        }
                    }
                    break;

                    default:
                        //__debugbreak();
                        break;
                    }
                }
            }
            else
            {
                __debugbreak();
            }
        }


        Ogre::SubMesh* sub = mesh->createSubMesh();
        sub->useSharedVertices = true;
        mesh->_setBounds(bbox, true);

        if (indices.empty())
        {
            sub->operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;
        }
        else
        {
            Ogre::HardwareIndexBufferSharedPtr ibuf = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
                Ogre::HardwareIndexBuffer::IT_32BIT, 6, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
            ibuf->writeData(0, ibuf->getSizeInBytes(), indices.data(), true);

            sub->indexData->indexBuffer = ibuf;
            sub->indexData->indexCount = indices.size();
            sub->indexData->indexStart = 0;

            const std::string materialName = GetMaterialName(materialIndex);

            sub->setMaterialName(materialName, "D2");
        }
    }

    virtual void loadResource(Ogre::Resource* resource) override
    {
        Ogre::Mesh* mesh = static_cast<Ogre::Mesh*>(resource);
        mesh->sharedVertexData = OGRE_NEW Ogre::VertexData;

        if (m_face != nullptr)
        {
            LoadVertexData8(mesh);
        }
        else
        {
            LoadVertexData7(mesh);
        }
    }

    std::string GetMaterialName(const std::uint32_t materialIndex) const
    {
        return reinterpret_cast<const char* >(m_materials[materialIndex].data());
    }
};


struct BSVisitor : public codec::data::b3d::B3dVisitorInterface
{
    explicit BSVisitor(const char* b3dName)
        : m_b3dName(b3dName)
    {
    }

    std::string m_b3dName;
    Ogre::SceneManager* m_sceneManager = nullptr;
    Ogre::SceneNode* m_rootNode = nullptr;
    B3dMeshLoader* m_meshLoader = nullptr;
    Ogre::MeshManager* m_meshManager = nullptr;

    size_t m_cnt = 0;

    virtual void VisitPosition(const common::Position& position) override
    {
        m_cnt += 1;

        const std::string unique_id = "::" + m_b3dName + "::" + std::to_string(m_cnt);

        Ogre::Entity* cube = m_sceneManager->createEntity("Cube_V" + unique_id, Ogre::SceneManager::PT_CUBE);

        const std::string materialName = "PlaneMaterialForVector_" + m_b3dName.substr(m_b3dName.size() - 6);
        cube->setMaterialName(materialName, "D2");

        Ogre::SceneNode* headNode = m_rootNode->createChildSceneNode("CubeNode_V" + unique_id);
        headNode->attachObject(cube);

        headNode->setPosition(position);

        float newScale = 0.1f / cube->getBoundingRadius();
        headNode->setScale(newScale, newScale, newScale);
    }

    virtual void VisitMaterials(const codec::data::b3d::Materials& materials) override
    {
        m_meshLoader->m_materials = materials;
    }

    virtual void VisitBlockData(const codec::data::b3d::block_data::SimpleFaces8& blockData) override
    {
        for (const auto& face : blockData.faces)
        {
            m_cnt += 1;

            m_meshLoader->m_face = &face;

            Ogre::MeshPtr mesh = m_meshManager->createManual("face8" + GetUniqueId(), "D2", m_meshLoader);

            Ogre::Entity* meshEntity = m_sceneManager->createEntity("face8::entity" + GetUniqueId(), mesh);
            //meshEntity->setMaterialName("SampleMaterialForMesh", "D2");

            Ogre::SceneNode* meshSceneNode = m_rootNode->createChildSceneNode("face8::scene_node" + GetUniqueId());
            meshSceneNode->attachObject(meshEntity);

            m_meshLoader->m_face = nullptr;
        }
    }

    virtual void VisitBlockData(const codec::data::b3d::block_data::GroupVertex7& blockData) override
    {
        m_cnt += 1;

        m_meshLoader->m_groupVertex = &blockData;

        Ogre::MeshPtr mesh = m_meshManager->createManual("gv7" + GetUniqueId(), "D2", m_meshLoader);

        Ogre::Entity* meshEntity = m_sceneManager->createEntity("gv7::entity" + GetUniqueId(), mesh);
        //meshEntity->setMaterialName("SampleMaterialForMesh", "D2");

        Ogre::SceneNode* meshSceneNode = m_rootNode->createChildSceneNode("gv7::scene_node" + GetUniqueId());
        meshSceneNode->attachObject(meshEntity);

        m_meshLoader->m_groupVertex = nullptr;
    }

    virtual void VisitBoundingSphere(const common::BoundingSphere& boundingSphere) override
    {
        m_cnt += 1;

        Ogre::Entity* cube = m_sceneManager->createEntity("Cube_BS" + GetUniqueId(), Ogre::SceneManager::PT_CUBE);
        
        cube->setMaterialName("PlaneMaterialWithBlending", "D2");

        Ogre::SceneNode* headNode = m_rootNode->createChildSceneNode("CubeNode_BS" + GetUniqueId());
        headNode->attachObject(cube);

        headNode->setPosition(boundingSphere.center);

        float newScale = 1 / cube->getBoundingRadius();
        headNode->setScale(newScale, newScale, newScale);
    }

    std::string GetUniqueId() const
    {
        return "::" + m_b3dName + "::" + std::to_string(m_cnt);
    }
};

void SimpleB3dMeshRenderer::LoadB3d(const char* b3dName, Ogre::SceneNode* b3dSceneNode)
{
    BSVisitor visitor{b3dName};
    visitor.m_sceneManager = m_sceneManager;
    visitor.m_rootNode = b3dSceneNode;
    visitor.m_meshLoader = OGRE_NEW B3dMeshLoader;
    visitor.m_meshManager = mRoot->getMeshManager();


    std::ifstream inputFile{b3dName, std::ios_base::binary};
    if (!inputFile)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_FILE_NOT_FOUND, "failed to open file");
    }

    codec::data::b3d::B3d b3d;
    codec::data::b3d::B3dReader reader;

    reader.Read(inputFile, b3d, &visitor);
}







#if 0

struct MyMeshLoader : public Ogre::ManualResourceLoader
{
    virtual void prepareResource(Ogre::Resource* resource) override
    {
        Ogre::Mesh* mesh = static_cast<Ogre::Mesh*>(resource);
        (void)mesh;


    }

    void LoadVertexData(Ogre::Mesh* mesh)
    {
        //! [manual_plane_geometry]
        float vertices[32] = {
            -100, -100, -100,  // pos
            0,0,1,          // normal
            0,1,            // texcoord
            100, -100, -100,
            0,0,1,
            1,1,
            100,  100, -100,
            0,0,1,
            1,0,
            -100,  100, -100 ,
            0,0,1,
            0,0
        };

        std::uint16_t faces[6] = { 0,1,2,
                           0,2,3 };
        //! [manual_plane_geometry]

        //! [vertex_data]
        mesh->sharedVertexData->vertexCount = 4;
        Ogre::VertexDeclaration* decl = mesh->sharedVertexData->vertexDeclaration;
        Ogre::VertexBufferBinding* bind = mesh->sharedVertexData->vertexBufferBinding;
        //! [vertex_data]

        //! [vertex_decl]
        size_t offset = 0;
        decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
        offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
        decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
        offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
        decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0);
        offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);
        //! [vertex_decl]

        //! [vertex_buffer]
        Ogre::HardwareVertexBufferSharedPtr vbuf =
            Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                offset, 4, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
        vbuf->writeData(0, vbuf->getSizeInBytes(), vertices, true);
        bind->setBinding(0, vbuf);

        Ogre::HardwareIndexBufferSharedPtr ibuf = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
            Ogre::HardwareIndexBuffer::IT_16BIT, 6, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
        ibuf->writeData(0, ibuf->getSizeInBytes(), faces, true);
        //! [vertex_buffer]

        //! [sub_mesh]
        Ogre::SubMesh* sub = mesh->createSubMesh();
        sub->useSharedVertices = true;
        sub->indexData->indexBuffer = ibuf;
        sub->indexData->indexCount = 6;
        sub->indexData->indexStart = 0;
        //! [sub_mesh]

        mesh->_setBounds(Ogre::AxisAlignedBox(-100, -100, -100, 100, 100, -100), true);
    }

    virtual void loadResource(Ogre::Resource* resource) override
    {
        Ogre::Mesh* mesh = static_cast<Ogre::Mesh*>(resource);

        mesh->sharedVertexData = OGRE_NEW Ogre::VertexData;

        LoadVertexData(mesh);
    }
};



void SimpleB3dMeshRenderer::LoadB3d()
{
    MyMeshLoader* mml = OGRE_NEW MyMeshLoader;
    Ogre::MeshPtr mesh = mRoot->getMeshManager()->createManual("aa.b3d", "D2", mml);

    //FillMeshWithB3d(mesh.get());

    Ogre::Entity* meshEntity = m_sceneManager->createEntity("aa.b3d", mesh);
    meshEntity->setMaterialName("SampleMaterialForMesh", "D2");

    Ogre::SceneNode* meshSceneNode = m_sceneManager->getRootSceneNode()->createChildSceneNode("hehehe");
    meshSceneNode->attachObject(meshEntity);
}

void SimpleB3dMeshRenderer::FillMeshWithB3d(Ogre::Mesh* /* mesh */)
{



}

#endif //0


} // namespace app
} // namespace d2_hack
