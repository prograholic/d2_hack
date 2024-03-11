#ifndef D2_HACK_APP_TOOL_SIMPLE_TXR_RENDERER_SIMPLE_TXR_RENDERER_H
#define D2_HACK_APP_TOOL_SIMPLE_TXR_RENDERER_SIMPLE_TXR_RENDERER_H

#include <d2_hack/app/base_application.h>

D2_HACK_DISABLE_WARNING_BEGIN(4251)
D2_HACK_DISABLE_WARNING_BEGIN(4100)
D2_HACK_DISABLE_WARNING_BEGIN(4275)

#include <Terrain/OgreTerrainGroup.h>

D2_HACK_DISABLE_WARNING_END() // 4275
D2_HACK_DISABLE_WARNING_END() // 4251
D2_HACK_DISABLE_WARNING_END() // 4100

#include <d2_hack/common/memory_mgmt.h>

namespace d2_hack
{
namespace app
{

class TerrainViewer : public BaseApplication
{
public:
    TerrainViewer();

private:

    std::unique_ptr<Ogre::TerrainGlobalOptions, common::Deleter> m_terrainGlobalOptions;
    std::unique_ptr<Ogre::TerrainGroup, common::Deleter> m_terrainGroup;

    virtual void CreateScene() override;

    virtual void shutdown() override;

    void CreateTerrain();
    void ConfigureTerrainDefaults(Ogre::Light* light);
    void InitBlendMaps(Ogre::Terrain* terrain);
    void DefineTerrain(long x, long y);
};

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_TOOL_SIMPLE_TXR_RENDERER_SIMPLE_TXR_RENDERER_H */
