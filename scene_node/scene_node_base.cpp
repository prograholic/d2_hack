#include <d2_hack/scene_node/scene_node_base.h>

namespace d2_hack
{
namespace scene_node
{


SceneNodeBase::SceneNodeBase(const std::string& name, std::uint32_t type)
    : common::NodeBase(name, type)
{
}

} // namespace scene_node
} // namespace d2_hack
