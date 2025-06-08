#ifndef D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_RESOURCE_MGMT_H
#define D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_RESOURCE_MGMT_H

#include <string>

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

std::string GetResourceName(const std::string& fileBaseName, const std::string& resourceName);

std::string GetColorName(const std::string& resId, std::uint32_t colorIndex);

std::string GetTextureFileName(const std::string& resId, std::uint32_t textureFileIndex);

std::string GetPaletteFileName(const std::string& resId, std::uint32_t paletteFileIndex);

std::string GetBackFileName(const std::string& resId, std::uint32_t backFileIndex);

std::string GetMaskFileName(const std::string& resId, std::uint32_t maskFileIndex);

std::string GetSoundFileName(const std::string& resId, std::uint32_t soundFileIndex);

std::string GetMaterialFileName(const std::string& resId, const std::string& materialId);

void SplitResourceFileName(const std::string& resourceFileName, std::string* resId = nullptr, std::string* resourceClass = nullptr, std::string* resourceId = nullptr, std::string* extension = nullptr);

} // namespace common
} // namespace d2_hack

#endif /* D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_RESOURCE_MGMT_H */
