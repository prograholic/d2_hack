#ifndef D2_HACK_RESOURCE_ARCHIVE_INCLUDE_D2_HACK_RESOURCE_ARCHIVE_OGRE_MATERIAL_PROVIDER_H
#define D2_HACK_RESOURCE_ARCHIVE_INCLUDE_D2_HACK_RESOURCE_ARCHIVE_OGRE_MATERIAL_PROVIDER_H

#include <d2_hack/resource/archive/res_archive.h>


namespace d2_hack
{
namespace resource
{
namespace archive
{
namespace res
{
    
class OgreMaterialProvider
{
public:

    Ogre::MaterialPtr CreateOrRetrieveMaterial(const std::string& materialName, const std::string& groupName);

};

} // namespace res
} // namespace archive
} // namespace resource
} // namespace d2_hack  


#endif // D2_HACK_RESOURCE_ARCHIVE_INCLUDE_D2_HACK_RESOURCE_ARCHIVE_OGRE_MATERIAL_PROVIDER_H
