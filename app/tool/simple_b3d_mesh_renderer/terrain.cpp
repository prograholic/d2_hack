#include "terrain.h"

namespace d2_hack
{
namespace app
{

Terrain::Terrain(Ogre::SceneManager* sceneManager)
    : m_terrainGlobalOptions(std::make_unique<Ogre::TerrainGlobalOptions>())
    , m_terrainGroup(std::make_unique<Ogre::TerrainGroup>(sceneManager, Ogre::Terrain::ALIGN_X_Z, 513, 1200.0f))
{
    CreateTerrain();
}

void Terrain::CreateTerrain()
{
    ConfigureTerrainDefaults();

    DefineTerrain(0, 0);

    // sync load since we want everything in place when we start
    m_terrainGroup->loadAllTerrains(true);
    m_terrainGroup->freeTemporaryResources();
}

void Terrain::ConfigureTerrainDefaults()
{
    m_terrainGlobalOptions->setMaxPixelError(8);
    // testing composite map
    m_terrainGlobalOptions->setCompositeMapDistance(3000);

    // Important to set these so that the terrain knows what to use for derived (non-realtime) data
    //m_terrainGlobalOptions->setLightMapDirection(light->getDerivedDirection());
    //m_terrainGlobalOptions->setCompositeMapAmbient(m_sceneManager->getAmbientLight());
    //m_terrainGlobalOptions->setCompositeMapDiffuse(light->getDiffuseColour());

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
}

void Terrain::DefineTerrain(long x, long y)
{
    Ogre::Image img;
    img.load("terrain0.raw2", "D2");
    m_terrainGroup->defineTerrain(x, y, &img);
}


} // namespace app
} // namespace d2_hack
