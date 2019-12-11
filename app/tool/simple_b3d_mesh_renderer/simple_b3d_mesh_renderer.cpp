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

SimpleB3dMeshRenderer::SimpleB3dMeshRenderer()
    : BaseApplication()
{
}

void SimpleB3dMeshRenderer::CreateScene()
{
    m_sceneManager->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

    Ogre::Light* light = m_sceneManager->createLight("MainLight");
    Ogre::SceneNode* lightSceneNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
    lightSceneNode->attachObject(light);
    lightSceneNode->setPosition(20.0f, 80.0f, 150.0f);

    LoadB3d();
}


struct BSVisitor : public codec::data::B3dVisitorInterface
{
    Ogre::SceneManager* m_sceneManager = nullptr;
    size_t m_cnt = 0;

    virtual void VisitVector3(const Ogre::Vector3& vector) override
    {
        m_cnt += 1;

        const std::string unique_id = std::to_string(m_cnt);

        Ogre::Entity* plane = m_sceneManager->createEntity("Cube_V" + unique_id, Ogre::SceneManager::PT_CUBE);

        plane->setMaterialName("PlaneMaterialForVector", "D2");

        Ogre::SceneNode* headNode = m_sceneManager->getRootSceneNode()->createChildSceneNode("CubeNode_V" + unique_id);
        headNode->attachObject(plane);

        headNode->setPosition(vector);

        float newScale = 0.1f / plane->getBoundingRadius();
        headNode->setScale(newScale, newScale, newScale);
    }

    virtual void VisitBoundingSphere(const common::BoundingSphere& boundingSphere) override
    {
        m_cnt += 1;

        const std::string unique_id = std::to_string(m_cnt);

        Ogre::Entity* plane = m_sceneManager->createEntity("Cube_BS" + unique_id, Ogre::SceneManager::PT_CUBE);
        
        plane->setMaterialName("PlaneMaterialWithBlending", "D2");

        Ogre::SceneNode* headNode = m_sceneManager->getRootSceneNode()->createChildSceneNode("CubeNode_BS" + unique_id);
        headNode->attachObject(plane);

        headNode->setPosition(boundingSphere.center);

        float newScale = 1 / plane->getBoundingRadius();
        headNode->setScale(newScale, newScale, newScale);
    }
};

void SimpleB3dMeshRenderer::LoadB3d()
{
    BSVisitor visitor;
    visitor.m_sceneManager = m_sceneManager;


    std::ifstream inputFile{D2_AA_B3D_FILENAME, std::ios_base::binary};
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
