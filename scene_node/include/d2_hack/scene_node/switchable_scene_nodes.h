#ifndef D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_SWITCHABLE_SCENE_NODES_H
#define D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_SWITCHABLE_SCENE_NODES_H

#include <d2_hack/scene_node/scene_node_base.h>

#include <OgreEntity.h>

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
};

class SubEntitiesSceneNode : public SceneNode< resource::data::b3d::block_data::SubMeshBlockXxx, SceneNodeBase>
{
public:
    SubEntitiesSceneNode(const std::string_view& name, std::vector<size_t> subEntityIds);

    virtual void Activate(bool active) override;

    virtual Ogre::Vector3f GetAbsolutePosition() const override;

    virtual Ogre::Quaternion GetAbsoluteOrientation() const override;

private:
    std::vector<size_t> m_subEntityIds;
};


class GroupUnknown2 : public SceneNode<resource::data::b3d::block_data::GroupUnknownBlock2, SceneNodeBase>
{
public:
    GroupUnknown2(
        const std::string_view& name,
        const resource::data::b3d::block_data::GroupUnknown2& data);

    virtual Ogre::Vector3f GetAbsolutePosition() const override;

    virtual Ogre::Quaternion GetAbsoluteOrientation() const override;
};

class GroupTrigger9 : public SceneNode<resource::data::b3d::block_data::GroupTriggerBlock9, SceneNodeBase>
{
public:
    GroupTrigger9(
        const std::string_view& name,
        const resource::data::b3d::block_data::GroupTrigger9& data);

    virtual Ogre::Vector3f GetAbsolutePosition() const override;

    virtual Ogre::Quaternion GetAbsoluteOrientation() const override;
};

class SceneNodeEvent21 : public SceneNode<resource::data::b3d::block_data::GroupObjectsBlock21, SceneNodeBase>
{
public:
    SceneNodeEvent21(
        const std::string_view& name,
        const resource::data::b3d::block_data::GroupObjects21& data);

    virtual Ogre::Vector3f GetAbsolutePosition() const override;

    virtual Ogre::Quaternion GetAbsoluteOrientation() const override;

    virtual void Activate(bool active) override;

    virtual void ApplyState(std::string_view stateName, size_t stateId) override;

private:
    EventEntrySceneNode* m_activeEntry;
    const std::uint32_t m_defaultEntryId;

    virtual void DoInit() override;
};

class GroupUnknown29 : public SceneNode<resource::data::b3d::block_data::GroupUnknownBlock29, SceneNodeBase>
{
public:
    GroupUnknown29(
        const std::string_view& name,
        const resource::data::b3d::block_data::GroupUnknown29& data);

    virtual Ogre::Vector3f GetAbsolutePosition() const override;

    virtual Ogre::Quaternion GetAbsoluteOrientation() const override;
};


} // namespace scene_node
} // namespace d2_hack

#endif /* D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_SWITCHABLE_SCENE_NODES_H */
