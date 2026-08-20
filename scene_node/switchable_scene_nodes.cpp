#include <d2_hack/scene_node/switchable_scene_nodes.h>

#include <d2_hack/common/log.h>


namespace d2_hack
{
namespace scene_node
{

EventEntrySceneNode::EventEntrySceneNode(const std::string_view& name)
    : SceneNode<resource::data::b3d::block_data::EventEntryBlockXxx, SceneNodeBase>(name)
{
}

Ogre::Vector3f EventEntrySceneNode::GetAbsolutePosition() const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "EventEntrySceneNode::GetAbsolutePosition is not implemented");
}

Ogre::Quaternion EventEntrySceneNode::GetAbsoluteOrientation() const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "EventEntrySceneNode::GetAbsoluteOrientation is not implemented");
}

void EventEntrySceneNode::SetVisible(bool /* visible */)
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "EventEntrySceneNode::SetVisible is not implemented");
}


/////////////////////////////////////////////////////////////////////////////////////////////


SwitchableSceneNode::SwitchableSceneNode(const std::string_view& name, std::uint32_t type)
    : SceneNodeBase(name, type)
{
}

Ogre::Vector3f SwitchableSceneNode::GetAbsolutePosition() const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "SwitchableSceneNode::GetAbsolutePosition is not implemented");
}

Ogre::Quaternion SwitchableSceneNode::GetAbsoluteOrientation() const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "SwitchableSceneNode::GetAbsoluteOrientation is not implemented");
}

void SwitchableSceneNode::SetVisible(bool /* visible */)
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "SwitchableSceneNode::SetVisible is not implemented");
}


/////////////////////////////////////////////////////////////////////////////////////////////


GroupUnknown2::GroupUnknown2(
    const std::string_view& name,
    const resource::data::b3d::block_data::GroupUnknown2& /* data */)
    : SceneNode<resource::data::b3d::block_data::GroupUnknownBlock2, SwitchableSceneNode>(name)
{
}

SceneNodeBase* GroupUnknown2::ActivateItem(const WorldContext& /* worldContext */)
{
    // TODO: rework

    SceneNodeBase* res = nullptr;
    for (const auto& childNode : this->GetChildNodeList())
    {
        SceneNodeBase* childSceneNode = std::static_pointer_cast<SceneNodeBase>(childNode).get();
        if (res == nullptr)
        {
            res = childSceneNode;
            res->SetVisible(true);
        }
        else
        {
            childSceneNode->SetVisible(false);
        }
    }
    return res;
}



GroupTrigger9::GroupTrigger9(
    const std::string_view& name,
    const resource::data::b3d::block_data::GroupTrigger9& /* data */)
    : SceneNode<resource::data::b3d::block_data::GroupTriggerBlock9, SwitchableSceneNode>(name)
{
}

SceneNodeBase* GroupTrigger9::ActivateItem(const WorldContext& /* worldContext */)
{
    SceneNodeBase* res = nullptr;
    for (const auto& childNode : this->GetChildNodeList())
    {
        SceneNodeBase* childSceneNode = std::static_pointer_cast<SceneNodeBase>(childNode).get();
        if (res == nullptr)
        {
            res = childSceneNode;
            res->SetVisible(true);
        }
        else
        {
            childSceneNode->SetVisible(false);
        }
    }
    return res;
}



SceneNodeEvent21::SceneNodeEvent21(
    const std::string_view& name,
    const resource::data::b3d::block_data::GroupObjects21& /* data */)
    : SceneNode<resource::data::b3d::block_data::GroupObjectsBlock21, SwitchableSceneNode>(name)
{
}

SceneNodeBase* SceneNodeEvent21::ActivateItem(const WorldContext& /* worldContext */)
{
    // TODO: rework

    SceneNodeBase* res = nullptr;
    for (const auto& childNode : this->GetChildNodeList())
    {
        SceneNodeBase* childSceneNode = std::static_pointer_cast<SceneNodeBase>(childNode).get();
        if (res == nullptr)
        {
            res = childSceneNode;
            childSceneNode->SetVisible(true);
        }
        else
        {
            childSceneNode->SetVisible(false);
        }
    }
    return res;
}


GroupUnknown29::GroupUnknown29(
    const std::string_view& name,
    const resource::data::b3d::block_data::GroupUnknown29& /* data */)
    : SceneNode<resource::data::b3d::block_data::GroupUnknownBlock29, SwitchableSceneNode>(name)
{
}

SceneNodeBase* GroupUnknown29::ActivateItem(const WorldContext& /* worldContext */)
{
    // TODO: rework

    SceneNodeBase* res = nullptr;
    for (const auto& childNode : this->GetChildNodeList())
    {
        SceneNodeBase* childSceneNode = std::static_pointer_cast<SceneNodeBase>(childNode).get();
        if (res == nullptr)
        {
            res = childSceneNode;
            res->SetVisible(true);
        }
        else
        {
            childSceneNode->SetVisible(false);
        }
    }
    return res;
}



} // namespace scene_node
} // namespace d2_hack
