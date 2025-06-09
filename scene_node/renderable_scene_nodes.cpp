#include <d2_hack/scene_node/renderable_scene_nodes.h>

namespace d2_hack
{
namespace scene_node
{

GroupVertexData7::GroupVertexData7(const std::string& name, Ogre::SceneNode* ogreSceneNode)
    : OgreSceneNode< resource::data::b3d::block_data::GroupVertexDataBlock7>(name, ogreSceneNode)
{
}

GroupVertexData37::GroupVertexData37(const std::string& name, Ogre::SceneNode* ogreSceneNode)
    : OgreSceneNode< resource::data::b3d::block_data::GroupVertexDataBlock37>(name, ogreSceneNode)
{
}


} // namespace scene_node
} // namespace d2_hack
