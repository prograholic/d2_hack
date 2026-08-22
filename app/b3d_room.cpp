#include <d2_hack/app/b3d_room.h>

#include "b3d_tree_visitor.h"

namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;

B3dRoom::B3dRoom(std::string_view name, scene_node::SceneNodeBaseList rootNodes)
    : BaseGameObject(name, std::move(rootNodes))
{
}

} // namespace app
} // namespace d2_hack
