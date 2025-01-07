#include <d2_hack/scene_node/renderable_scene_nodes.h>

namespace d2_hack
{
namespace scene_node
{

GroupVertexData7::GroupVertexData7(const std::string& name, Ogre::SceneNode* ogreSceneNode)
    : OgreSceneNode< resource::data::b3d::block_data::GroupVertexDataBlock7>(name, ogreSceneNode)
{
}

void GroupVertexData7::PlayerMoved(const WorldContext& worldContext, const Ogre::Vector3f& movement)
{
    for (const auto& childNode : this->GetChildNodeList())
    {
        SceneNodeBase* childSceneNode = std::static_pointer_cast<SceneNodeBase>(childNode).get();
        childSceneNode->PlayerMoved(worldContext, movement);
    }
}


GroupVertexData37::GroupVertexData37(const std::string& name, Ogre::SceneNode* ogreSceneNode)
    : OgreSceneNode< resource::data::b3d::block_data::GroupVertexDataBlock37>(name, ogreSceneNode)
{
}

void GroupVertexData37::PlayerMoved(const WorldContext& worldContext, const Ogre::Vector3f& movement)
{
    for (const auto& childNode : this->GetChildNodeList())
    {
        SceneNodeBase* childSceneNode = std::static_pointer_cast<SceneNodeBase>(childNode).get();
        childSceneNode->PlayerMoved(worldContext, movement);
    }
}

} // namespace scene_node
} // namespace d2_hack
