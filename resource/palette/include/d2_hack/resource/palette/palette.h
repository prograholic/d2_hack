#ifndef D2_HACK_RESOURCE_PALETTE_INCLUDE_D2_HACK_RESOURCE_PALETTE_PALETTE_H
#define D2_HACK_RESOURCE_PALETTE_INCLUDE_D2_HACK_RESOURCE_PALETTE_PALETTE_H

#include <d2_hack/common/platform.h>

D2_HACK_DISABLE_WARNING_BEGIN(4100)
#include <OgreColourValue.h>
#include <OgreResource.h>
D2_HACK_DISABLE_WARNING_END() //4100

namespace d2_hack
{
namespace resource
{
namespace palette
{

class Palette: public Ogre::Resource
{
public:
    Palette(Ogre::ResourceManager* creator,
            const Ogre::String& name,
            Ogre::ResourceHandle handle,
            const Ogre::String& group,
            bool isManual,
            Ogre::ManualResourceLoader* loader);

    Ogre::ColourValue GetColor(size_t index) const;
private:
    std::vector<Ogre::ColourValue> m_palette;
    
    virtual void loadImpl() override;

    virtual void unloadImpl() override;
};

typedef std::shared_ptr<Palette> PalettePtr;

} // namespace palette
} // namespace resource
} // namespace d2_hack

#endif /* D2_HACK_RESOURCE_PALETTE_INCLUDE_D2_HACK_RESOURCE_PALETTE_PALETTE_H */

