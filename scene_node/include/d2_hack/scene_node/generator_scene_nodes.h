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
};


class TreeGeneratorSceneNode : public OgreSceneNode<resource::data::b3d::block_data::SimpleGeneratedObjectsBlock40>
{
public:

    struct TreeParams
    {
        std::string materialName;
        std::string meshName;
        float textureScale;
        float scale;
        Ogre::Vector3f location;
    };

    TreeGeneratorSceneNode(const std::string& name,
                           const resource::data::b3d::block_data::SimpleGeneratedObjects40& data,
                           const std::string& b3dId,
                           Ogre::SceneNode* ogreSceneNode,
                           Ogre::SceneManager* sceneManager);

private:
    Ogre::MeshPtr CreateMesh(const TreeParams& treeParams, Ogre::SceneManager* sceneManager);

    TreeParams DeduceTreeParams(const resource::data::b3d::block_data::SimpleGeneratedObjects40& data, const std::string& b3dId);
};

} // namespace scene_node
} // namespace d2_hack

#endif /* D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_GENERATOR_SCENE_NODES_H */
