#include <d2_hack/resource/archive/ogre_material_provider.h>

#include <OgreException.h>
#include <OgreGpuProgramManager.h>

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

struct Material
{
    std::string name;
    MaterialDescriptorList descriptors;
};

typedef std::vector<Material> MaterialList;


static MaterialList ParseMaterialDescriptors(const std::string_view& resId, const std::string& groupName, Ogre::ResourceGroupManager& rgManager)
{
    std::string resourcePattern = common::GetMaterialFileName(resId, "*");

    auto resourceNames = rgManager.findResourceNames(groupName, resourcePattern);

    MaterialList res;

    for (const auto& resourceName : *resourceNames)
    {
        MaterialDescriptor md = ParseMaterialDescriptor(rgManager.openResource(resourceName, groupName)->getAsString());
        if (res.empty())
        {
            res.push_back({resourceName, {md}});
        }
        else
        {
            if (md.coord)
            {
                if (*md.coord == 3)
                {
                    res.push_back({ {}, {md} });
                } else if (*md.coord == 2)
                {
                    res.back().descriptors.push_back(md);
                }
                else
                {
                    OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, std::format("Unknown texture layer index: {}", *md.coord));
                }
            }
            else
            {
                if (res.back().name.empty())
                {
                    Material& lastMaterial = res.back();
                    assert(lastMaterial.descriptors.size() == 2);
                    assert(*lastMaterial.descriptors[0].coord == 3);
                    assert(*lastMaterial.descriptors[1].coord == 2);

                    lastMaterial.name = resourceName;
                    lastMaterial.descriptors.push_back(md);

                    std::reverse(lastMaterial.descriptors.begin(), lastMaterial.descriptors.end());
                }
                else
                {
                    res.push_back({ resourceName, {md} });
                }
            }
        }
    }

    return res;
}


static void FillMaterialWithColor(const MaterialDescriptor& md, Ogre::Pass& pass)
{
    pass.setLightingEnabled(true);
    pass.setAmbient(Ogre::ColourValue{ 0.1f, 0.3f, 0.1f, 1.0f });
    pass.setDiffuse(Ogre::ColourValue{ 0.2f, 0.2f, 0.2f, 1.0f });

    palette::PalettePtr plm = manager::Manager::getSingleton().Load(common::GetPaletteFileName("COMMON", "common"), common::DefaultResourceGroup);

    pass.setEmissive(plm->GetColor(md.index));
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

static void SetTextureAlpha(const MaterialDescriptor& md, Ogre::Pass& pass)
{
    if (md.type != MaterialType::ttx)
    {
        return;
    }
    if (!md.col)
    {
        return;
    }

    auto& gpuPm = Ogre::GpuProgramManager::getSingleton();


    auto vertexGpuProgram = gpuPm.load("d2_ttx_alpha_vs", common::DefaultResourceGroup, "set_texture_alpha_by_color_vs.glsl", Ogre::GpuProgramType::GPT_VERTEX_PROGRAM, "glsl");
    auto fragmentGpuProgram = gpuPm.load("d2_ttx_alpha_fs", common::DefaultResourceGroup, "set_texture_alpha_by_color_fs.glsl", Ogre::GpuProgramType::GPT_FRAGMENT_PROGRAM, "glsl");

    pass.setGpuProgram(Ogre::GpuProgramType::GPT_VERTEX_PROGRAM, vertexGpuProgram);
    pass.setGpuProgram(Ogre::GpuProgramType::GPT_FRAGMENT_PROGRAM, fragmentGpuProgram);

    auto vertexParams = pass.getVertexProgramParameters();
    vertexParams->setNamedAutoConstant("modelViewMatrix", Ogre::GpuProgramParameters::ACT_WORLDVIEW_MATRIX);
    vertexParams->setNamedAutoConstant("projectionMatrix", Ogre::GpuProgramParameters::ACT_PROJECTION_MATRIX);


    auto fragmentParams = pass.getFragmentProgramParameters();

    palette::PalettePtr plm = manager::Manager::getSingleton().Load(common::GetPaletteFileName("COMMON", "common"), common::DefaultResourceGroup);
    auto alphaColor = plm->GetColor(*md.col - 1);
    fragmentParams->setNamedConstant("AlphaColor", Ogre::Vector3f{alphaColor.r, alphaColor.g, alphaColor.b});
    fragmentParams->setNamedConstant("MainTexture", 0);
}

static void FillMaterialWithTexture(const MaterialDescriptor& md, const std::string& resId, Ogre::Pass& pass)
{
    //pass->setSceneBlending(Ogre::SceneBlendType::SBT_TRANSPARENT_ALPHA); вроде работает на некоторых текстурах, но нужно смотреть в общем.

    auto textureUnitState = pass.createTextureUnitState();

    std::string textureName = common::GetTextureFileName(resId, md.index);
    auto textureRes = Ogre::TextureManager::getSingleton().createOrRetrieve(textureName, common::DefaultResourceGroup);

    textureUnitState->setTexture(std::static_pointer_cast<Ogre::Texture>(textureRes.first));

    SetTextureAnimation(md, textureUnitState);
    SetTextureAlpha(md, pass);
}


static void FillMaterialWithContent(const MaterialDescriptor& md, const std::string& resId, Ogre::Pass& pass)
{
    switch (md.type)
    {
    case MaterialType::col:
        FillMaterialWithColor(md, pass);
        break;

    case MaterialType::tex:
    case MaterialType::ttx:
    case MaterialType::itx:
        FillMaterialWithTexture(md, resId, pass);
        break;

    default:
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, std::format("Unknown material type: {}", static_cast<int>(md.type)));
    }
}

