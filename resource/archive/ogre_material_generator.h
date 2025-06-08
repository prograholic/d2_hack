#ifndef D2_HACK_RESOURCE_ARCHIVE_OGRE_MATERIAL_GENERATOR_H
#define D2_HACK_RESOURCE_ARCHIVE_OGRE_MATERIAL_GENERATOR_H

#include <d2_hack/resource/archive/res_archive.h>


namespace d2_hack
{
namespace resource
{
namespace archive
{
namespace res
{

Ogre::DataStreamPtr GenerateMaterial(const ResFileInfo& fileInfo, const std::string& filename, const Ogre::DataStreamPtr& stream);

} // namespace res
} // namespace archive
} // namespace resource
} // namespace d2_hack  

#endif // D2_HACK_RESOURCE_ARCHIVE_OGRE_MATERIAL_GENERATOR_H
