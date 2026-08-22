#include <d2_hack/scene_node/switchable_scene_nodes.h>

#include <OgreSubEntity.h>

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


/////////////////////////////////////////////////////////////////////////////////////////////

SubEntitiesSceneNode::SubEntitiesSceneNode(
    const std::string_view& name,
    std::vector<size_t> subEntityIds)
    : SceneNode<resource::data::b3d::block_data::SubMeshBlockXxx, SceneNodeBase>(name)
    , m_subEntityIds(std::move(subEntityIds))
{
}

void SubEntitiesSceneNode::Activate(bool active)
{
    for (const auto& subEntityId : m_subEntityIds)
    {
        GetEntity()->getSubEntity(subEntityId)->setVisible(active);
    }
}

Ogre::Vector3f SubEntitiesSceneNode::GetAbsolutePosition() const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "SubEntitiesSceneNode::GetAbsolutePosition not implemented");
}

Ogre::Quaternion SubEntitiesSceneNode::GetAbsoluteOrientation() const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "SubEntitiesSceneNode::GetAbsoluteOrientation not implemented");
}

/////////////////////////////////////////////////////////////////////////////////////////////


GroupUnknown2::GroupUnknown2(
    const std::string_view& name,
    const resource::data::b3d::block_data::GroupUnknown2& /* data */)
    : SceneNode<resource::data::b3d::block_data::GroupUnknownBlock2, SceneNodeBase>(name)
{
}

Ogre::Vector3f GroupUnknown2::GetAbsolutePosition() const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "GroupUnknown2::GetAbsolutePosition not implemented");
}

Ogre::Quaternion GroupUnknown2::GetAbsoluteOrientation() const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "GroupUnknown2::GetAbsoluteOrientation not implemented");
}



GroupTrigger9::GroupTrigger9(
    const std::string_view& name,
    const resource::data::b3d::block_data::GroupTrigger9& /* data */)
    : SceneNode<resource::data::b3d::block_data::GroupTriggerBlock9, SceneNodeBase>(name)
{
}

Ogre::Vector3f GroupTrigger9::GetAbsolutePosition() const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "GroupTrigger9::GetAbsolutePosition not implemented");
}

Ogre::Quaternion GroupTrigger9::GetAbsoluteOrientation() const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "GroupTrigger9::GetAbsoluteOrientation not implemented");
}


SceneNodeEvent21::SceneNodeEvent21(
    const std::string_view& name,
    const resource::data::b3d::block_data::GroupObjects21& data)
    : SceneNode<resource::data::b3d::block_data::GroupObjectsBlock21, SceneNodeBase>(name)
    , m_activeEntry(nullptr)
    , m_defaultEntryId(data.defaultValue)
{
}

Ogre::Vector3f SceneNodeEvent21::GetAbsolutePosition() const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "SceneNodeEvent21::GetAbsolutePosition not implemented");
}

Ogre::Quaternion SceneNodeEvent21::GetAbsoluteOrientation() const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "SceneNodeEvent21::GetAbsoluteOrientation not implemented");
}

void SceneNodeEvent21::Activate(bool active)
{
    m_activeEntry->Activate(active);
}

void SceneNodeEvent21::ApplyState(std::string_view stateName, size_t stateId)
{
    assert(stateId < GetChildNodeList().size());

    if (stateName == GetName())
    {
        for (size_t i = 0; i != GetChildNodeList().size(); ++i)
        {
            EventEntrySceneNode* eventEntry = std::static_pointer_cast<EventEntrySceneNode>(GetChildNodeList()[i]).get();

            eventEntry->Activate(i == stateId);
        }
        m_activeEntry = std::static_pointer_cast<EventEntrySceneNode>(GetChildNodeList()[stateId]).get();
    }

    m_activeEntry->ApplyState(stateName, stateId);
}

void SceneNodeEvent21::DoInit()
{
    ApplyState(GetName(), m_defaultEntryId);
}


GroupUnknown29::GroupUnknown29(
    const std::string_view& name,
    const resource::data::b3d::block_data::GroupUnknown29& /* data */)
    : SceneNode<resource::data::b3d::block_data::GroupUnknownBlock29, SceneNodeBase>(name)
{
}

Ogre::Vector3f GroupUnknown29::GetAbsolutePosition() const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "GroupUnknown29::GetAbsolutePosition not implemented");
}

Ogre::Quaternion GroupUnknown29::GetAbsoluteOrientation() const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "GroupUnknown29::GetAbsoluteOrientation not implemented");
}


} // namespace scene_node
} // namespace d2_hack
