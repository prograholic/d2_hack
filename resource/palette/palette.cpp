#include <d2_hack/resource/palette/palette.h>

D2_HACK_DISABLE_WARNING_BEGIN(4100)

#include <OgreResourceGroupManager.h>

D2_HACK_DISABLE_WARNING_END() //4100

#include <d2_hack/common/resource_mgmt.h>

#include "plm_reader.h"

namespace d2_hack
{
namespace resource
{
namespace palette
{

Palette::Palette(Ogre::ResourceManager* creator,
                 const Ogre::String& name,
                 Ogre::ResourceHandle handle,
                 const Ogre::String& group,
                 bool isManual,
                 Ogre::ManualResourceLoader* loader)
    : Ogre::Resource(creator, name, handle, group, isManual, loader)
{
}

Ogre::ColourValue Palette::GetColor(size_t index) const
{
    return m_palette.at(index);
}

void Palette::loadImpl()
{
    Ogre::DataStreamPtr dataStream = Ogre::ResourceGroupManager::getSingleton().openResource(mName, common::DefaultResourceGroup);

    PlmReader reader{*dataStream};
    PlmFile plmFile;
    reader.Read(plmFile);

    m_palette = std::move(plmFile.palette);
}

void Palette::unloadImpl()
{
    throw std::runtime_error("not implemented");
}


} // namespace palette
} // namespace resource
} // namespace d2_hack
