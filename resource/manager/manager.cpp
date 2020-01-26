#include <d2_hack/resource/manager/manager.h>

namespace Ogre
{
template<> d2_hack::resource::manager::Manager* Singleton<d2_hack::resource::manager::Manager>::msSingleton = nullptr;
} // namespace Ogre

namespace d2_hack
{
namespace resource
{
namespace manager
{

static const std::string ResourceType = "d2colorinfo";

Manager::Manager()
{
    Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(ResourceType, this);
}
Manager::~Manager()
{
    Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(ResourceType);
}

color::ColorResourcePtr Manager::Create(const Ogre::String& name,
                                        const Ogre::String& group,
                                        bool isManual,
                                        Ogre::ManualResourceLoader* loader,
                                        const Ogre::NameValuePairList* createParams)
{
    return std::static_pointer_cast<color::ColorResource>(createResource(name, group, isManual, loader, createParams));
}


color::ColorResourcePtr Manager::GetColorByName(const Ogre::String& name, const Ogre::String& groupName)
{
    return std::static_pointer_cast<color::ColorResource>(getResourceByName(name, groupName));
}

Ogre::Resource* Manager::createImpl(const Ogre::String& /* name */,
                                    Ogre::ResourceHandle /* handle */,
                                    const Ogre::String& /* group */,
                                    bool /* isManual */,
                                    Ogre::ManualResourceLoader* /* loader */,
                                    const Ogre::NameValuePairList* /* createParams */)
{
    return nullptr;
}

} // namespace manager
} // namespace resource
} // namespace d2_hack
