#include "b3d_car.h"

#include "b3d_tree_visitor.h"

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
    : m_rootNodes()
{
    B3dSceneBuilder sceneBuilder{b3dId, sceneManager, rootNode, meshManager, ogreMaterialProvider, m_rootNodes};
    B3dTreeVisitor visitor{sceneBuilder};
    auto visitResult = VisitNode(b3dNode, visitor);
    (void)visitResult;
}

} // namespace app
} // namespace d2_hack
