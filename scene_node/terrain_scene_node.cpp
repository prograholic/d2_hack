#include <d2_hack/scene_node/terrain_scene_node.h>

namespace d2_hack
{
namespace scene_node
{

const std::uint16_t TerrainSize = 257;
const Ogre::Real TerrainWorldSize = 1000.0f;
const Ogre::Vector3 TerrainOrigin{ 4550.0f, -5500.0f, 425.0f };
const char* HeightMap0Name = "terrain0.raw2";
const char* HeightMap1Name = "terrain1.raw2";
const char* HeightMap2Name = "terrain2.raw2";

TerrainSceneNode::TerrainSceneNode(const std::string& name, Ogre::SceneNode* ogreSceneNode, Ogre::SceneManager* sceneManager)
    : OgreSceneNode<resource::data::b3d::block_data::SimpleGeneratedObjectsBlock40>(name, ogreSceneNode)
    , m_terrainGlobalOptions(std::make_unique<Ogre::TerrainGlobalOptions>())
    , m_terrainGroup(std::make_unique<Ogre::TerrainGroup>(sceneManager, Ogre::Terrain::ALIGN_X_Y, TerrainSize, TerrainWorldSize, ogreSceneNode))
{
    m_terrainGroup->setResourceGroup("D2");
    CreateTerrain();
    ogreSceneNode->roll(Ogre::Degree(90.0f));
    ogreSceneNode->setPosition(TerrainOrigin);
}

void TerrainSceneNode::CreateTerrain()
{
    ConfigureTerrainDefaults();

    DefineTerrains();

    // sync load since we want everything in place when we start
    m_terrainGroup->loadAllTerrains(true);
    m_terrainGroup->freeTemporaryResources();
}

void TerrainSceneNode::ConfigureTerrainDefaults()
{
    m_terrainGlobalOptions->setMaxPixelError(8);
    // testing composite map
    m_terrainGlobalOptions->setCompositeMapDistance(300000);

    // Important to set these so that the terrain knows what to use for derived (non-realtime) data
    //m_terrainGlobalOptions->setLightMapDirection(light->getDerivedDirection());
    //m_terrainGlobalOptions->setCompositeMapAmbient(m_sceneManager->getAmbientLight());
    //m_terrainGlobalOptions->setCompositeMapDiffuse(light->getDiffuseColour());

    // Configure default import settings for if we use imported image
    Ogre::Terrain::ImportData& defaultimp = m_terrainGroup->getDefaultImportSettings();
    defaultimp.terrainSize = TerrainSize;
    defaultimp.worldSize = TerrainWorldSize;
    defaultimp.inputScale = TerrainWorldSize / 10;
    defaultimp.minBatchSize = 65;
    defaultimp.maxBatchSize = 129;

    // textures
    defaultimp.layerList.resize(1);
    defaultimp.layerList[0].worldSize = TerrainWorldSize / 3;
    
    defaultimp.layerList[0].textureNames.push_back("aa\\txr\\ter000.txr");
    defaultimp.layerList[0].textureNames.push_back("aa\\txr\\ter000.txr"); // FIXME: find proper texture???
}

void TerrainSceneNode::DefineTerrains()
{
    Ogre::Image img0;
    img0.load(HeightMap0Name, "D2");
    img0.flipAroundX();

    Ogre::Image img1;
    img1.load(HeightMap1Name, "D2");
    img1.flipAroundX();

    Ogre::Image img2;
    img2.load(HeightMap2Name, "D2");

    const std::uint32_t borderSize = img0.getWidth();

    std::vector<float> yyy;
    yyy.resize(borderSize * borderSize);
    const std::uint16_t* data0 = reinterpret_cast<const std::uint16_t*>(img0.getData());
    const std::uint16_t* data1 = reinterpret_cast<const std::uint16_t*>(img1.getData());
    for (std::uint32_t x = 0; x != borderSize; ++x)
    {
        for (std::uint32_t y = 0; y != borderSize; ++y)
        {
            auto pos = x * borderSize + y;
            yyy[pos] = static_cast<float>(((x + y) > borderSize) ? data1[pos] : data0[pos]) / std::numeric_limits<std::uint16_t>::max();
        }
    }

    m_terrainGroup->defineTerrain(2, 2, yyy.data());

    m_terrainGroup->defineTerrain(2, 1, &img2);
    m_terrainGroup->defineTerrain(1, 2, &img2);

    m_terrainGroup->defineTerrain(1, 1, &img0);
    m_terrainGroup->defineTerrain(1, 0, &img2);
    m_terrainGroup->defineTerrain(0, 1, &img2);
    
    for (std::uint32_t x = 0; x != borderSize; ++x)
    {
        for (std::uint32_t y = 0; y != borderSize; ++y)
        {
            auto pos = x * borderSize + y;
            yyy[pos] = static_cast<float>(((x + y) < borderSize) ? data1[pos] : data0[pos]) / std::numeric_limits<std::uint16_t>::max();
        }
    }
    m_terrainGroup->defineTerrain(0, 0, yyy.data());
}

} // namespace scene_node
} // namespace d2_hack
