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
    SceneNodeBase(const std::string_view& name, std::uint32_t type);

    ~SceneNodeBase() noexcept;

    static size_t GetSceneNodeBaseCount();

    void Initialize(Ogre::SceneNode* sceneNode);

    Ogre::SceneNode* GetSceneNode();

    virtual void ApplyState(std::string_view stateName, size_t stateId);

    virtual void Activate(bool visible);

    virtual Ogre::Vector3f GetAbsolutePosition() const = 0;

    virtual Ogre::Quaternion GetAbsoluteOrientation() const = 0;

private:
    static size_t m_scNodeCount;

    virtual void DoInit();

protected:
    Ogre::SceneNode* m_sceneNode;
    Ogre::Entity* GetEntity();
};

typedef std::shared_ptr<SceneNodeBase> SceneNodeBasePtr;
typedef std::deque<SceneNodeBasePtr> SceneNodeBaseList;


class OgreSceneNodeBase : public SceneNodeBase
{
public:
    OgreSceneNodeBase(const std::string_view& name, std::uint32_t type, Ogre::SceneNode* ogreSceneNode)
        : SceneNodeBase(name, type)
        , m_ogreSceneNode(ogreSceneNode)
    {
    }

    void SetVisible(bool visible)
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

    Ogre::SceneNode* GetOgreSceneNode()
    {
        return m_ogreSceneNode;
    }

private:
    Ogre::SceneNode* m_ogreSceneNode;
};

template <std::uint32_t NodeTypeId, typename SceneNodeBaseType>
class SceneNode : public SceneNodeBaseType
{
public:
    static constexpr std::uint32_t Value = NodeTypeId;

    template<typename... Args>
    SceneNode(const std::string_view& name, Args&&... args)
        : SceneNodeBaseType(name, Value, std::forward<Args&&>(args)...)
    {
    }
};

template <std::uint32_t NodeTypeId, typename SceneNodeBaseType = OgreSceneNodeBase>
class OgreSceneNode : public SceneNode<NodeTypeId, SceneNodeBaseType>
{
public:

    template<typename... Args>
    OgreSceneNode(const std::string_view& name, Ogre::SceneNode* ogreSceneNode, Args&&... args)
        : SceneNode<NodeTypeId, SceneNodeBaseType>(name, ogreSceneNode, std::forward<Args&&>(args)...)
    {
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
