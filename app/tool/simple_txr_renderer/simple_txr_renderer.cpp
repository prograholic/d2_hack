#include "simple_txr_renderer.h"

#include <span>

#include <OgreEntity.h>

#include <d2_hack/common/types.h>
#include <d2_hack/common/resource_mgmt.h>

namespace d2_hack
{
namespace app
{

SimpleTxrRenderer::SimpleTxrRenderer()
    : BaseB3dApplication("SimpleTxrRenderer")
{
}

struct MaterialsInfoEntry
{
    std::string_view resId;
    std::span<const std::string_view> materials;
};

static const std::string_view materialsAA [] =
{
    "mat_water",// tex 16  att 56  \"wave\"",
    //"mat_bump_water tex 15  coord 2  att 55",
    //"mat_refl_water tex 14  env 0.07 0.07  notile  coord 3"
};

static const std::string_view materialsAK[] =
{
    "mat_river1",// tex 8  move -0.125 0  att 48 ",
    //"mat_bump_river1 tex 7  coord 2  att 47 ",
    //"mat_refl_river1 tex 6  coord 3 "
};

static const std::string_view materialsAF [] =
{
    "tree01",// ttx 2  col 256  notile ",

    "mat_water1",// tex 31 ",
    "mat_water2",// tex 32 ",
    "mat_water3",// tex 33 ",
    "mat_water4",// tex 34 ",
    //"mat_bump_water tex 10  move 0 - 0.05  coord 2  att 50",
    //"mat_refl_water tex 9  env 0.07 0.07  notile  coord 3"
};

static const std::string_view materialsBG[] =
{
    "mat_vent1",// ttx 44  RotPoint 0.5 0.5  rot 100 "
};

static const std::string_view materialTrucks[] =
{
    "Ka50_vint1",// tex 510  RotPoint 0.5 0.5  rot 390  notile ",
    "Ka50_vint2",// tex 510  RotPoint 0.5 0.5  rot -390  notile"
};
    

const MaterialsInfoEntry materialInfoEntries[] =
{
    {"aa", materialsAA},
    {"ak", materialsAK},
    {"af", materialsAF},
    {"bg", materialsBG},
    {"TRUCKS", materialTrucks}
};




using resource::archive::res::OgreMaterialProvider;
using namespace resource::data::b3d;

void SimpleTxrRenderer::CreateEntityForResource(Ogre::SceneNode* parent, const std::string_view& resId, const std::string_view& materialId)
{
    static int id = 0;
    Ogre::Entity* plane = m_sceneManager->createEntity(std::format("Plane {}", id), Ogre::SceneManager::PT_PLANE);

    auto childNode = parent->createChildSceneNode(std::string{materialId}, Ogre::Vector3f{220.0f * id - 1000.0f, 0.0f, -2000.0f});
    id += 1;

    childNode->attachObject(plane);


    auto materialName = common::GetMaterialFileName(resId, materialId);

    Ogre::MaterialPtr material = m_ogreMaterialProvider->CreateOrRetrieveMaterial(materialName, common::DefaultResourceGroup);

    plane->setMaterial(material);
}

void SimpleTxrRenderer::CreateScene()
{
    m_sceneManager->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

    Ogre::Light* light = m_sceneManager->createLight("MainLight");

    light->setSpecularColour(Ogre::ColourValue::Red);
    light->setDiffuseColour(Ogre::ColourValue::Green);


    auto rootNode = m_sceneManager->getRootSceneNode();

    Ogre::SceneNode* lightSceneNode = rootNode->createChildSceneNode();
    lightSceneNode->attachObject(light);
    lightSceneNode->setPosition(20.0f, 80.0f, 150.0f);

    for (const auto& entry: materialInfoEntries)
    {
        for (const auto& material : entry.materials)
        {
            CreateEntityForResource(rootNode, entry.resId, material);
        }
    }

    auto b3dSceneNode = rootNode->createChildSceneNode();


    B3dRegistry b3dRegistry
    {
        D2_ROOT_DIR,
        "ENV"
    };

    const std::string_view cars[] = 
    {
        { "Zil" }
    };

    CreateB3dScene(b3dRegistry, cars, b3dSceneNode);
}



} // namespace app
} // namespace d2_hack
