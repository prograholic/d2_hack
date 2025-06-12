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


std::string GetColorName(const std::string_view& resId, std::uint32_t colorIndex)
{
    return GetResourceName(resId, "COLOR", colorIndex, extensions::ColorExtNoDot);
}

std::string GetTextureFileName(const std::string_view& resId, std::uint32_t textureIndex)
{
    return GetResourceName(resId, "TEXTURE", textureIndex, extensions::TextureFileExtNoDot);
}

std::string GetPaletteFileName(const std::string_view& resId, const std::string_view& paletteId)
{
    return GetResourceName(resId, "PALETTE", paletteId, extensions::PaletteFileExtNoDot);
}

std::string GetBackFileName(const std::string_view& resId, std::uint32_t backFileIndex)
{
    return GetResourceName(resId, "BACKFILE", backFileIndex, extensions::BackFileExtNoDot);
}

std::string GetMaskFileName(const std::string_view& resId, std::uint32_t maskFileIndex)
{
    return GetResourceName(resId, "MASKFILE", maskFileIndex, extensions::MaskFileExtNoDot);
}

std::string GetSoundFileName(const std::string_view& resId, std::uint32_t soundFileIndex)
{
    return GetResourceName(resId, "SOUNDFILE", soundFileIndex, extensions::SoundFileExtNoDot);
}

std::string GetMaterialFileName(const std::string_view& resId, const std::string_view& materialId)
{
    return GetResourceName(resId, "MATERIAL", materialId, extensions::MaterialExtNoDot);
}


void SplitResourceFileName(const std::string_view& resourceFileName, std::string* resId, std::string* resourceClass, std::string* resourceId, std::string* extension)
{
    std::string::size_type start = 0;
    auto pos = resourceFileName.find('_');
    if (pos == resourceFileName.npos)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, std::format("resourceFileName: `{}` does not have _", resourceFileName));
    }
    if (resId)
    {
        *resId = resourceFileName.substr(start, pos - start);
    }

    start = pos + 1;
    pos = resourceFileName.find('-');
    if (pos == resourceFileName.npos)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, std::format("resourceFileName: `{}` does not have -", resourceFileName));
    }
    if (resourceClass)
    {
        *resourceClass = resourceFileName.substr(start, pos - start);
    }

    start = pos + 1;
    pos = resourceFileName.find('.');
    if (pos == resourceFileName.npos)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, std::format("resourceFileName: `{}` does not have .", resourceFileName));
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
