#include <d2_hack/resource/archive/ogre_material_provider.h>

#include <OgreException.h>

#include <boost/format.hpp>

#include <d2_hack/common/types.h>
#include <d2_hack/common/memory_mgmt.h>
#include <d2_hack/common/resource_mgmt.h>

#include <d2_hack/resource/manager/manager.h>

#include "res_material_parser.h"

namespace d2_hack
{
namespace resource
{
namespace archive
{
namespace res
{


static void FillMaterialWithColor(const MaterialDescriptor& md, const std::string& /* resId */, Ogre::Material& material)
{
    auto technique = material.createTechnique();
    auto pass = technique->createPass();

    pass->setLightingEnabled(true);
    pass->setAmbient(Ogre::ColourValue{ 0.1f, 0.3f, 0.1f, 1.0f });
    pass->setDiffuse(Ogre::ColourValue{ 0.2f, 0.2f, 0.2f, 1.0f });

    palette::PalettePtr plm = manager::Manager::getSingleton().Load(common::GetPaletteFileName("COMMON", "common"), common::DefaultResourceGroup);

    pass->setEmissive(plm->GetColor(md.index));
}

static void SetTextureAnimation(const MaterialDescriptor& md, Ogre::TextureUnitState* textureUnitState)
{
    if (md.rot)
    {
        textureUnitState->setRotateAnimation(*md.rot);
        // TODO: RotPoint is 0.5, 0.5 for each rotation, so skip it
    }

    if (md.wave)
    {
        textureUnitState->setTransformAnimation(Ogre::TextureUnitState::TextureTransformType::TT_ROTATE, Ogre::WaveformType::WFT_SINE);
    }

    if (md.move)
    {
        textureUnitState->setScrollAnimation(md.move->x, md.move->y);
    }
}

static void FillMaterialWithTexture(const MaterialDescriptor& md, const std::string& resId, Ogre::Material& material)
{
    auto technique = material.createTechnique();
    auto pass = technique->createPass();

    auto textureUnitState = pass->createTextureUnitState();

    std::string textureName = common::GetTextureFileName(resId, md.index);
    auto textureRes = Ogre::TextureManager::getSingleton().createOrRetrieve(textureName, common::DefaultResourceGroup);

    textureUnitState->setTexture(std::static_pointer_cast<Ogre::Texture>(textureRes.first));

    SetTextureAnimation(md, textureUnitState);
}


static void FillMaterialWithContent(const MaterialDescriptor& md, const std::string& resId, Ogre::Material& material)
{
    material.removeAllTechniques();

    switch (md.type)
    {
    case MaterialType::col:
        FillMaterialWithColor(md, resId, material);
        break;

    case MaterialType::tex:
    case MaterialType::ttx:
    case MaterialType::itx:
        FillMaterialWithTexture(md, resId, material);
        break;

    default:
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, std::format("Unknown material type: {}", static_cast<int>(md.type)));
    }
}


Ogre::MaterialPtr OgreMaterialProvider::CreateOrRetrieveMaterial(const std::string& materialName, const std::string& groupName)
{
    Ogre::MaterialManager& mgr = Ogre::MaterialManager::getSingleton();
    auto res = mgr.createOrRetrieve(materialName, groupName, true);

    auto material = std::static_pointer_cast<Ogre::Material>(res.first);
    if (res.second)
    {
        Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(materialName, common::DefaultResourceGroup);

        auto content = stream->getAsString();
        MaterialDescriptor md = ParseMaterialDescriptor(content);

        std::string resId;
        common::SplitResourceFileName(materialName, &resId);

        FillMaterialWithContent(md, resId, *material); // TODO: possible Race Condition in multithreaded env
    }

    return material;
}


} // namespace res
} //namespace archive
} // namespace resource
} // namespace d2_hack  
