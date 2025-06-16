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

class OgreMaterialProviderImpl;

class OgreMaterialProvider
{
public:

    OgreMaterialProvider();

    ~OgreMaterialProvider() noexcept;

    Ogre::MaterialPtr CreateOrRetrieveMaterial(const std::string_view& materialName, const std::string& groupName);

private:
    std::unique_ptr<OgreMaterialProviderImpl> m_impl;
};

} // namespace res
} // namespace archive
} // namespace resource
} // namespace d2_hack  


#endif // D2_HACK_RESOURCE_ARCHIVE_INCLUDE_D2_HACK_RESOURCE_ARCHIVE_OGRE_MATERIAL_PROVIDER_H
