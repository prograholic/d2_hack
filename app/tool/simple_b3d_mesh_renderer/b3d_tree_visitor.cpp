#include "b3d_tree_visitor.h"

#include <d2_hack/scene_node/scene_node_base.h>
#include <d2_hack/scene_node/switchable_scene_nodes.h>
#include <d2_hack/scene_node/collision_scene_nodes.h>
#include <d2_hack/scene_node/terrain_scene_node.h>
#include <d2_hack/scene_node/renderable_scene_nodes.h>

#include <d2_hack/common/utils.h>
#include <d2_hack/common/log.h>

//#define B3D_NOT_IMPLEMENTED() D2_HACK_LOG("") << __FUNCSIG__ << ": NOT IMPLEMENTED"
#define B3D_NOT_IMPLEMENTED()


namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;


B3dTreeVisitor::B3dTreeVisitor(B3dSceneBuilder& sceneBuilder)
    : m_sceneBuilder(sceneBuilder)
{
}

B3dSceneBuilder& B3dTreeVisitor::GetSceneBuilder()
{
    return m_sceneBuilder;
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeEventEntry& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::EventEntrySceneNode>(parent, node.GetName(), sceneNode);
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupUnknown2& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::GroupUnknown2>(parent, node.GetName(), sceneNode, node.GetBlockData());
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupRoadInfraObjects4& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::OgreSceneNode<NodeGroupRoadInfraObjects4::Value>>(parent, node.GetName(), sceneNode);
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupObjects5& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::OgreSceneNode<NodeGroupObjects5::Value>>(parent, node.GetName(), sceneNode);
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupVertexData7& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::GroupVertexData7>(parent, node.GetName(), sceneNode);
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleFaces8& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    auto res = CreateSceneNode<scene_node::OgreSceneNode<NodeSimpleFaces8::Value>>(parent, node.GetName(), sceneNode);

    VisitFaces(node);

    return res;
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupTrigger9& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::GroupTrigger9>(parent, node.GetName(), sceneNode, node.GetBlockData());
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupLodParameters10& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::GroupLod10>(parent, node.GetName(), sceneNode, node.GetBlockData());
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupUnknown12& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::OgreSceneNode<NodeGroupUnknown12::Value>>(parent, node.GetName(), sceneNode);
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleTrigger13& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::OgreSceneNode<NodeSimpleTrigger13::Value>>(parent, node.GetName(), sceneNode);
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleUnknown14& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::OgreSceneNode<NodeSimpleUnknown14::Value>>(parent, node.GetName(), sceneNode);
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleObjectConnector18& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    auto res = CreateSceneNode<scene_node::OgreSceneNode<NodeSimpleObjectConnector18::Value>>(parent, node.GetName(), sceneNode);

    GetSceneBuilder().ProcessObjectConnector(node);

    return res;
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupObjects19& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::OgreSceneNode<NodeGroupObjects19::Value>>(parent, node.GetName(), sceneNode);
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleFlatCollision20& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* /* sceneNode */)
{
    return CreateSceneNode<scene_node::SimpleFlatCollision20>(parent, node.GetName(), node.GetBlockData());
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupObjects21& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::SceneNodeEvent21>(parent, node.GetName(), sceneNode, node.GetBlockData());
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleVolumeCollision23& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* /* sceneNode */)
{
    return CreateSceneNode<scene_node::SimpleVolumeCollision23>(parent, node.GetName(), node.GetBlockData());
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleUnknown25& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::OgreSceneNode<NodeSimpleUnknown25::Value>>(parent, node.GetName(), sceneNode);
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleFaces28& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    auto res = CreateSceneNode<scene_node::OgreSceneNode<NodeSimpleFaces28::Value>>(parent, node.GetName(), sceneNode);

    VisitFaces(node);

    return res;
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupUnknown29& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::GroupUnknown29>(parent, node.GetName(), sceneNode, node.GetBlockData());
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimplePortal30& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::OgreSceneNode<NodeSimplePortal30::Value>>(parent, node.GetName(), sceneNode);
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupLightingObjects33& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::OgreSceneNode<NodeGroupLightingObjects33::Value>>(parent, node.GetName(), sceneNode);
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleUnknown34& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::OgreSceneNode<NodeSimpleUnknown34::Value>>(parent, node.GetName(), sceneNode);
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleFaces35& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    auto res = CreateSceneNode<scene_node::OgreSceneNode<NodeSimpleFaces35::Value>>(parent, node.GetName(), sceneNode);

    VisitFaces(node);

    return res;
}

scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupVertexData37& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateSceneNode<scene_node::GroupVertexData37>(parent, node.GetName(), sceneNode);
}


scene_node::SceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleGeneratedObjects40& node, const scene_node::SceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    auto generatorName = common::ResourceNameToString(node.GetBlockData().name);
    if (generatorName == "$$GeneratorOfTerrain")
    {
        return CreateSceneNode<scene_node::TerrainSceneNode>(parent, "terrain." + node.GetName(), sceneNode, GetSceneBuilder().GetSceneManager());
    }
    else
    {
        D2_HACK_LOG(B3dTreeVisitor::CreateNode) << "skipping unsupported generator: " << generatorName;
        return CreateSceneNode<scene_node::OgreSceneNode<NodeSimpleGeneratedObjects40::Value>>(parent, node.GetName(), sceneNode);
    }
}


/////////////////////////////////////////////////////////////////////////////////////


template <typename FacesNode>
void B3dTreeVisitor::VisitFaces(FacesNode& node)
{
    const auto& block = node.GetBlockData();
    for (const auto& face : block.faces)
    {
        m_sceneBuilder.CreateMesh(node.GetName(), face.meshInfo, node.GetOriginalRoot()->GetMaterialNameByIndex(face.materialIndex));
    }
}

} // namespace app
} // namespace d2_hack
