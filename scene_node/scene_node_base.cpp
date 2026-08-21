#include <d2_hack/scene_node/scene_node_base.h>

namespace d2_hack
{
namespace scene_node
{

size_t SceneNodeBase::m_scNodeCount = 0;

SceneNodeBase::SceneNodeBase(const std::string_view& name, std::uint32_t type)
    : common::NodeBase(name, type)
    , m_entity(nullptr)
{
    m_scNodeCount += 1;
}

SceneNodeBase::~SceneNodeBase()
{
    assert(m_scNodeCount > 0);

    m_scNodeCount -= 1;
}

void SceneNodeBase::Initialize(Ogre::Entity* entity)
{
    m_entity = entity;

    for (auto& childNode : GetChildNodeList())
    {
        std::static_pointer_cast<SceneNodeBase>(childNode)->Initialize(entity);
    }

    DoInit();
}

size_t SceneNodeBase::GetSceneNodeBaseCount()
{
    return m_scNodeCount;
}

void SceneNodeBase::ApplyState(std::string_view stateName, size_t stateId)
{
    for (const auto& child : GetChildNodeList())
    {
        std::static_pointer_cast<SceneNodeBase>(child)->ApplyState(stateName, stateId);
    }
}

void SceneNodeBase::Activate(bool active)
{
    for (const auto& child : GetChildNodeList())
    {
        std::static_pointer_cast<SceneNodeBase>(child)->Activate(active);
    }
}

void SceneNodeBase::DoInit()
{
    // pass
}

} // namespace scene_node
} // namespace d2_hack
