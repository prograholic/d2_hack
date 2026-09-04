#include <d2_hack/app/b3d_room.h>

namespace d2_hack
{
namespace app
{

Tree::Tree(std::string_view name, scene_node::SceneNodeBaseList rootNodes)
    : MoveableObject(name, std::move(rootNodes))
{
}

B3dRoom::B3dRoom(std::string_view name, scene_node::SceneNodeBaseList rootNodes, Trees trees)
    : BaseGameObject(name, std::move(rootNodes))
    , m_trees(std::move(trees))
{
}

void B3dRoom::ApplyEvent(const char* eventName, size_t eventId)
{
    for (const auto& rootNode : m_rootNodes)
    {
        rootNode->ApplyState(eventName, eventId);
    }
}

} // namespace app
} // namespace d2_hack
