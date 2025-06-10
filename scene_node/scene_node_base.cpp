#include <d2_hack/scene_node/scene_node_base.h>

namespace d2_hack
{
namespace scene_node
{

size_t SceneNodeBase::m_scNodeCount = 0;

SceneNodeBase::SceneNodeBase(const std::string& name, std::uint32_t type)
    : common::NodeBase(name, type)
{
    m_scNodeCount += 1;
}

SceneNodeBase::~SceneNodeBase()
{
    assert(m_scNodeCount > 0);

    m_scNodeCount -= 1;
}

size_t SceneNodeBase::GetSceneNodeBaseCount()
{
    return m_scNodeCount;
}

} // namespace scene_node
} // namespace d2_hack
