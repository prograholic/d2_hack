#include "b3d_tree_visitor.h"

#include <OgreSceneNode.h>

#include <d2_hack/common/utils.h>
#include <d2_hack/common/log.h>

#include <d2_hack/resource/data/b3d_utils.h>

#include "terrain.h"

//#define B3D_NOT_IMPLEMENTED() D2_HACK_LOG("") << __FUNCSIG__ << ": NOT IMPLEMENTED"
#define B3D_NOT_IMPLEMENTED()


namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;

class B3dSceneNodeEvent : public B3dOgreSceneNode<block_data::GroupObjectsBlock21>
{
public:
    B3dSceneNodeEvent(size_t activeNode, const std::string& name, Ogre::SceneNode* sceneNode)
        : B3dOgreSceneNode<block_data::GroupObjectsBlock21>(name, sceneNode)
        , m_activeNode(activeNode)
    {
    }

    virtual void SetVisible(bool visible) override
    {
        auto& childs = GetChildNodeList();

        assert(m_activeNode < childs.size());

        for (size_t i = 0; i != childs.size(); ++i)
        {
            auto b3dSceneNode = std::static_pointer_cast<B3dSceneNodeBase>(childs[i]);
            if (i == m_activeNode)
            {
                b3dSceneNode->SetVisible(visible);
            }
            else
            {
                b3dSceneNode->SetVisible(false);
            }
        }
    }

private:

    const size_t m_activeNode;
};

B3dTreeVisitor::B3dTreeVisitor(B3dSceneBuilder& sceneBuilder)
    : m_sceneBuilder(sceneBuilder)
{
}

B3dSceneBuilder& B3dTreeVisitor::GetSceneBuilder()
{
    return m_sceneBuilder;
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeEventEntry& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateB3dSceneNode<B3dOgreSceneNode<NodeEventEntry::Value>>(parent, node.GetName(), sceneNode);
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupUnknown2& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateB3dSceneNode<B3dOgreSceneNode<NodeGroupUnknown2::Value>>(parent, node.GetName(), sceneNode);
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupRoadInfraObjects4& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateB3dSceneNode<B3dOgreSceneNode<NodeGroupRoadInfraObjects4::Value>>(parent, node.GetName(), sceneNode);
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupObjects5& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateB3dSceneNode<B3dOgreSceneNode<NodeGroupObjects5::Value>>(parent, node.GetName(), sceneNode);
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupVertexData7& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateB3dSceneNode<B3dOgreSceneNode<NodeGroupVertexData7::Value>>(parent, node.GetName(), sceneNode);
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleFaces8& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    auto res = CreateB3dSceneNode<B3dOgreSceneNode<NodeSimpleFaces8::Value>>(parent, node.GetName(), sceneNode);

    VisitFaces(node);

    return res;
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupTrigger9& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateB3dSceneNode<B3dOgreSceneNode<NodeGroupTrigger9::Value>>(parent, node.GetName(), sceneNode);
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupLodParameters10& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateB3dSceneNode<B3dOgreSceneNode<NodeGroupLodParameters10::Value>>(parent, node.GetName(), sceneNode);
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupUnknown12& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateB3dSceneNode<B3dOgreSceneNode<NodeGroupUnknown12::Value>>(parent, node.GetName(), sceneNode);
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleTrigger13& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateB3dSceneNode<B3dOgreSceneNode<NodeSimpleTrigger13::Value>>(parent, node.GetName(), sceneNode);
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleUnknown14& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateB3dSceneNode<B3dOgreSceneNode<NodeSimpleUnknown14::Value>>(parent, node.GetName(), sceneNode);
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleObjectConnector18& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    auto res = CreateB3dSceneNode<B3dOgreSceneNode<NodeSimpleObjectConnector18::Value>>(parent, node.GetName(), sceneNode);

    GetSceneBuilder().ProcessObjectConnector(node);

    return res;
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupObjects19& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateB3dSceneNode<B3dOgreSceneNode<NodeGroupObjects19::Value>>(parent, node.GetName(), sceneNode);
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleFlatCollision20& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateB3dSceneNode<B3dOgreSceneNode<NodeSimpleFlatCollision20::Value>>(parent, node.GetName(), sceneNode);
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupObjects21& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateB3dSceneNode<B3dSceneNodeEvent>(parent, node.GetBlockData().defaultValue, node.GetName(), sceneNode);
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleVolumeCollision23& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateB3dSceneNode<B3dOgreSceneNode<NodeSimpleVolumeCollision23::Value>>(parent, node.GetName(), sceneNode);
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleUnknown25& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateB3dSceneNode<B3dOgreSceneNode<NodeSimpleUnknown25::Value>>(parent, node.GetName(), sceneNode);
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleFaces28& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    auto res = CreateB3dSceneNode<B3dOgreSceneNode<NodeSimpleFaces28::Value>>(parent, node.GetName(), sceneNode);

    VisitFaces(node);

    return res;
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupUnknown29& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateB3dSceneNode<B3dOgreSceneNode<NodeGroupUnknown29::Value>>(parent, node.GetName(), sceneNode);
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimplePortal30& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateB3dSceneNode<B3dOgreSceneNode<NodeSimplePortal30::Value>>(parent, node.GetName(), sceneNode);
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupLightingObjects33& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateB3dSceneNode<B3dOgreSceneNode<NodeGroupLightingObjects33::Value>>(parent, node.GetName(), sceneNode);
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleUnknown34& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateB3dSceneNode<B3dOgreSceneNode<NodeSimpleUnknown34::Value>>(parent, node.GetName(), sceneNode);
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleFaces35& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    auto res = CreateB3dSceneNode<B3dOgreSceneNode<NodeSimpleFaces35::Value>>(parent, node.GetName(), sceneNode);

    VisitFaces(node);

    return res;
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeGroupVertexData37& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return CreateB3dSceneNode<B3dOgreSceneNode<NodeGroupVertexData37::Value>>(parent, node.GetName(), sceneNode);
}


B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const NodeSimpleGeneratedObjects40& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    auto generatorName = common::ResourceNameToString(node.GetBlockData().name);
    if (generatorName == "$$GeneratorOfTerrain")
    {
        return CreateB3dSceneNode<Terrain>(parent, GetSceneBuilder().GetSceneManager(), sceneNode);
    }
    else
    {
        D2_HACK_LOG(B3dTreeVisitor::CreateNode) << "skipping unsupported generator: " << generatorName;
        return CreateB3dSceneNode<B3dOgreSceneNode<NodeSimpleGeneratedObjects40::Value>>(parent, node.GetName(), sceneNode);
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
