#include <d2_hack/app/base_game_object.h>

namespace d2_hack
{
namespace app
{

BaseGameObject::BaseGameObject(scene_node::SceneNodeBaseList rootNodes)
    : m_rootNodes(std::move(rootNodes))
{
}

void BaseGameObject::OnCameraMoved(const scene_node::WorldContext& worldContext, const Ogre::Vector3f& movement)
{
    for (const auto& rootNode : m_rootNodes)
    {
        rootNode->OnCameraMoved(worldContext, movement);
    }
}


} // namespace app
} // namespace d2_hack
