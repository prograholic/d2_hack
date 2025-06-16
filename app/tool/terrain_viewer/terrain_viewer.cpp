#include "terrain_viewer.h"

#include <d2_hack/common/types.h>

namespace d2_hack
{
namespace app
{

TerrainViewer::TerrainViewer()
    : BaseApplication()
{
}

void TerrainViewer::CreateScene()
{
    m_sceneManager->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

    Ogre::Light* light = m_sceneManager->createLight("MainLight");
    Ogre::SceneNode* lightSceneNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
    lightSceneNode->attachObject(light);
    lightSceneNode->setPosition(20.0f, 80.0f, 150.0f);

    CreateTerrain();
}

void TerrainViewer::shutdown()
{
    m_terrainGroup.reset();
    m_terrainGlobalOptions.reset();
    BaseApplication::shutdown();
}

void TerrainViewer::CreateTerrain()
{
    m_camera->setNearClipDistance(0.1f);
    m_camera->setFarClipDistance(50000);

    if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
    {
        m_camera->setFarClipDistance(0);   // enable infinite far clip distance if we can
    }

    Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
    Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(7);

    Ogre::Light* light = m_sceneManager->createLight("tstLight");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDiffuseColour(Ogre::ColourValue::White);
    light->setSpecularColour(Ogre::ColourValue(0.4f, 0.4f, 0.4f));

    Ogre::SceneNode* lightSceneNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
    lightSceneNode->attachObject(light);

    m_sceneManager->setAmbientLight(Ogre::ColourValue(0.2f, 0.2f, 0.2f));

    m_terrainGlobalOptions.reset(OGRE_NEW Ogre::TerrainGlobalOptions());

    m_terrainGroup.reset(OGRE_NEW Ogre::TerrainGroup(m_sceneManager, Ogre::Terrain::ALIGN_X_Z, 513, 1200.0f));
    m_terrainGroup->setFilenameConvention("BasicTutorial3Terrain", "dat");
    m_terrainGroup->setOrigin(Ogre::Vector3::ZERO);

    ConfigureTerrainDefaults(light);

    DefineTerrain(0, 0);

    // sync load since we want everything in place when we start
    m_terrainGroup->loadAllTerrains(true);

    //Ogre::TerrainGroup::TerrainIterator ti = m_terrainGroup->getTerrainIterator();
    //while (ti.hasMoreElements())
    //{
        //Ogre::Terrain* t = ti.getNext()->instance;
        //InitBlendMaps(t);
    //}

    m_terrainGroup->freeTemporaryResources();
}

void TerrainViewer::ConfigureTerrainDefaults(Ogre::Light* light)
{
    // Configure global
    m_terrainGlobalOptions->setMaxPixelError(8);
    // testing composite map
    m_terrainGlobalOptions->setCompositeMapDistance(3000);

    // Important to set these so that the terrain knows what to use for derived (non-realtime) data
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

void TerrainViewer::InitBlendMaps(Ogre::Terrain* /* terrain */)
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

void TerrainViewer::DefineTerrain(long x, long y)
{
    //Ogre::String filename = m_terrainGroup->generateFilename(x, y);
    //if (Ogre::ResourceGroupManager::getSingleton().resourceExists(m_terrainGroup->getResourceGroup(), filename))
    //{
        //m_terrainGroup->defineTerrain(x, y);
    //}
    //else
    {
        Ogre::Image img;
        img.load("terrain0.raw2", common::DefaultResourceGroup);
        m_terrainGroup->defineTerrain(x, y, &img);
    }
}

} // namespace app
} // namespace d2_hack
