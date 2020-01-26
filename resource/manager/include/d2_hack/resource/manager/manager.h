#ifndef D2_HACK_RESOURCE_MANAGER_INCLUDE_D2_HACK_RESOURCE_MANAGER_MANAGER_H
#define D2_HACK_RESOURCE_MANAGER_INCLUDE_D2_HACK_RESOURCE_MANAGER_MANAGER_H

#include <d2_hack/common/platform.h>

D2_HACK_DISABLE_WARNING_BEGIN(4100)
#include <OgreResourceManager.h>
D2_HACK_DISABLE_WARNING_END() //4100
#include <OgreSingleton.h>

#include <d2_hack/resource/color/color.h>

namespace d2_hack
{
namespace resource
{
namespace manager
{

class Manager: public Ogre::ResourceManager, public Ogre::Singleton<Manager>
{
public:
    Manager();
    ~Manager();

    color::ColorResourcePtr Create(const Ogre::String& name,
                                   const Ogre::String& group,
                                   bool isManual = false,
                                   Ogre::ManualResourceLoader* loader = nullptr,
                                   const Ogre::NameValuePairList* createParams = nullptr);


    color::ColorResourcePtr GetColorByName(const Ogre::String& name,
                                           const Ogre::String& groupName = Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);

private:

    virtual Ogre::Resource* createImpl(const Ogre::String& name,
                                       Ogre::ResourceHandle handle,
                                       const Ogre::String& group,
                                       bool isManual,
                                       Ogre::ManualResourceLoader* loader,
                                       const Ogre::NameValuePairList* createParams) override;

};

} // namespace image
} // namespace resource
} // namespace d2_hack

#endif /* D2_HACK_RESOURCE_MANAGER_INCLUDE_D2_HACK_RESOURCE_MANAGER_MANAGER_H */

