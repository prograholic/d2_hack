#include "b3d_tree_visitor.h"

#include <OgreSceneNode.h>

#include <d2_hack/common/utils.h>
#include <d2_hack/common/log.h>

#include <d2_hack/resource/data/b3d_utils.h>

//#define B3D_NOT_IMPLEMENTED() D2_HACK_LOG("") << __FUNCSIG__ << ": NOT IMPLEMENTED"
#define B3D_NOT_IMPLEMENTED()


namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;


B3dSceneNodeBase::B3dSceneNodeBase(const std::string& name, std::uint32_t type)
    : common::NodeBase(name, type)
{
}


template <std::uint32_t NodeTypeId>
class B3dSceneNode : public B3dSceneNodeBase
{
public:
    static constexpr std::uint32_t Value = NodeTypeId;

    explicit B3dSceneNode(const std::string& name)
        : B3dSceneNodeBase(name, Value)
    {
    }
};


template <std::uint32_t NodeTypeId>
class B3dOgreSceneNode : public B3dSceneNode<NodeTypeId>
{
public:

    B3dOgreSceneNode(const std::string& name, Ogre::SceneNode* sceneNode)
        : B3dSceneNode<NodeTypeId>(name)
        , m_sceneNode(sceneNode)
    {
    }

    virtual void SetVisible(bool visible) override
    {
        m_sceneNode->setVisible(visible);
    }

    static B3dSceneNodeBasePtr Create(const B3dSceneNodeBasePtr& parent, const std::string& name, Ogre::SceneNode* sceneNode)
    {
        auto res = std::make_shared<B3dOgreSceneNode<NodeTypeId>>(name, sceneNode);
        if (parent)
        {
            parent->AddChildNode(res);
        }

        return res;
    }

private:
    Ogre::SceneNode* m_sceneNode;
};


typedef B3dOgreSceneNode<block_data::GroupObjectsBlock19> GroupObject19B3dSceneNode;




B3dTreeVisitor::B3dTreeVisitor(B3dSceneBuilder& sceneBuilder)
    : m_sceneBuilder(sceneBuilder)
{
}

B3dSceneBuilder& B3dTreeVisitor::GetSceneBuilder()
{
    return m_sceneBuilder;
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const resource::data::b3d::NodeGroupObjects5& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return B3dOgreSceneNode<NodeGroupObjects5::Value>::Create(parent, node.GetName(), sceneNode);
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const resource::data::b3d::NodeSimpleFaces8& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    auto res = B3dOgreSceneNode<NodeSimpleFaces8::Value>::Create(parent, node.GetName(), sceneNode);

    VisitFaces(node);

    return res;
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const resource::data::b3d::NodeGroupLodParameters10& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return B3dOgreSceneNode<NodeGroupLodParameters10::Value>::Create(parent, node.GetName(), sceneNode);
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const resource::data::b3d::NodeGroupObjects19& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return B3dOgreSceneNode<NodeGroupObjects19::Value>::Create(parent, node.GetName(), sceneNode);
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const resource::data::b3d::NodeGroupVertexData37& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    return B3dOgreSceneNode<NodeGroupVertexData37::Value>::Create(parent, node.GetName(), sceneNode);
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const resource::data::b3d::NodeSimpleFaces28& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    auto res = B3dOgreSceneNode<NodeSimpleFaces28::Value>::Create(parent, node.GetName(), sceneNode);

    VisitFaces(node);

    return res;
}

B3dSceneNodeBasePtr B3dTreeVisitor::CreateNode(const resource::data::b3d::NodeSimpleFaces35& node, const B3dSceneNodeBasePtr& parent, Ogre::SceneNode* sceneNode)
{
    auto res = B3dOgreSceneNode<NodeSimpleFaces35::Value>::Create(parent, node.GetName(), sceneNode);

    VisitFaces(node);

    return res;
}


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
