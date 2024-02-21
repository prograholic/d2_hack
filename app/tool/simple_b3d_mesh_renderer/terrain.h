#ifndef D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_TERRAIN_H
#define D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_TERRAIN_H

#include <d2_hack/common/platform.h>

#include <memory>

#include <Terrain/OgreTerrainGroup.h>

#include <d2_hack/resource/data/b3d_node.h>

namespace d2_hack
{
namespace app
{

class Terrain
{
public:
    explicit Terrain(Ogre::SceneManager* sceneManager);

    Ogre::Vector3 GetPosition() const;
    void SetPosition(const Ogre::Vector3& pos);

private:
    std::unique_ptr<Ogre::TerrainGlobalOptions> m_terrainGlobalOptions;
    std::unique_ptr<Ogre::TerrainGroup> m_terrainGroup;

    void CreateTerrain();
    void ConfigureTerrainDefaults();
    void DefineTerrains();

};

typedef std::unique_ptr<Terrain> TerrainPtr;

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_TERRAIN_H */
