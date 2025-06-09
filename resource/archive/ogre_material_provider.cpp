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

static const char ColorMaterialTemplate [] = 
    "material %1%\n"
    "{\n"
    "    technique\n"
    "    {\n"
    "        pass\n"
    "        {\n"
    "            lighting on\n"
    "            ambient 0.1 0.3 0.1 1\n"
    "            diffuse 0.2 0.2 0.2 1\n"
    "            emissive %2% %3% %4% %5%\n"
    "            depth_check %6%\n"
    "        }\n"
    "    }\n"
    "}\n";


static const char TextureMaterialTemplate[] =
    "material %1%\n"
    "{\n"
    "    technique\n"
    "    {\n"
    "        pass\n"
    "        {\n"
    "            texture_unit\n"
    "            {\n"
    "                texture %2%\n"
    "            }\n"
    "            depth_check %3%\n"
    "        }\n"
    "    }\n"
    "}\n";


static Ogre::DataStreamPtr PrepareColorMaterial(const MaterialDescriptor& md, const std::string& filename)
{
    float alpha = 1.0f;
    if (md.transparency)
    {
        alpha = *md.transparency;
    }

    std::string depthCheck = "on";
    if (md.noz)
    {
        depthCheck = "off";
    }

    palette::PalettePtr plm = manager::Manager::getSingleton().Load("COMMON\\common.plm", common::DefaultResourceGroup);

    Ogre::ColourValue cv = plm->GetColor(md.index - 1);
    std::string materialContent = str(boost::format(ColorMaterialTemplate) % filename.substr(0, filename.size() - 9) % cv.r % cv.g % cv.b % alpha % depthCheck);

    const std::size_t dataSize = materialContent.size();

    std::unique_ptr<char, common::ArrayDeleter<Ogre::MEMCATEGORY_GENERAL>> buffer{ OGRE_ALLOC_T(char, dataSize, Ogre::MEMCATEGORY_GENERAL) };
    std::memcpy(buffer.get(), materialContent.c_str(), dataSize);

    Ogre::DataStreamPtr res = std::make_shared<Ogre::MemoryDataStream>(buffer.get(), dataSize, true, true);
    buffer.release();

    return res;
}

static Ogre::DataStreamPtr PrepareTextureMaterial(const MaterialDescriptor& md, const std::string& filename)
{
    const std::string textureFilename;// = LookupTextureByIndex(fileInfo, md.index);

    std::string depthCheck = "on";
    if (md.noz)
    {
        depthCheck = "off";
    }

    std::string materialContent = str(boost::format(TextureMaterialTemplate) % filename.substr(0, filename.size() - 9) % textureFilename % depthCheck);


    const std::size_t dataSize = materialContent.size();

    std::unique_ptr<char, common::ArrayDeleter<Ogre::MEMCATEGORY_GENERAL>> buffer{ OGRE_ALLOC_T(char, dataSize, Ogre::MEMCATEGORY_GENERAL) };
    std::memcpy(buffer.get(), materialContent.c_str(), dataSize);

    Ogre::DataStreamPtr res = std::make_shared<Ogre::MemoryDataStream>(buffer.get(), dataSize, true, true);
    buffer.release();

    return res;
}



Ogre::DataStreamPtr GenerateMaterial(const std::string& filename, const Ogre::DataStreamPtr& stream)
{
    const std::string content = stream->getAsString();

    const MaterialDescriptor md = ParseMaterialDescriptor(content);

    switch (md.type)
    {
    case MaterialType::col:
        return PrepareColorMaterial(md, filename);

    case MaterialType::itx:
    case MaterialType::tex:
    case MaterialType::ttx:
        return PrepareTextureMaterial(md, filename);
    default:
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, std::format("Unknown material type in {}: {}", filename, static_cast<int>(md.type)));
    }
}


#if 0

"material %1%\n"
"{\n"
"    technique\n"
"    {\n"
"        pass\n"
"        {\n"
"            lighting on\n"
"            ambient 0.1 0.3 0.1 1\n"
"            diffuse 0.2 0.2 0.2 1\n"
"            emissive %2% %3% %4% %5%\n"
"            depth_check %6%\n"
"        }\n"
"    }\n"
"}\n";

#endif // 0

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

#if 0

"material %1%\n"
"{\n"
"    technique\n"
"    {\n"
"        pass\n"
"        {\n"
"            texture_unit\n"
"            {\n"
"                texture %2%\n"
"            }\n"
"            depth_check %3%\n"
"        }\n"
"    }\n"
"}\n";


#endif // 0

static void FillMaterialWithTexture(const MaterialDescriptor& md, const std::string& resId, Ogre::Material& material)
{
    auto technique = material.createTechnique();
    auto pass = technique->createPass();

    auto textureUnitState = pass->createTextureUnitState();

    std::string textureName = common::GetTextureFileName(resId, md.index);
    auto textureRes = Ogre::TextureManager::getSingleton().createOrRetrieve(textureName, common::DefaultResourceGroup);

    textureUnitState->setTexture(std::static_pointer_cast<Ogre::Texture>(textureRes.first));
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
