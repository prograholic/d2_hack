#include <d2_hack/app/base_game_object.h>

namespace d2_hack
{
namespace app
{

BaseGameObject::BaseGameObject(std::string_view name, scene_node::SceneNodeBaseList rootNodes)
    : m_rootNodes(std::move(rootNodes))
    , m_name(name)
{
}

std::string_view BaseGameObject::GetName() const
{
    return m_name;
}

const scene_node::SceneNodeBaseList& BaseGameObject::GetRootNodes() const
{
    return m_rootNodes;
}


} // namespace app
} // namespace d2_hack
