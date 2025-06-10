#include "b3d_car.h"

namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;


B3dCar::B3dCar(const resource::data::b3d::B3dNodePtr& b3dNode,
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
