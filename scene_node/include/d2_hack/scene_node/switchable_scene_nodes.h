#ifndef D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_SWITCHABLE_SCENE_NODES_H
#define D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_SWITCHABLE_SCENE_NODES_H

#include <d2_hack/scene_node/scene_node_base.h>

#include <d2_hack/resource/data/b3d_types.h>

namespace d2_hack
{
namespace scene_node
{


class EventEntrySceneNode : public OgreSceneNode<resource::data::b3d::block_data::EventEntryBlockXxx>
{
public:
    EventEntrySceneNode(const std::string& name, Ogre::SceneNode* ogreSceneNode);

    virtual void PlayerMoved(const WorldContext& worldContext, const Ogre::Vector3f& movement) override;
};


class SwitchableOgreSceneNode : public SceneNodeBase
{
public:

    SwitchableOgreSceneNode(const std::string& name, std::uint32_t type);

    virtual void PlayerMoved(const WorldContext& worldContext, const Ogre::Vector3f& movement) override;

private:
    virtual SceneNodeBase* ActivateItem(const WorldContext& worldContext) = 0;
};

class GroupUnknown2 : public OgreSceneNode<resource::data::b3d::block_data::GroupUnknownBlock2, SwitchableOgreSceneNode>
{
public:
    GroupUnknown2(
        const std::string& name,
        Ogre::SceneNode* ogreSceneNode,
        const resource::data::b3d::block_data::GroupUnknown2& data);

private:
    virtual SceneNodeBase* ActivateItem(const WorldContext& worldContext) override;
};

class GroupTrigger9 : public OgreSceneNode<resource::data::b3d::block_data::GroupTriggerBlock9, SwitchableOgreSceneNode>
{
public:
    GroupTrigger9(
        const std::string& name,
        Ogre::SceneNode* ogreSceneNode,
        const resource::data::b3d::block_data::GroupTrigger9& data);

private:
    virtual SceneNodeBase* ActivateItem(const WorldContext& worldContext) override;
};

class GroupLod10 : public OgreSceneNode<resource::data::b3d::block_data::GroupLodParametersBlock10, SwitchableOgreSceneNode>
{
public:
    GroupLod10(
        const std::string& name,
        Ogre::SceneNode* ogreSceneNode,
        const resource::data::b3d::block_data::GroupLodParameters10& data);

private:
    const resource::data::b3d::block_data::GroupLodParameters10 m_data;

    virtual SceneNodeBase* ActivateItem(const WorldContext& worldContext) override;
};

class SceneNodeEvent21 : public OgreSceneNode<resource::data::b3d::block_data::GroupObjectsBlock21, SwitchableOgreSceneNode>
{
public:
    SceneNodeEvent21(
        const std::string& name,
        Ogre::SceneNode* ogreSceneNode,
        const resource::data::b3d::block_data::GroupObjects21& data);

private:
    virtual SceneNodeBase* ActivateItem(const WorldContext& worldContext) override;
};

class GroupUnknown29 : public OgreSceneNode<resource::data::b3d::block_data::GroupUnknownBlock29, SwitchableOgreSceneNode>
{
public:
    GroupUnknown29(
        const std::string& name,
        Ogre::SceneNode* ogreSceneNode,
        const resource::data::b3d::block_data::GroupUnknown29& data);

private:
    virtual SceneNodeBase* ActivateItem(const WorldContext& worldContext) override;
};


} // namespace scene_node
} // namespace d2_hack

#endif /* D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_SWITCHABLE_SCENE_NODES_H */
