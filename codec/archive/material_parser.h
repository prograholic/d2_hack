#ifndef D2_HACK_CODEC_ARCHIVE_MATERIAL_PARSER_H
#define D2_HACK_CODEC_ARCHIVE_MATERIAL_PARSER_H

#include <OgreDataStream.h>

namespace d2_hack
{
namespace codec
{
namespace archive
{
namespace res
{

Ogre::DataStreamPtr ParseMaterial(const std::string& resId, const std::string& filename, const Ogre::DataStreamPtr& stream);

} // namespace res
} // namespace archive
} // namespace codec
} // namespace d2_hack  

#endif // D2_HACK_CODEC_ARCHIVE_MATERIAL_PARSER_H
