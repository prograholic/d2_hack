#ifndef D2_HACK_CODEC_ARCHIVE_RES_FILE_INFO_H
#define D2_HACK_CODEC_ARCHIVE_RES_FILE_INFO_H

#include <map>
#include <istream>

#include <OgreString.h>

namespace d2_hack
{
namespace codec
{
namespace archive
{

struct ResEntry
{
    size_t offset;
    size_t size;
};

struct ResFileInfo
{
    std::map<Ogre::String, ResEntry> info;
};

void ReadFileInfo(std::istream& stream, ResFileInfo& fileInfo);


} //namespace archive
} // namespace codec
} // namespace d2_hack  




#endif // D2_HACK_CODEC_ARCHIVE_RES_FILE_INFO_H

