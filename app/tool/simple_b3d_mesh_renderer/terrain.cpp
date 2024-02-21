#include "terrain.h"

namespace d2_hack
{
namespace app
{

const std::uint16_t TerrainSize = 513;
const Ogre::Real TerrainWorldSize = 3000.0f;
const char* HeightMap0Name = "terrain0.raw2";
const char* HeightMap1Name = "terrain1.raw2";
const char* HeightMap2Name = "terrain2.raw2";

Terrain::Terrain(Ogre::SceneManager* sceneManager)
    : m_terrainGlobalOptions(std::make_unique<Ogre::TerrainGlobalOptions>())
    , m_terrainGroup(std::make_unique<Ogre::TerrainGroup>(sceneManager, Ogre::Terrain::ALIGN_X_Z, TerrainSize, TerrainWorldSize))
{
    m_terrainGroup->setResourceGroup("D2");
    CreateTerrain();
}

Ogre::Vector3 Terrain::GetPosition() const
{
    return m_terrainGroup->getOrigin();
}

void Terrain::SetPosition(const Ogre::Vector3& pos)
{
    m_terrainGroup->setOrigin(pos);
}

void Terrain::CreateTerrain()
{
    SetPosition(Ogre::Vector3{3300.0f, 300.0f, 4500.0f});

    ConfigureTerrainDefaults();

    DefineTerrains();

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
    defaultimp.terrainSize = TerrainSize;
    defaultimp.worldSize = TerrainWorldSize;
    defaultimp.inputScale = 600;
    defaultimp.minBatchSize = 65;
    defaultimp.maxBatchSize = 129;

    // textures
    defaultimp.layerList.resize(1);
    defaultimp.layerList[0].worldSize = TerrainWorldSize / 10;
    
    defaultimp.layerList[0].textureNames.push_back("aa\\txr\\ter000.txr");
    defaultimp.layerList[0].textureNames.push_back(HeightMap0Name);
    //defaultimp.layerList[0].textureNames.push_back("aa\\txr\\ter000.txr");
}

void Terrain::DefineTerrains()
{
    Ogre::Image img0;
    img0.load(HeightMap0Name, "D2");
    img0.flipAroundX();
    img0.flipAroundY();
    

    Ogre::Image img1;
    img1.load(HeightMap1Name, "D2");
    img1.flipAroundX();
    img1.flipAroundY();


    Ogre::Image img2;
    img2.load(HeightMap2Name, "D2");
    img2.flipAroundX();
    img2.flipAroundY();

    m_terrainGroup->defineTerrain(0, 0, &img1);
    m_terrainGroup->defineTerrain(0, 1, &img2);
    m_terrainGroup->defineTerrain(1, 0, &img2);
    m_terrainGroup->defineTerrain(1, 1, &img0);
    m_terrainGroup->defineTerrain(1, 2, &img2);
    m_terrainGroup->defineTerrain(2, 1, &img2);
    m_terrainGroup->defineTerrain(2, 2, &img0);
}


} // namespace app
} // namespace d2_hack
