#ifndef D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_SCENE_NODE_H
#define D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_SCENE_NODE_H

#include <d2_hack/common/platform.h>

#include <Terrain/OgreTerrainGroup.h>

#include <d2_hack/scene_node/scene_node_base.h>
#include <d2_hack/resource/data/b3d_types.h>



namespace d2_hack
{
namespace scene_node
{

class TerrainSceneNode : public OgreSceneNode<resource::data::b3d::block_data::SimpleGeneratedObjectsBlock40>
{
public:
    TerrainSceneNode(const std::string& name, Ogre::SceneNode* ogreSceneNode, Ogre::SceneManager* sceneManager);

private:
    std::unique_ptr<Ogre::TerrainGlobalOptions> m_terrainGlobalOptions;
    std::unique_ptr<Ogre::TerrainGroup> m_terrainGroup;

    void CreateTerrain();
    void ConfigureTerrainDefaults();
    void DefineTerrains();

    virtual void DoActivate(const WorldContext& worldContext) override;

    virtual void DoDeactivate(const WorldContext& worldContext) override;
};

} // namespace scene_node
} // namespace d2_hack

#endif /* D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_SCENE_NODE_H */
