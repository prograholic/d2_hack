#include <d2_hack/scene_node/switchable_scene_nodes.h>

namespace d2_hack
{
namespace scene_node
{


SwitchableOgreSceneNode::SwitchableOgreSceneNode(const std::string& name, std::uint32_t type)
    : SceneNodeBase(name, type)
{

}

void SwitchableOgreSceneNode::Activate(const WorldContext& worldContext)
{
    std::size_t active = GetActiveItem(worldContext);

    DoActivate(worldContext);

    const auto& childs = this->GetChildNodeList();
    for (std::size_t i = 0; i != childs.size(); ++i)
    {
        auto childSceneNode = std::static_pointer_cast<SceneNodeBase>(childs[i]);
        if (i == active)
        {
            childSceneNode->Activate(worldContext);
        }
        else
        {
            childSceneNode->Deactivate(worldContext);
        }
    }
}

GroupUnknown2::GroupUnknown2(
    const std::string& name,
    Ogre::SceneNode* ogreSceneNode,
    const resource::data::b3d::block_data::GroupUnknown2& /* data */)
    : OgreSceneNode<resource::data::b3d::block_data::GroupUnknownBlock2, SwitchableOgreSceneNode>(name, ogreSceneNode)
{
}

std::size_t GroupUnknown2::GetActiveItem(const WorldContext& /* worldContext */)
{
    return 0;
}



GroupTrigger9::GroupTrigger9(
    const std::string& name,
    Ogre::SceneNode* ogreSceneNode,
    const resource::data::b3d::block_data::GroupTrigger9& /* data */)
    : OgreSceneNode<resource::data::b3d::block_data::GroupTriggerBlock9, SwitchableOgreSceneNode>(name, ogreSceneNode)
{
}

std::size_t GroupTrigger9::GetActiveItem(const WorldContext& /* worldContext */)
{
    return 0;
}



GroupLod10::GroupLod10(
    const std::string& name,
    Ogre::SceneNode* ogreSceneNode,
    const resource::data::b3d::block_data::GroupLodParameters10& data)
    : OgreSceneNode<resource::data::b3d::block_data::GroupLodParametersBlock10, SwitchableOgreSceneNode>(name, ogreSceneNode)
    , m_data(data)
{
}

std::size_t GroupLod10::GetActiveItem(const WorldContext& worldContext)
{
    auto distance = worldContext.playerPosition.distance(m_data.unknown);
    if (distance > m_data.distanceToPlayer)
    {
        return 1;
    }

    return 0;
}


SceneNodeEvent21::SceneNodeEvent21(
    const std::string& name,
    Ogre::SceneNode* ogreSceneNode,
    const resource::data::b3d::block_data::GroupObjects21& /* data */)
    : OgreSceneNode<resource::data::b3d::block_data::GroupObjectsBlock21, SwitchableOgreSceneNode>(name, ogreSceneNode)
{
}

std::size_t SceneNodeEvent21::GetActiveItem(const WorldContext& /* worldContext */)
{
    return 0;
}


GroupUnknown29::GroupUnknown29(
    const std::string& name,
    Ogre::SceneNode* ogreSceneNode,
    const resource::data::b3d::block_data::GroupUnknown29& /* data */)
    : OgreSceneNode<resource::data::b3d::block_data::GroupUnknownBlock29, SwitchableOgreSceneNode>(name, ogreSceneNode)
{
}

std::size_t GroupUnknown29::GetActiveItem(const WorldContext& /* worldContext */)
{
    return 0;
}



} // namespace scene_node
} // namespace d2_hack
