#include <d2_hack/common/resource_mgmt.h>

#include <OgreException.h>


namespace d2_hack
{
namespace common
{
namespace extensions
{

const char ColorExt[] = ".d2_color";
const char MaterialExt[] = ".d2_material";
const char TextureFileExt[] = ".d2_texture";
const char PaletteFileExt[] = ".d2_palette";
const char BackFileExt[] = ".d2_back";
const char MaskFileExt[] = ".d2_mask";
const char SoundFileExt[] = ".d2_sound";

const char ColorExtNoDot[] = "d2_color";
const char MaterialExtNoDot[] = "d2_material";
const char TextureFileExtNoDot[] = "d2_texture";
const char PaletteFileExtNoDot[] = "d2_palette";
const char BackFileExtNoDot[] = "d2_back";
const char MaskFileExtNoDot[] = "d2_mask";
const char SoundFileExtNoDot[] = "d2_sound";

} // namespace extensions


std::string GetResourceName(const std::string& fileBaseName, const std::string& resourceName)
{
    return fileBaseName + "_" + resourceName;
}

std::string GetColorName(const std::string& resId, std::uint32_t colorIndex)
{
    return GetResourceName(resId, "COLOR-" + std::to_string(colorIndex)) + extensions::ColorExt;
}

std::string GetTextureFileName(const std::string& resId, std::uint32_t textureIndex)
{
    return GetResourceName(resId, "TEXTURE-" + std::to_string(textureIndex)) + extensions::TextureFileExt;
}

std::string GetPaletteFileName(const std::string& resId, std::uint32_t paletteFileIndex)
{
    return GetResourceName(resId, "PALETTE-" + std::to_string(paletteFileIndex)) + extensions::PaletteFileExt;
}

std::string GetBackFileName(const std::string& resId, std::uint32_t backFileIndex)
{
    return GetResourceName(resId, "BACKFILE-" + std::to_string(backFileIndex)) + extensions::BackFileExt;
}

std::string GetMaskFileName(const std::string& resId, std::uint32_t maskFileIndex)
{
    return GetResourceName(resId, "MASKFILE-" + std::to_string(maskFileIndex)) + extensions::MaskFileExt;
}

std::string GetSoundFileName(const std::string& resId, std::uint32_t soundFileIndex)
{
    return GetResourceName(resId, "SOUNDFILE-" + std::to_string(soundFileIndex)) + extensions::SoundFileExt;
}

std::string GetMaterialFileName(const std::string& resId, const std::string& materialId)
{
    return GetResourceName(resId, "MATERIAL-" + materialId) + extensions::MaterialExt;
}


void SplitResourceFileName(const std::string& resourceFileName, std::string* resId, std::string* resourceClass, std::string* resourceId, std::string* extension)
{
    std::string::size_type start = 0;
    auto pos = resourceFileName.find('_');
    if (pos == resourceFileName.npos)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "resourceFileName: `" + resourceFileName + "` does not have _");
    }
    if (resId)
    {
        *resId = resourceFileName.substr(start, pos - start);
    }

    start = pos + 1;
    pos = resourceFileName.find('-');
    if (pos == resourceFileName.npos)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "resourceFileName: `" + resourceFileName + "` does not have -");
    }
    if (resourceClass)
    {
        *resourceClass = resourceFileName.substr(start, pos - start);
    }

    start = pos + 1;
    pos = resourceFileName.find('.');
    if (pos == resourceFileName.npos)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "resourceFileName: `" + resourceFileName + "` does not have .");
    }
    if (resourceId)
    {
        *resourceId = resourceFileName.substr(start, pos - start);
    }

    if (extension)
    {
        *extension = resourceFileName.substr(pos);
    }
}

} // namespace common
} // namespace d2_hack
