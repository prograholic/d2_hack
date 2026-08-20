#ifndef D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_SWITCHABLE_SCENE_NODES_H
#define D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_SWITCHABLE_SCENE_NODES_H

#include <d2_hack/scene_node/scene_node_base.h>

#include <d2_hack/resource/data/b3d_types.h>

namespace d2_hack
{
namespace scene_node
{


class EventEntrySceneNode : public SceneNode<resource::data::b3d::block_data::EventEntryBlockXxx, SceneNodeBase>
{
public:
    explicit EventEntrySceneNode(const std::string_view& name);

    virtual Ogre::Vector3f GetAbsolutePosition() const override;

    virtual Ogre::Quaternion GetAbsoluteOrientation() const override;

    virtual void SetVisible(bool visible) override;
};


class SwitchableSceneNode : public SceneNodeBase
{
public:

    SwitchableSceneNode(const std::string_view& name, std::uint32_t type);

    virtual Ogre::Vector3f GetAbsolutePosition() const override;

    virtual Ogre::Quaternion GetAbsoluteOrientation() const override;

    virtual void SetVisible(bool visible) override;

private:
    virtual SceneNodeBase* ActivateItem(const WorldContext& worldContext) = 0;
};

class GroupUnknown2 : public SceneNode<resource::data::b3d::block_data::GroupUnknownBlock2, SwitchableSceneNode>
{
public:
    GroupUnknown2(
        const std::string_view& name,
        const resource::data::b3d::block_data::GroupUnknown2& data);

private:
    virtual SceneNodeBase* ActivateItem(const WorldContext& worldContext) override;
};

class GroupTrigger9 : public SceneNode<resource::data::b3d::block_data::GroupTriggerBlock9, SwitchableSceneNode>
{
public:
    GroupTrigger9(
        const std::string_view& name,
        const resource::data::b3d::block_data::GroupTrigger9& data);

private:
    virtual SceneNodeBase* ActivateItem(const WorldContext& worldContext) override;
};

class SceneNodeEvent21 : public SceneNode<resource::data::b3d::block_data::GroupObjectsBlock21, SwitchableSceneNode>
{
public:
    SceneNodeEvent21(
        const std::string_view& name,
        const resource::data::b3d::block_data::GroupObjects21& data);

    size_t StateCount() const;

    void SetActiveState(std::string_view stateName, size_t stateId);

private:
    virtual SceneNodeBase* ActivateItem(const WorldContext& worldContext) override;
};

class GroupUnknown29 : public SceneNode<resource::data::b3d::block_data::GroupUnknownBlock29, SwitchableSceneNode>
{
public:
    GroupUnknown29(
        const std::string_view& name,
        const resource::data::b3d::block_data::GroupUnknown29& data);

private:
    virtual SceneNodeBase* ActivateItem(const WorldContext& worldContext) override;
};


} // namespace scene_node
} // namespace d2_hack

#endif /* D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_SWITCHABLE_SCENE_NODES_H */
