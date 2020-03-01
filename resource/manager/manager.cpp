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

palette::PalettePtr Manager::Load(const Ogre::String& name,
                                  const Ogre::String& group,
                                  bool isManual,
                                  Ogre::ManualResourceLoader* loader,
                                  const Ogre::NameValuePairList* createParams)
{
    auto res = createOrRetrieve(name, group, isManual, loader, createParams);
    if (res.second)
    {
        res.first->load();
    }

    return std::static_pointer_cast<palette::Palette>(res.first);
}

Ogre::Resource* Manager::createImpl(const Ogre::String& name,
                                    Ogre::ResourceHandle handle,
                                    const Ogre::String& group,
                                    bool isManual,
                                    Ogre::ManualResourceLoader* loader,
                                    const Ogre::NameValuePairList* /* createParams */)
{
    return OGRE_NEW palette::Palette{this, name, handle, group, isManual, loader};
}

} // namespace manager
} // namespace resource
} // namespace d2_hack
