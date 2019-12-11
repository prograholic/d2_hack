#include "application.h"

#include <OgreEntity.h>
#include <OgreMeshManager.h>
#include <OgreMesh.h>
#include <OgreSubMesh.h>

namespace d2_hack
{

Application::Application()
    : BaseApplication()
{
}

void Application::CreateScene()
{
    m_sceneManager->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

    Ogre::Entity* plane = m_sceneManager->createEntity("Plane", Ogre::SceneManager::PT_PLANE);

    plane->setMaterialName("PlaneMaterial", "D2");

    Ogre::SceneNode* headNode = m_sceneManager->getRootSceneNode()->createChildSceneNode("HeadNode");
    headNode->attachObject(plane);

    Ogre::Light* light = m_sceneManager->createLight("MainLight");
    Ogre::SceneNode* lightSceneNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
    lightSceneNode->attachObject(light);
    lightSceneNode->setPosition(20.0f, 80.0f, 150.0f);

    CreateTerrain();

    LoadB3d();
}

void Application::CreateTerrain()
{
    m_camera->setNearClipDistance(0.1f);
    m_camera->setFarClipDistance(50000);

    if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
    {
        m_camera->setFarClipDistance(0);   // enable infinite far clip distance if we can
    }

    Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
    Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(7);

    Ogre::Vector3 lightdir(0.55f, -0.3f, 0.75f);
    lightdir.normalise();

    Ogre::Light* light = m_sceneManager->createLight("tstLight");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDirection(lightdir);
    light->setDiffuseColour(Ogre::ColourValue::White);
    light->setSpecularColour(Ogre::ColourValue(0.4f, 0.4f, 0.4f));

    m_sceneManager->setAmbientLight(Ogre::ColourValue(0.2f, 0.2f, 0.2f));

    m_terrainGlobalOptions.reset(OGRE_NEW Ogre::TerrainGlobalOptions());

    m_terrainGroup.reset(OGRE_NEW Ogre::TerrainGroup(m_sceneManager, Ogre::Terrain::ALIGN_X_Z, 513, 1200.0f));
    m_terrainGroup->setFilenameConvention(Ogre::String("BasicTutorial3Terrain"), Ogre::String("dat"));
    m_terrainGroup->setOrigin(Ogre::Vector3::ZERO);

    ConfigureTerrainDefaults(light);

    for (long x = 0; x <= 0; ++x)
        for (long y = 0; y <= 0; ++y)
            DefineTerrain(x, y);

    // sync load since we want everything in place when we start
    m_terrainGroup->loadAllTerrains(true);

    Ogre::TerrainGroup::TerrainIterator ti = m_terrainGroup->getTerrainIterator();
    while (ti.hasMoreElements())
    {
        Ogre::Terrain* t = ti.getNext()->instance;
        InitBlendMaps(t);
    }

    m_terrainGroup->freeTemporaryResources();
}

void Application::ConfigureTerrainDefaults(Ogre::Light* light)
{
    // Configure global
    m_terrainGlobalOptions->setMaxPixelError(8);
    // testing composite map
    m_terrainGlobalOptions->setCompositeMapDistance(3000);

    // Important to set these so that the terrain knows what to use for derived (non-realtime) data
    m_terrainGlobalOptions->setLightMapDirection(light->getDerivedDirection());
    m_terrainGlobalOptions->setCompositeMapAmbient(m_sceneManager->getAmbientLight());
    m_terrainGlobalOptions->setCompositeMapDiffuse(light->getDiffuseColour());

    // Configure default import settings for if we use imported image
    Ogre::Terrain::ImportData& defaultimp = m_terrainGroup->getDefaultImportSettings();
    defaultimp.terrainSize = 513;
    defaultimp.worldSize = 1200.0f;
    defaultimp.inputScale = 60;
    defaultimp.minBatchSize = 65;
    defaultimp.maxBatchSize = 129;
    // textures

    defaultimp.layerList.resize(1);
    defaultimp.layerList[0].worldSize = 1200.0f;
    defaultimp.layerList[0].textureNames.push_back("terrain0.raw2");
    defaultimp.layerList[0].textureNames.push_back("terrain0.raw2");

    //defaultimp.layerList[1].worldSize = 1200.0f;
    //defaultimp.layerList[1].textureNames.push_back("terrain1.raw2");
    //defaultimp.layerList[1].textureNames.push_back("terrain1.raw2");

    //defaultimp.layerList[2].worldSize = 1200.0f;
    //defaultimp.layerList[2].textureNames.push_back("terrain2.raw2");
    //defaultimp.layerList[2].textureNames.push_back("terrain2.raw2");

    #if 0
    defaultimp.layerList.resize(3);
    defaultimp.layerList[0].worldSize = 100;
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
    defaultimp.layerList[1].worldSize = 30;
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
    defaultimp.layerList[2].worldSize = 200;
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
#endif //0
}

void Application::InitBlendMaps(Ogre::Terrain* /* terrain */)
{
#if 0
    Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
    Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
    Ogre::Real minHeight0 = 70;
    Ogre::Real fadeDist0 = 40;
    Ogre::Real minHeight1 = 70;
    Ogre::Real fadeDist1 = 15;
    float* pBlend0 = blendMap0->getBlendPointer();
    float* pBlend1 = blendMap1->getBlendPointer();
    for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
    {
        for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
        {
            Ogre::Real tx, ty;

            blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
            Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
            Ogre::Real val = (height - minHeight0) / fadeDist0;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend0++ = val;

            val = (height - minHeight1) / fadeDist1;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend1++ = val;
        }
    }
    blendMap0->dirty();
    blendMap1->dirty();
    blendMap0->update();
    blendMap1->update();
#endif //0
}

void Application::DefineTerrain(long x, long y)
{
    Ogre::String filename = m_terrainGroup->generateFilename(x, y);
    if (Ogre::ResourceGroupManager::getSingleton().resourceExists(m_terrainGroup->getResourceGroup(), filename))
    {
        m_terrainGroup->defineTerrain(x, y);
    }
    else
    {
        Ogre::Image img;
        img.load("terrain0.raw2", "D2");
        m_terrainGroup->defineTerrain(x, y, &img);
    }
}


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
            -100, -100, 100,  // pos
            0,0,1,          // normal
            0,1,            // texcoord
            100, -100, 100,
            0,0,1,
            1,1,
            100,  100, 100,
            0,0,1,
            1,0,
            -100,  100, 100 ,
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

