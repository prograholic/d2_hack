#ifndef D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_GENERATOR_SCENE_NODES_H
#define D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_GENERATOR_SCENE_NODES_H

#include <d2_hack/common/platform.h>

#include <Terrain/OgreTerrainGroup.h>

#include <d2_hack/scene_node/scene_node_base.h>
#include <d2_hack/resource/data/b3d_types.h>
#include <d2_hack/resource/archive/ogre_material_provider.h>



namespace d2_hack
{
namespace scene_node
{

class TerrainSceneNode : public OgreSceneNode<resource::data::b3d::block_data::SimpleGeneratedObjectsBlock40>
{
public:
    TerrainSceneNode(const std::string_view& name, Ogre::SceneNode* ogreSceneNode, Ogre::SceneManager* sceneManager);

private:
    std::unique_ptr<Ogre::TerrainGlobalOptions> m_terrainGlobalOptions;
    std::unique_ptr<Ogre::TerrainGroup> m_terrainGroup;

    void CreateTerrain();
    void ConfigureTerrainDefaults();
    void DefineTerrains();
};

class TerrainSceneNode2 : public SceneNode<resource::data::b3d::block_data::SimpleGeneratedObjectsBlock40, SceneNodeBase>
{
public:
    TerrainSceneNode2(const std::string_view& name, Ogre::SceneManager* sceneManager);

    virtual Ogre::Vector3f GetAbsolutePosition() const override;

    virtual Ogre::Quaternion GetAbsoluteOrientation() const override;

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
        std::string extraMaterialName;
        std::string meshName;
        float textureScale;
        float scale;
        Ogre::Vector3f location;
    };

    TreeGeneratorSceneNode(const std::string_view& name,
                           const resource::data::b3d::block_data::SimpleGeneratedObjects40& data,
                           const std::string_view& b3dId,
                           Ogre::SceneNode* ogreSceneNode,
                           Ogre::SceneManager* sceneManager,
                           resource::archive::res::OgreMaterialProvider* ogreMaterialProvider);

private:
    Ogre::MeshPtr CreateMesh(const TreeParams& treeParams, Ogre::SceneManager* sceneManager);

    TreeParams DeduceTreeParams(const resource::data::b3d::block_data::SimpleGeneratedObjects40& data, const std::string_view& b3dId);
};

class TreeGeneratorSceneNode2 : public SceneNode<resource::data::b3d::block_data::SimpleGeneratedObjectsBlock40, SceneNodeBase>
{
public:
    explicit TreeGeneratorSceneNode2(const std::string_view& name);

    virtual Ogre::Vector3f GetAbsolutePosition() const override;

    virtual Ogre::Quaternion GetAbsoluteOrientation() const override;
};

} // namespace scene_node
} // namespace d2_hack

#endif /* D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_GENERATOR_SCENE_NODES_H */
