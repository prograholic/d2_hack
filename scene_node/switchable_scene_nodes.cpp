#include <d2_hack/scene_node/switchable_scene_nodes.h>

namespace d2_hack
{
namespace scene_node
{


SwitchableSceneNodeBase::SwitchableSceneNodeBase(const std::string& name, std::uint32_t type, size_t activeNode)
    : SceneNodeBase(name, type)
    , m_activeNode(activeNode)
{
}

void SwitchableSceneNodeBase::SetVisible(bool visible)
{
    auto& childs = GetChildNodeList();

    assert(m_activeNode < childs.size());

    for (size_t i = 0; i != childs.size(); ++i)
    {
        auto b3dSceneNode = std::static_pointer_cast<SceneNodeBase>(childs[i]);
        if (i == m_activeNode)
        {
            b3dSceneNode->SetVisible(visible);
        }
        else
        {
            b3dSceneNode->SetVisible(false);
        }
    }
}

void SwitchableSceneNodeBase::SetActive(size_t activeNode)
{
    auto& childs = GetChildNodeList();

    if (activeNode >= childs.size())
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "activeNodeId too big: " + std::to_string(activeNode) + ", childs count: " + std::to_string(childs.size()));
    }

    m_activeNode = activeNode;
    SetVisible(true); //TODO: possibly incorrect implementation of SetVisible + SetActive
}

size_t SwitchableSceneNodeBase::GetActiveNode() const
{
    return m_activeNode;
}

} // namespace scene_node
} // namespace d2_hack
