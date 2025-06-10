#ifndef D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_NODE_BASE_H
#define D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_NODE_BASE_H

#include <d2_hack/common/platform.h>

#include <cassert>
#include <string>
#include <memory>
#include <deque>


namespace d2_hack
{
namespace common
{

typedef std::shared_ptr<class NodeBase> NodePtr;
typedef std::weak_ptr<class NodeBase> WeakNodePtr;
typedef std::deque<NodePtr> NodeList;



class NodeBase : public std::enable_shared_from_this<NodeBase>
{
    NodeBase(const NodeBase&) = delete;
    NodeBase& operator=(const NodeBase&) = delete;

protected:
    NodeBase(const std::string& name, std::uint32_t type);

public:

    virtual ~NodeBase() noexcept = default;

    const std::string& GetName() const;

    std::uint32_t GetType() const;

    template <typename TypedNode>
    TypedNode* NodeCast()
    {
        assert(GetType() == TypedNode::Value);

        return static_cast<TypedNode*>(this);
    }

    template <typename TypedNode>
    const TypedNode* NodeCast() const
    {
        assert(GetType() == TypedNode::Value);

        return static_cast<const TypedNode*>(this);
    }

    template <typename TypedNode>
    TypedNode* TryNodeCast()
    {
        if (GetType() == TypedNode::Value)
        {
            return static_cast<TypedNode*>(this);
        }

        return nullptr;
    }

    template <typename TypedNode>
    const TypedNode* TryNodeCast() const
    {
        if (GetType() == TypedNode::Value)
        {
            return static_cast<const TypedNode*>(this);
        }

        return nullptr;
    }

    NodeList& GetChildNodeList();

    void SetChildNodes(NodeList&& childNodes);

    void AddChildNode(const NodePtr& node);

    bool HasParent() const;

    NodePtr GetParent();

    void SetParent(const WeakNodePtr& parent);

private:
    NodeList m_childNodeList;
    WeakNodePtr m_parent;

    const std::string m_name;
    const std::uint32_t m_type;
};

} // namespace common
} // namespace d2_hack


#endif /* D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_NODE_BASE_H */
