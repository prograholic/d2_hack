#ifndef D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_SCENE_NODE_BASE_H
#define D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_SCENE_NODE_BASE_H

#include <d2_hack/common/platform.h>

#include <OgreSceneNode.h>

#include <d2_hack/common/node_base.h>

namespace d2_hack
{
namespace scene_node
{

struct WorldContext
{
    Ogre::Vector3f playerPosition;
};

class SceneNodeBase : public common::NodeBase
{
public:
    SceneNodeBase(const std::string& name, std::uint32_t type);

    virtual void Activate(const WorldContext& worldContext)
    {
        DoActivate(worldContext);
        for (const auto& childNode : this->GetChildNodeList())
        {
            auto childSceneNode = std::static_pointer_cast<SceneNodeBase>(childNode);
            childSceneNode->Activate(worldContext);
        }
    }

    virtual void Deactivate(const WorldContext& worldContext)
    {
        DoDeactivate(worldContext);
        for (const auto& childNode : this->GetChildNodeList())
        {
            auto childSceneNode = std::static_pointer_cast<SceneNodeBase>(childNode);
            childSceneNode->Deactivate(worldContext);
        }
    }

protected:
    virtual void DoActivate(const WorldContext& worldContext) = 0;

    virtual void DoDeactivate(const WorldContext& worldContext) = 0;
};

typedef std::shared_ptr<SceneNodeBase> SceneNodeBasePtr;
typedef std::deque<SceneNodeBasePtr> SceneNodeBaseList;


template <std::uint32_t NodeTypeId, typename SceneNodeBaseType>
class SceneNode : public SceneNodeBaseType
{
public:
    static constexpr std::uint32_t Value = NodeTypeId;

    template<typename... Args>
    SceneNode(const std::string& name, Args&&... args)
        : SceneNodeBaseType(name, Value, std::forward<Args&&>(args)...)
    {
    }
};

template <std::uint32_t NodeTypeId, typename SceneNodeBaseType = SceneNodeBase>
class OgreSceneNode : public SceneNode<NodeTypeId, SceneNodeBaseType>
{
public:

    template<typename... Args>
    OgreSceneNode(const std::string& name, Ogre::SceneNode* ogreSceneNode, Args&&... args)
        : SceneNode<NodeTypeId, SceneNodeBaseType>(name, std::forward<Args&&>(args)...)
        , m_ogreSceneNode(ogreSceneNode)
    {
    }

private:
    Ogre::SceneNode* m_ogreSceneNode;

    virtual void DoActivate(const WorldContext& /* worldContext */) override
    {
        m_ogreSceneNode->setVisible(true);
    }

    virtual void DoDeactivate(const WorldContext& /* worldContext */) override
    {
        m_ogreSceneNode->setVisible(false);
    }
};

template <typename Node, typename... Args>
std::shared_ptr<Node> CreateSceneNode(const SceneNodeBasePtr& parent, Args&&... args)
{
    auto res = std::make_shared<Node>(std::forward<Args&&>(args)...);
    if (parent)
    {
        parent->AddChildNode(res);
    }

    return res;
}

} // namespace scene_node
} // namespace d2_hack

#endif /* D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_SCENE_NODE_BASE_H */