        mesh->_setBounds(Ogre::AxisAlignedBox(-100, -100, 100, 100, 100, 100), true);
    }

    void LoadVertexDataDeclaration(Ogre::Mesh* mesh)
    {
        mesh->sharedVertexData->vertexDeclaration->addElement(0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
    }

    virtual void loadResource(Ogre::Resource* resource) override
    {
        Ogre::Mesh* mesh = static_cast<Ogre::Mesh*>(resource);

        mesh->sharedVertexData = OGRE_NEW Ogre::VertexData;

        LoadVertexDataDeclaration(mesh);
        LoadVertexData(mesh);
    }
};

void Application::LoadB3d()
{
    MyMeshLoader* mml = OGRE_NEW MyMeshLoader;
    Ogre::MeshPtr mesh = mRoot->getMeshManager()->createManual("aa.b3d", "D2", mml);

    //FillMeshWithB3d(mesh.get());

    Ogre::Entity* meshEntity = m_sceneManager->createEntity("aa.b3d", mesh);
    meshEntity->setMaterialName("SampleMaterialForMesh", "D2");

    Ogre::SceneNode* meshSceneNode = m_sceneManager->getRootSceneNode()->createChildSceneNode("hehehe");
    meshSceneNode->attachObject(meshEntity);
}

void Application::FillMeshWithB3d(Ogre::Mesh* /* mesh */)
{
    


}


} // namespace d2_hack
