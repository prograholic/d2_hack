#ifndef D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_TERRAIN_H
#define D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_TERRAIN_H

#include <d2_hack/common/platform.h>

#include <memory>

#include <Terrain/OgreTerrainGroup.h>

#include "b3d_scene_builder.h"



namespace d2_hack
{
namespace app
{

class Terrain : public B3dOgreSceneNode<resource::data::b3d::block_data::SimpleGeneratedObjectsBlock40>
{
public:
    Terrain(const std::string& name, Ogre::SceneNode* sceneNode, Ogre::SceneManager* sceneManager);

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