class OgreMaterialProviderImpl
{
public:

    void PopulateMaterial(const std::string& materialName, const std::string& groupName, Ogre::Material& material)
    {
        std::string resId;
        common::SplitResourceFileName(materialName, &resId);

        const auto& d2Materials = GetMaterialDescriptorsFromCache(resId, groupName);

        material.removeAllTechniques();

        auto technique = material.createTechnique();

        const auto& d2Material = GetD2Material(d2Materials, materialName);

        size_t i = 0; // TODO: мне нужны PS (и возможно VS) для того чтобы нормально разобраться с bump mapping и reflection

        const auto& md = d2Material.descriptors[i % d2Material.descriptors.size()];
        //for (const auto& md : d2Material.descriptors)
        {
            auto pass = technique->createPass();
            FillMaterialWithContent(md, resId, *pass);
        }
    }

private:
    std::map <std::string, MaterialList> m_materialCache;

    const MaterialList& GetMaterialDescriptorsFromCache(const std::string& resId, const std::string& groupName)
    {
        auto pos = m_materialCache.find(resId);
        if (pos != m_materialCache.end())
        {
            return pos->second;
        }

        std::string resourcePattern = common::GetMaterialFileName(resId, "*");

        Ogre::DataStreamList streams = Ogre::ResourceGroupManager::getSingleton().openResources(resourcePattern);

        const auto [it, status] = m_materialCache.insert({resId, ParseMaterialDescriptors(resId, groupName, Ogre::ResourceGroupManager::getSingleton())});

        return it->second;
    }

    static const Material& GetD2Material(const MaterialList& materials, const std::string_view& resourceName)
    {
        for (const auto& material : materials)
        {
            if (material.name == resourceName)
            {
                return material;
            }
        }

        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, std::format("Cannot find material with name: {}", resourceName));
    }
};


OgreMaterialProvider::OgreMaterialProvider()
    : m_impl(new OgreMaterialProviderImpl{})
{
}

OgreMaterialProvider::~OgreMaterialProvider()
{
}

Ogre::MaterialPtr OgreMaterialProvider::CreateOrRetrieveMaterial(const std::string& materialName, const std::string& groupName)
{
    Ogre::MaterialManager& mgr = Ogre::MaterialManager::getSingleton();
    auto res = mgr.createOrRetrieve(materialName, groupName, true);

    auto material = std::static_pointer_cast<Ogre::Material>(res.first);
    if (res.second)
    {
        m_impl->PopulateMaterial(materialName, groupName, *material); // TODO: possible Race Condition in multithreaded env
    }

    return material;
}


} // namespace res
} //namespace archive
} // namespace resource
} // namespace d2_hack  
