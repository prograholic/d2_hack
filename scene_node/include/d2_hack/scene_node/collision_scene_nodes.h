#ifndef D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_COLLISION_SCENE_NODE_H
#define D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_COLLISION_SCENE_NODE_H

#include <d2_hack/scene_node/scene_node_base.h>

#include <d2_hack/resource/data/b3d_types.h>

namespace d2_hack
{
namespace scene_node
{
class SimpleFlatCollision20 : public OgreSceneNode<resource::data::b3d::block_data::SimpleFlatCollisionBlock20>
{
public:
    SimpleFlatCollision20(
        const std::string& name,
        Ogre::SceneNode* ogreSceneNode,
        const resource::data::b3d::block_data::SimpleFlatCollision20& data,
        Ogre::SceneManager* sceneManager);
};

class SimpleVolumeCollision23 : public OgreSceneNode<resource::data::b3d::block_data::SimpleVolumeCollisionBlock23>
{
public:
    SimpleVolumeCollision23(
        const std::string& name,
        Ogre::SceneNode* ogreSceneNode,
        const resource::data::b3d::block_data::SimpleVolumeCollision23& data,
        Ogre::SceneManager* sceneManager);
};


} // namespace scene_node
} // namespace d2_hack

#endif /* D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_COLLISION_SCENE_NODE_H */
