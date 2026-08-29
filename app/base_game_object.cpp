#include <d2_hack/app/base_game_object.h>

#include <OgreException.h>

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



MoveableObject::MoveableObject(std::string_view name, scene_node::SceneNodeBaseList rootNodes)
    : BaseGameObject(name, std::move(rootNodes))
{
}

void MoveableObject::SetPosition(const Ogre::Vector3& /* position */)
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "MoveableObject::SetPosition not implemented");
}


} // namespace app
} // namespace d2_hack
