#include <d2_hack/resource/color/color.h>


namespace d2_hack
{
namespace resource
{
namespace color
{

ColorResource::ColorResource(Ogre::ResourceManager* creator,
                             const Ogre::String& name,
                             Ogre::ResourceHandle handle,
                             const Ogre::String& group,
                             bool isManual,
                             Ogre::ManualResourceLoader* loader)
    : Ogre::Resource(creator, name, handle, group, isManual, loader)
{
}

Ogre::ColourValue ColorResource::GetColor() const
{
    return m_color;
}


} // namespace color
} // namespace resource
} // namespace d2_hack
