#include <d2_hack/app/base_game_object.h>

namespace d2_hack
{
namespace app
{

BaseGameObject::BaseGameObject(scene_node::SceneNodeBaseList rootNodes)
    : m_rootNodes(std::move(rootNodes))
{
}

const scene_node::SceneNodeBaseList& BaseGameObject::GetRootNodes() const
{
    return m_rootNodes;
}


} // namespace app
} // namespace d2_hack
