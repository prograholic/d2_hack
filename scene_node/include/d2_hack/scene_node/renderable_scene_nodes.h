#ifndef D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_RENDERABLE_SCENE_NODE_H
#define D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_RENDERABLE_SCENE_NODE_H

#include <d2_hack/scene_node/scene_node_base.h>

#include <d2_hack/resource/data/b3d_types.h>

namespace d2_hack
{
namespace scene_node
{

class GroupVertexData7 : public OgreSceneNode< resource::data::b3d::block_data::GroupVertexDataBlock7>
{
public:

    GroupVertexData7(const std::string& name, Ogre::SceneNode* ogreSceneNode);

    virtual void PlayerMoved(const WorldContext& worldContext, const Ogre::Vector3f& movement) override;
};

class GroupVertexData37 : public OgreSceneNode< resource::data::b3d::block_data::GroupVertexDataBlock37>
{
public:

    GroupVertexData37(const std::string& name, Ogre::SceneNode* ogreSceneNode);

    virtual void PlayerMoved(const WorldContext& worldContext, const Ogre::Vector3f& movement) override;
};


} // namespace scene_node
} // namespace d2_hack

#endif /* D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_RENDERABLE_SCENE_NODE_H */
