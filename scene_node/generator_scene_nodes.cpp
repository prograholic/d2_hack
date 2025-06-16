#include <d2_hack/scene_node/generator_scene_nodes.h>

#include <OgreMeshManager.h>
#include <OgreEntity.h>

#include <d2_hack/common/utils.h>
#include <d2_hack/common/resource_mgmt.h>
#include <d2_hack/common/log.h>

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

TerrainSceneNode::TerrainSceneNode(const std::string_view& name, Ogre::SceneNode* /* ogreSceneNode */, Ogre::SceneManager* sceneManager)
    : OgreSceneNode<block_data::SimpleGeneratedObjectsBlock40>(name, nullptr)
    , m_terrainGlobalOptions(std::make_unique<Ogre::TerrainGlobalOptions>())
    , m_terrainGroup(std::make_unique<Ogre::TerrainGroup>(sceneManager, Ogre::Terrain::ALIGN_X_Z, TerrainSize, TerrainWorldSize))
{
    m_terrainGroup->setResourceGroup(common::DefaultResourceGroup);
    CreateTerrain();
    m_terrainGroup->setOrigin(TerrainOrigin);
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


TreeGeneratorSceneNode::TreeGeneratorSceneNode(
    const std::string_view& name,
    const block_data::SimpleGeneratedObjects40& data,
    const std::string_view& b3dId,
    Ogre::SceneNode* ogreSceneNode,
    Ogre::SceneManager* sceneManager,
    resource::archive::res::OgreMaterialProvider* ogreMaterialProvider)
    : OgreSceneNode<block_data::SimpleGeneratedObjectsBlock40>(name, nullptr)
{
    TreeParams treeParams = DeduceTreeParams(data, b3dId);

    Ogre::MeshPtr treeMesh = Ogre::MeshManager::getSingleton().getByName(treeParams.meshName);

    if (!treeMesh)
    {
        D2_HACK_LOG(TreeGeneratorSceneNode::TreeGeneratorSceneNode) << "cannot find mesh \"" << treeParams.meshName << "\", create new...";
        treeMesh = CreateMesh(treeParams, sceneManager);
    }
    else
    {
        D2_HACK_LOG(TreeGeneratorSceneNode::TreeGeneratorSceneNode) << "reuse mesh \"" << treeParams.meshName << "\"";
    }

    Ogre::Entity* e = sceneManager->createEntity(treeMesh);
    auto material = ogreMaterialProvider->CreateOrRetrieveMaterial(treeParams.materialName, common::DefaultResourceGroup);
    if (material)
    {
        e->setMaterial(material);
    }
    else
    {
        D2_HACK_LOG(TreeGeneratorSceneNode::TreeGeneratorSceneNode) << "Cannot find material \"" << treeParams.materialName << "\"";
    }
    

    ogreSceneNode->attachObject(e);
    ogreSceneNode->setPosition(treeParams.location);
    ogreSceneNode->pitch(Ogre::Radian(Ogre::Degree(90)), Ogre::Node::TransformSpace::TS_WORLD);
}

Ogre::MeshPtr TreeGeneratorSceneNode::CreateMesh(const TreeGeneratorSceneNode::TreeParams& treeParams, Ogre::SceneManager* sceneManager)
{
    Ogre::ManualObject* obj = sceneManager->createManualObject();

    obj->estimateVertexCount(16);
    obj->estimateIndexCount(48);

    obj->begin(treeParams.materialName, Ogre::RenderOperation::OT_TRIANGLE_LIST, common::DefaultResourceGroup);

    obj->position(treeParams.scale / 5 - 1, 0, 0);
    obj->textureCoord(treeParams.textureScale - 0.49f, 1.0f);

    obj->position(treeParams.scale / 5, 0, 0);
    obj->textureCoord(treeParams.textureScale - 0.05f, 1.0f);

    obj->position(treeParams.scale / 5 - 1, treeParams.scale, 0);
    obj->textureCoord(treeParams.textureScale - 0.49f, 0);

    obj->position(treeParams.scale / 5, treeParams.scale, 0);
    obj->textureCoord(treeParams.textureScale - 0.05f, 0);

    obj->position(0, 0, treeParams.scale / 5 * -1);
    obj->textureCoord(treeParams.textureScale - 0.05f, 1.0f);

    obj->position(0, 0, treeParams.scale / 5);
    obj->textureCoord(treeParams.textureScale - 0.49f, 1.0f);

    obj->position(0, treeParams.scale, treeParams.scale / 5 * -1);
    obj->textureCoord(treeParams.textureScale - 0.05f, 0);

    obj->position(0, treeParams.scale, treeParams.scale / 5);
    obj->textureCoord(treeParams.textureScale - 0.49f, 0);

    obj->position(treeParams.scale / 7 * -1, 0, treeParams.scale / 7 * -1);
    obj->textureCoord(treeParams.textureScale - 0.49f, 1.0f);

    obj->position(treeParams.scale / 7, 0, treeParams.scale / 7);
    obj->textureCoord(treeParams.textureScale - 0.05f, 1.0f);

    obj->position(treeParams.scale / 7 * -1, treeParams.scale, treeParams.scale / 7 * -1);
    obj->textureCoord(treeParams.textureScale - 0.49f, 0);

    obj->position(treeParams.scale / 7, treeParams.scale, treeParams.scale / 7);
    obj->textureCoord(treeParams.textureScale - 0.05f, 0);

    obj->position(treeParams.scale / 7 * -1, 0, treeParams.scale / 7);
    obj->textureCoord(treeParams.textureScale - 0.05f, 1.0f);

    obj->position(treeParams.scale / 7, 0, treeParams.scale / 7 * -1);
    obj->textureCoord(treeParams.textureScale - 0.49f, 1.0f);

    obj->position(treeParams.scale / 7 * -1, treeParams.scale, treeParams.scale / 7);
    obj->textureCoord(treeParams.textureScale - 0.05f, 0);

    obj->position(treeParams.scale / 7, treeParams.scale, treeParams.scale / 7 * -1);
    obj->textureCoord(treeParams.textureScale - 0.49f, 0);

    ///////////////////////////////////////////////////////////////////////////

    obj->triangle(0, 1, 2);
    obj->triangle(3, 2, 1);
    obj->triangle(0, 2, 1);
    obj->triangle(3, 1, 2);
    obj->triangle(4, 5, 6);
    obj->triangle(7, 6, 5);
    obj->triangle(4, 6, 5);
    obj->triangle(7, 5, 6);
    obj->triangle(8, 9, 10);
    obj->triangle(11, 10, 9);
    obj->triangle(8, 10, 9);
    obj->triangle(11, 9, 10);
    obj->triangle(12, 13, 14);
    obj->triangle(15, 14, 13);
    obj->triangle(12, 14, 13);
    obj->triangle(15, 13, 14);

    ///////////////////////////////////////////////////////////////////////////

    obj->end();

    auto res = obj->convertToMesh(treeParams.meshName);

    sceneManager->destroyManualObject(obj);

    return res;
}


static void CalculateMaterialName(std::uint32_t type, std::string& materialName, std::string& extraMaterialName)
{
    const std::uint32_t subType = (type >> 4) & 0x0f;
    if ((subType % 2) == 0)
    {
        materialName = std::format("tree{}{}", subType, subType + 1);
    }
    else
    {
        materialName = std::format("tree{}{}", subType - 1, subType);
    }

    if ((type % 2) != 0)
    {
        extraMaterialName = "tree" + std::to_string(subType);
    }
    else
    {
        extraMaterialName = "";
    }


}

TreeGeneratorSceneNode::TreeParams TreeGeneratorSceneNode::DeduceTreeParams(const block_data::SimpleGeneratedObjects40& data, const std::string_view& b3dId)
{
    TreeParams res;

    res.scale = data.boundingSphere.radius;
    res.location = data.boundingSphere.center;
    res.textureScale = 1.0f;

    CalculateMaterialName(data.unknown0.type, res.materialName, res.extraMaterialName);

    const std::uint32_t scaleType = (data.unknown0.type >> 8) & 0x0f;
    switch (scaleType)
    {
    case 0:
        res.textureScale = 0.5f;
        break;

    case 1:
        res.textureScale = 0.75f;
        break;

    case 2:
        res.textureScale = 0.6666667f;
        break;

    case 3:
        res.textureScale = 0.8f;
        break;

    case 4:
        res.textureScale = 0.8f;
        break;

    case 5:
        res.textureScale = 0.55f;
        break;

    case 6:
        res.textureScale = 1.0;
        break;

    default:
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, std::format("Unknown scale type {}, type: {}", scaleType, data.unknown0.type));
    }

    D2_HACK_LOG(TreeGeneratorSceneNode::DeduceTreeParams) << 
        "data.unknown0.type: 0x" << std::hex << data.unknown0.type << ", "
        "material name: " << res.materialName << ", "
        "extra material name: " << res.extraMaterialName;


    res.meshName = "tree_mesh_" + res.materialName + "_" + std::to_string(res.scale) + "_" + std::to_string(res.textureScale);
    res.materialName = common::GetMaterialFileName(b3dId, res.materialName);
    res.extraMaterialName = common::GetMaterialFileName(b3dId, res.extraMaterialName);

    return res;
}




} // namespace scene_node
} // namespace d2_hack
