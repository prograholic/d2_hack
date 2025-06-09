#ifndef D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_COLLISION_SCENE_NODES_H
#define D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_COLLISION_SCENE_NODES_H

#include <d2_hack/scene_node/scene_node_base.h>

#include <d2_hack/resource/data/b3d_types.h>

namespace d2_hack
{
namespace scene_node
{

class SimpleFlatCollision20 : public SceneNode<resource::data::b3d::block_data::SimpleFlatCollisionBlock20, SceneNodeBase>
{
public:
    SimpleFlatCollision20(
        const std::string& name,
        const resource::data::b3d::block_data::SimpleFlatCollision20& data);

    virtual void OnCameraMoved(const WorldContext& worldContext, const Ogre::Vector3f& movement) override;

    virtual void SetVisible(const WorldContext& worldContext, bool visible) override;

    virtual Ogre::Vector3f GetAbsolutePosition() const override;

    virtual Ogre::Quaternion GetAbsoluteOrientation() const override;
};

class SimpleVolumeCollision23 : public SceneNode<resource::data::b3d::block_data::SimpleVolumeCollisionBlock23, SceneNodeBase>
{
public:
    SimpleVolumeCollision23(
        const std::string& name,
        const resource::data::b3d::block_data::SimpleVolumeCollision23& data);

    virtual void OnCameraMoved(const WorldContext& worldContext, const Ogre::Vector3f& movement) override;

    virtual void SetVisible(const WorldContext& worldContext, bool visible) override;

    virtual Ogre::Vector3f GetAbsolutePosition() const override;

    virtual Ogre::Quaternion GetAbsoluteOrientation() const override;
};


} // namespace scene_node
} // namespace d2_hack

#endif /* D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_COLLISION_SCENE_NODES_H */
