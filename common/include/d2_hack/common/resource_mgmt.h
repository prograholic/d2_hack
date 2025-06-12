#ifndef D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_RESOURCE_MGMT_H
#define D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_RESOURCE_MGMT_H

#include <string>
#include <string_view>
#include <format>

namespace d2_hack
{
namespace common
{
namespace extensions
{
extern const char ColorExt[];
extern const char MaterialExt[];
extern const char TextureFileExt[];
extern const char PaletteFileExt[];
extern const char BackFileExt[];
extern const char MaskFileExt[];
extern const char SoundFileExt[];

extern const char ColorExtNoDot[];
extern const char MaterialExtNoDot[];
extern const char TextureFileExtNoDot[];
extern const char PaletteFileExtNoDot[];
extern const char BackFileExtNoDot[];
extern const char MaskFileExtNoDot[];
extern const char SoundFileExtNoDot[];
} //namespace extensions

template <typename ResourceNameType>
std::string GetResourceName(const std::string_view& fileBaseName, const ResourceNameType& resourceName)
{
    return std::format("{}_{}", fileBaseName, resourceName);
}

template <typename ResourceType, typename ResourceIdName, typename Extension>
std::string GetResourceName(const std::string_view& fileBaseName, const ResourceType& resourceType, const ResourceIdName& resourceId, const Extension& extension)
{
    return std::format("{}_{}-{}.{}", fileBaseName, resourceType, resourceId, extension);
}

std::string GetColorName(const std::string_view& resId, std::uint32_t colorIndex);

std::string GetTextureFileName(const std::string_view& resId, std::uint32_t textureFileIndex);

std::string GetPaletteFileName(const std::string_view& resId, const std::string_view& paletteId);

std::string GetBackFileName(const std::string_view& resId, std::uint32_t backFileIndex);

std::string GetMaskFileName(const std::string_view& resId, std::uint32_t maskFileIndex);

std::string GetSoundFileName(const std::string_view& resId, std::uint32_t soundFileIndex);

std::string GetMaterialFileName(const std::string_view& resId, const std::string_view& materialId);

void SplitResourceFileName(const std::string_view& resourceFileName, std::string* resId = nullptr, std::string* resourceClass = nullptr, std::string* resourceId = nullptr, std::string* extension = nullptr);

} // namespace common
} // namespace d2_hack

#endif /* D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_RESOURCE_MGMT_H */
