#ifndef D2_HACK_RESOURCE_ARCHIVE_MATERIAL_PARSER_H
#define D2_HACK_RESOURCE_ARCHIVE_MATERIAL_PARSER_H

#include <OgreDataStream.h>

#include "res_file_info.h"

namespace d2_hack
{
namespace resource
{
namespace archive
{
namespace res
{

Ogre::DataStreamPtr ParseMaterial(const ResFileInfo& fileInfo, const std::string& filename, const Ogre::DataStreamPtr& stream);

} // namespace res
} // namespace archive
} // namespace resource
} // namespace d2_hack  

#endif // D2_HACK_RESOURCE_ARCHIVE_MATERIAL_PARSER_H
