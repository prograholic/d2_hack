#include <d2_hack/scene_node/generator_scene_nodes.h>

#include <d2_hack/common/resource_mgmt.h>

namespace d2_hack
{
namespace scene_node
{

using namespace resource::data::b3d;

const std::uint16_t TerrainSize = 257;
const Ogre::Real TerrainWorldSize = 1000.0f;
const Ogre::Vector3 TerrainOrigin{ 2548.0f, 425.0f, 5490.0 };
const char* HeightMap0Name = "terrain0.raw2";
const char* HeightMap1Name = "terrain1.raw2";
const char* HeightMap2Name = "terrain2.raw2";


TerrainSceneNode::TerrainSceneNode(const std::string_view& name, Ogre::SceneManager* sceneManager)
    : SceneNode<block_data::SimpleGeneratedObjectsBlock40, SceneNodeBase>(name)
    , m_terrainGlobalOptions(std::make_unique<Ogre::TerrainGlobalOptions>())
    , m_terrainGroup(std::make_unique<Ogre::TerrainGroup>(sceneManager, Ogre::Terrain::ALIGN_X_Z, TerrainSize, TerrainWorldSize))
{
    m_terrainGroup->setResourceGroup(common::DefaultResourceGroup);
    CreateTerrain();
    m_terrainGroup->setOrigin(TerrainOrigin);
}

Ogre::Vector3f TerrainSceneNode::GetAbsolutePosition() const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "TerrainSceneNode::GetAbsolutePosition is not implemented");
}

Ogre::Quaternion TerrainSceneNode::GetAbsoluteOrientation() const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "TerrainSceneNode::GetAbsoluteOrientation is not implemented");
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
    m_terrainGlobalOptions->setCompositeMapDistance(3000);

    // Important to set these so that the terrain knows what to use for derived (non-realtime) data
    //m_terrainGlobalOptions->setLightMapDirection(light->getDerivedDirection());
    //m_terrainGlobalOptions->setCompositeMapAmbient(m_sceneManager->getAmbientLight());
    //m_terrainGlobalOptions->setCompositeMapDiffuse(light->getDiffuseColour());

    // Configure default import settings for if we use imported image
    Ogre::Terrain::ImportData& defaultimp = m_terrainGroup->getDefaultImportSettings();
    defaultimp.terrainSize = TerrainSize;
    defaultimp.worldSize = TerrainWorldSize;
    defaultimp.inputScale = TerrainWorldSize / 10;
    defaultimp.minBatchSize = 33;
    defaultimp.maxBatchSize = 65;

    // textures
    defaultimp.layerList.resize(1);
    defaultimp.layerList[0].worldSize = TerrainWorldSize / 3;

    std::string textureName = common::GetTextureFileName("aa", 12); //ter000.txr
    defaultimp.layerList[0].textureNames.push_back(textureName);
    defaultimp.layerList[0].textureNames.push_back(textureName); // FIXME: find proper texture???
}

void TerrainSceneNode::DefineTerrains()
{
    // TODO: reorganize image rotating
    Ogre::Image img0;
    img0.load(HeightMap0Name, common::DefaultResourceGroup);

    const std::uint32_t borderSize = img0.getWidth();

    img0.flipAroundX();

    Ogre::Image img1;
    img1.load(HeightMap1Name, common::DefaultResourceGroup);
    img1.flipAroundX();


    Ogre::Image img2;
    img2.load(HeightMap2Name, common::DefaultResourceGroup);

    std::vector<float> yyy1;
    yyy1.resize(borderSize * borderSize);
    const std::uint16_t* data0 = reinterpret_cast<const std::uint16_t*>(img0.getData());
    const std::uint16_t* data1 = reinterpret_cast<const std::uint16_t*>(img1.getData());
    for (std::uint32_t x = 0; x != borderSize; ++x)
    {
        for (std::uint32_t y = 0; y != borderSize; ++y)
        {
            auto pos = x * borderSize + y;
            yyy1[pos] = static_cast<float>(((x + y) > borderSize) ? data1[pos] : data0[pos]) / std::numeric_limits<std::uint16_t>::max();
        }
    }


    std::vector<float> yyy2;
    yyy2.resize(borderSize * borderSize);
    for (std::uint32_t x = 0; x != borderSize; ++x)
    {
        for (std::uint32_t y = 0; y != borderSize; ++y)
        {
            auto pos = x * borderSize + y;
            yyy2[pos] = static_cast<float>(((x + y) < borderSize) ? data1[pos] : data0[pos]) / std::numeric_limits<std::uint16_t>::max();
        }
    }

    img0.flipAroundX();
    common::RotateImageDataClockWise(reinterpret_cast<std::uint16_t*>(img0.getData()), borderSize);
    common::RotateImageDataClockWise(reinterpret_cast<std::uint16_t*>(img2.getData()), borderSize);
    common::RotateImageDataCounterClockWise(yyy1.data(), borderSize);
    common::RotateImageDataCounterClockWise(yyy2.data(), borderSize);

    m_terrainGroup->defineTerrain(0, 2, yyy1.data());

    m_terrainGroup->defineTerrain(2, 1, &img2);
    m_terrainGroup->defineTerrain(1, 2, &img2);

    m_terrainGroup->defineTerrain(1, 1, &img0);
    m_terrainGroup->defineTerrain(1, 0, &img2);
    m_terrainGroup->defineTerrain(0, 1, &img2);

    m_terrainGroup->defineTerrain(2, 0, yyy2.data());
}

///////////////////////////////////////////////////////////////////////////////

TreeGeneratorSceneNode::TreeGeneratorSceneNode(const std::string_view& name)
    : SceneNode<block_data::SimpleGeneratedObjectsBlock40, SceneNodeBase>(name)
{
}

Ogre::Vector3f TreeGeneratorSceneNode::GetAbsolutePosition() const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "TreeGeneratorSceneNode::GetAbsolutePosition is not implemented");
}

Ogre::Quaternion TreeGeneratorSceneNode::GetAbsoluteOrientation() const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "TreeGeneratorSceneNode::GetAbsoluteOrientation is not implemented");
}

} // namespace scene_node
} // namespace d2_hack
