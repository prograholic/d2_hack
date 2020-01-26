#ifndef D2_HACK_RESOURCE_COLOR_INCLUDE_D2_HACK_RESOURCE_COLOR_COLOR_H
#define D2_HACK_RESOURCE_COLOR_INCLUDE_D2_HACK_RESOURCE_COLOR_COLOR_H

#include <d2_hack/common/platform.h>

D2_HACK_DISABLE_WARNING_BEGIN(4100)
#include <OgreColourValue.h>
#include <OgreResource.h>
D2_HACK_DISABLE_WARNING_END() //4100

namespace d2_hack
{
namespace resource
{
namespace color
{

class ColorResource: public Ogre::Resource
{
public:
    ColorResource(Ogre::ResourceManager* creator,
                  const Ogre::String& name,
                  Ogre::ResourceHandle handle,
                  const Ogre::String& group,
                  bool isManual,
                  Ogre::ManualResourceLoader* loader);

    Ogre::ColourValue GetColor() const;

private:
    Ogre::ColourValue m_color;
};

typedef std::shared_ptr<ColorResource> ColorResourcePtr;

} // namespace image
} // namespace resource
} // namespace d2_hack

#endif /* D2_HACK_RESOURCE_COLOR_INCLUDE_D2_HACK_RESOURCE_COLOR_COLOR_H */

