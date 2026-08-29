#ifndef D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_GENERATOR_SCENE_NODES_H
#define D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_GENERATOR_SCENE_NODES_H

#include <d2_hack/common/platform.h>

#include <Terrain/OgreTerrainGroup.h>

#include <d2_hack/scene_node/scene_node_base.h>

#include <d2_hack/resource/data/b3d_types.h>

namespace d2_hack
{
namespace scene_node
{

class TerrainSceneNode : public SceneNode<resource::data::b3d::block_data::SimpleGeneratedObjectsBlock40, SceneNodeBase>
{
public:
    TerrainSceneNode(const std::string_view& name, Ogre::SceneManager* sceneManager);

    virtual Ogre::Vector3f GetAbsolutePosition() const override;

    virtual Ogre::Quaternion GetAbsoluteOrientation() const override;

private:
    std::unique_ptr<Ogre::TerrainGlobalOptions> m_terrainGlobalOptions;
    std::unique_ptr<Ogre::TerrainGroup> m_terrainGroup;

    void CreateTerrain();
    void ConfigureTerrainDefaults();
    void DefineTerrains();
};

class TreeGeneratorSceneNode : public SceneNode<resource::data::b3d::block_data::SimpleGeneratedObjectsBlock40, SceneNodeBase>
{
public:
    explicit TreeGeneratorSceneNode(const std::string_view& name);

    virtual Ogre::Vector3f GetAbsolutePosition() const override;

    virtual Ogre::Quaternion GetAbsoluteOrientation() const override;
};

} // namespace scene_node
} // namespace d2_hack

#endif /* D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_GENERATOR_SCENE_NODES_H */
