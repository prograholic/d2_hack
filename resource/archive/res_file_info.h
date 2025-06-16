#ifndef D2_HACK_RESOURCE_ARCHIVE_RES_FILE_INFO_H
#define D2_HACK_RESOURCE_ARCHIVE_RES_FILE_INFO_H

#include <vector>

#include <OgreDataStream.h>

namespace d2_hack
{
namespace resource
{
namespace archive
{
namespace res
{

struct ResEntry
{
    size_t offset;
    size_t size;
    std::string name;
    std::string resString;
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

void ReadFileInfo(const std::string_view& resId, Ogre::DataStream& stream, ResFileInfo& fileInfo);

} // namespace res
} // namespace archive
} // namespace resource
} // namespace d2_hack  




#endif // D2_HACK_RESOURCE_ARCHIVE_RES_FILE_INFO_H

