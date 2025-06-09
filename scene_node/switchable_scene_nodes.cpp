#include <d2_hack/scene_node/switchable_scene_nodes.h>

#include <d2_hack/common/log.h>


namespace d2_hack
{
namespace scene_node
{

EventEntrySceneNode::EventEntrySceneNode(const std::string& name, Ogre::SceneNode* ogreSceneNode)
    : OgreSceneNode<resource::data::b3d::block_data::EventEntryBlockXxx>(name, ogreSceneNode)
{
}


/////////////////////////////////////////////////////////////////////////////////////////////


SwitchableOgreSceneNode::SwitchableOgreSceneNode(const std::string& name, std::uint32_t type)
    : SceneNodeBase(name, type)
{
}

void SwitchableOgreSceneNode::OnCameraMoved(const WorldContext& worldContext, const Ogre::Vector3f& movement)
{
    SceneNodeBase* activeNode = ActivateItem(worldContext);
    if (activeNode)
    {
        activeNode->OnCameraMoved(worldContext, movement);
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////


GroupUnknown2::GroupUnknown2(
    const std::string& name,
    Ogre::SceneNode* ogreSceneNode,
    const resource::data::b3d::block_data::GroupUnknown2& /* data */)
    : OgreSceneNode<resource::data::b3d::block_data::GroupUnknownBlock2, SwitchableOgreSceneNode>(name, ogreSceneNode)
{
}

SceneNodeBase* GroupUnknown2::ActivateItem(const WorldContext& worldContext)
{
    // TODO: rework

    SceneNodeBase* res = nullptr;
    for (const auto& childNode : this->GetChildNodeList())
    {
        SceneNodeBase* childSceneNode = std::static_pointer_cast<SceneNodeBase>(childNode).get();
        if (res == nullptr)
        {
            res = childSceneNode;
            res->SetVisible(worldContext, true);
        }
        else
        {
            childSceneNode->SetVisible(worldContext, false);
        }
    }
    return res;
}



GroupTrigger9::GroupTrigger9(
    const std::string& name,
    Ogre::SceneNode* ogreSceneNode,
    const resource::data::b3d::block_data::GroupTrigger9& /* data */)
    : OgreSceneNode<resource::data::b3d::block_data::GroupTriggerBlock9, SwitchableOgreSceneNode>(name, ogreSceneNode)
{
}

SceneNodeBase* GroupTrigger9::ActivateItem(const WorldContext& worldContext)
{
    SceneNodeBase* res = nullptr;
    for (const auto& childNode : this->GetChildNodeList())
    {
        SceneNodeBase* childSceneNode = std::static_pointer_cast<SceneNodeBase>(childNode).get();
        if (res == nullptr)
        {
            res = childSceneNode;
            res->SetVisible(worldContext, true);
        }
        else
        {
            childSceneNode->SetVisible(worldContext, false);
        }
    }
    return res;
}



GroupLod10::GroupLod10(
    const std::string& name,
    Ogre::SceneNode* ogreSceneNode,
    const resource::data::b3d::block_data::GroupLodParameters10& data)
    : OgreSceneNode<resource::data::b3d::block_data::GroupLodParametersBlock10, SwitchableOgreSceneNode>(name, ogreSceneNode)
    , m_data(data)
{
}

SceneNodeBase* GroupLod10::ActivateItem(const WorldContext& worldContext)
{
    const auto& childs = GetChildNodeList();
    assert(childs.size() == 2);

    Ogre::Vector3f absoluteLodPosition = (GetAbsoluteOrientation() * m_data.unknown) + GetAbsolutePosition();

    bool isInsideLod = worldContext.playerPosition.distance(absoluteLodPosition) < m_data.distanceToPlayer;

    SceneNodeBase* active = std::static_pointer_cast<SceneNodeBase>(childs[isInsideLod ? 0 : 1]).get();
    SceneNodeBase* inactive = std::static_pointer_cast<SceneNodeBase>(childs[isInsideLod ? 1 : 0]).get();

    active->SetVisible(worldContext, true);
    inactive->SetVisible(worldContext, false);

    return active;
}


SceneNodeEvent21::SceneNodeEvent21(
    const std::string& name,
    Ogre::SceneNode* ogreSceneNode,
    const resource::data::b3d::block_data::GroupObjects21& /* data */)
    : OgreSceneNode<resource::data::b3d::block_data::GroupObjectsBlock21, SwitchableOgreSceneNode>(name, ogreSceneNode)
{
}

SceneNodeBase* SceneNodeEvent21::ActivateItem(const WorldContext& worldContext)
{
    // TODO: rework

    SceneNodeBase* res = nullptr;
    for (const auto& childNode : this->GetChildNodeList())
    {
        SceneNodeBase* childSceneNode = std::static_pointer_cast<SceneNodeBase>(childNode).get();
        if (res == nullptr)
        {
            res = childSceneNode;
            res->SetVisible(worldContext, true);
        }
        else
        {
            childSceneNode->SetVisible(worldContext, false);
        }
    }
    return res;
}


GroupUnknown29::GroupUnknown29(
    const std::string& name,
    Ogre::SceneNode* ogreSceneNode,
    const resource::data::b3d::block_data::GroupUnknown29& /* data */)
    : OgreSceneNode<resource::data::b3d::block_data::GroupUnknownBlock29, SwitchableOgreSceneNode>(name, ogreSceneNode)
{
}

SceneNodeBase* GroupUnknown29::ActivateItem(const WorldContext& worldContext)
{
    // TODO: rework

    SceneNodeBase* res = nullptr;
    for (const auto& childNode : this->GetChildNodeList())
    {
        SceneNodeBase* childSceneNode = std::static_pointer_cast<SceneNodeBase>(childNode).get();
        if (res == nullptr)
        {
            res = childSceneNode;
            res->SetVisible(worldContext, true);
        }
        else
        {
            childSceneNode->SetVisible(worldContext, false);
        }
    }
    return res;
}



} // namespace scene_node
} // namespace d2_hack
