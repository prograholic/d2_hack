#include "b3d_room.h"

#include "b3d_tree_visitor.h"

namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;

B3dRoom::B3dRoom(const B3dNodePtr& b3dNode,
                 const std::string& b3dId,
                 Ogre::SceneManager* sceneManager,
                 Ogre::SceneNode* rootNode,
                 Ogre::MeshManager* meshManager,
                 resource::archive::res::OgreMaterialProvider* ogreMaterialProvider)
    : BaseGameObject(b3dNode, b3dId, sceneManager, rootNode, meshManager, ogreMaterialProvider)
{
}

} // namespace app
} // namespace d2_hack
