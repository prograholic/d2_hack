#ifndef D2_HACK_RESOURCE_ARCHIVE_RES_FILE_INFO_H
#define D2_HACK_RESOURCE_ARCHIVE_RES_FILE_INFO_H

#include <map>

#include <OgreDataStream.h>
#include <OgreString.h>

namespace d2_hack
{
namespace resource
{
namespace archive
{
namespace res
{

enum class EntryType
{
    File,
    Color,
    Material
};

struct ResEntry
{
    EntryType type;

    size_t index;
    size_t offset;
    size_t size;
    std::string name;
};

namespace entries
{
extern const char Colors[];
extern const char Materials[];
extern const char Sounds[];
extern const char TextureFiles[];
extern const char PaletteFiles[];
extern const char BackFiles[];
extern const char MaskFiles[];
extern const char SoundFiles[];
} // namespace entries

struct ResFileInfo
{
    std::vector<ResEntry> info;
};

void ReadFileInfo(const std::string& resId, Ogre::DataStream& stream, ResFileInfo& fileInfo);


std::string GetColorFileName(const std::string& resId, const std::string& colorId);

} // namespace res
} // namespace archive
} // namespace resource
} // namespace d2_hack  




#endif // D2_HACK_RESOURCE_ARCHIVE_RES_FILE_INFO_H

