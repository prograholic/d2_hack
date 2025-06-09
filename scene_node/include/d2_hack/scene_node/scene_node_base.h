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

    /**
     * @param worldContext - stores actual player location
     * @param movement - delta between previous player location and current
     */
    virtual void OnCameraMoved(const WorldContext& worldContext, const Ogre::Vector3f& movement)
    {
        for (const auto& childNode : this->GetChildNodeList())
        {
            SceneNodeBase* childSceneNode = std::static_pointer_cast<SceneNodeBase>(childNode).get();
            childSceneNode->OnCameraMoved(worldContext, movement);
        }
    }

    virtual void SetVisible(const WorldContext& worldContext, bool visible) = 0;

    virtual Ogre::Vector3f GetAbsolutePosition() const = 0;

    virtual Ogre::Quaternion GetAbsoluteOrientation() const = 0;
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

    virtual void SetVisible(const WorldContext& /* worldContext */, bool visible) override
    {
        m_ogreSceneNode->setVisible(visible);
    }

    virtual Ogre::Vector3f GetAbsolutePosition() const override
    {
        return m_ogreSceneNode->_getDerivedPosition();
    }

    virtual Ogre::Quaternion GetAbsoluteOrientation() const override
    {
        return m_ogreSceneNode->_getDerivedOrientation();
    }

private:
    Ogre::SceneNode* m_ogreSceneNode;
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
