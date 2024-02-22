#ifndef D2_HACK_RESOURCE_DATA_INCLUDE_D2_HACK_RESOURCE_DATA_B3D_VISITOR_H
#define D2_HACK_RESOURCE_DATA_INCLUDE_D2_HACK_RESOURCE_DATA_B3D_VISITOR_H

#include <d2_hack/common/platform.h>

#include <d2_hack/resource/data/b3d_tree.h>

namespace d2_hack
{
namespace resource
{
namespace data
{
namespace b3d
{

class NodeVisitorInterface
{
public:
    virtual ~NodeVisitorInterface() = default;

    virtual [[nodiscard]] VisitResult Visit(NodeHierarchyBreaker& /*node */, VisitMode /* visidMode */) = 0;

    virtual [[nodiscard]] VisitResult Visit(NodeEmpty0& /* node */, VisitMode /* visitMode */) = 0;

    virtual [[nodiscard]] VisitResult Visit(NodeSimpleObjectConnector1& /* node */, VisitMode /* visitMode */) = 0;

    virtual [[nodiscard]] VisitResult Visit(NodeGroupUnknown2& /* node */, VisitMode /* visitMode */) = 0;

    virtual [[nodiscard]] VisitResult Visit(NodeGroupRoadInfraObjects4& /* node */, VisitMode /* visitMode */) = 0;

    virtual [[nodiscard]] VisitResult Visit(NodeGroupObjects5& /* node */, VisitMode /* visitMode */) = 0;

    virtual [[nodiscard]] VisitResult Visit(NodeGroupVertexData7& /* node */, VisitMode /* visitMode */) = 0;

    virtual [[nodiscard]] VisitResult Visit(NodeSimpleFaces8& /* node */, VisitMode /* visitMode */) = 0;

    virtual [[nodiscard]] VisitResult Visit(NodeGroupTrigger9& /* node */, VisitMode /* visitMode */) = 0;

    virtual [[nodiscard]] VisitResult Visit(NodeGroupLodParameters10& /* node */, VisitMode /* visitMode */) = 0;

    virtual [[nodiscard]] VisitResult Visit(NodeGroupUnknown12& /* node */, VisitMode /* visitMode */) = 0;

    virtual [[nodiscard]] VisitResult Visit(NodeSimpleTrigger13& /* node */, VisitMode /* visitMode */) = 0;

    virtual [[nodiscard]] VisitResult Visit(NodeSimpleUnknown14& /* node */, VisitMode /* visitMode */) = 0;

    virtual [[nodiscard]] VisitResult Visit(NodeSimpleObjectConnector18& /* node */, VisitMode /* visitMode */) = 0;

    virtual [[nodiscard]] VisitResult Visit(NodeGroupObjects19& /* node */, VisitMode /* visitMode */) = 0;

    virtual [[nodiscard]] VisitResult Visit(NodeSimpleFlatCollision20& /* node */, VisitMode /* visitMode */) = 0;

    virtual [[nodiscard]] VisitResult Visit(NodeGroupObjects21& /* node */, VisitMode /* visitMode */) = 0;

    virtual [[nodiscard]] VisitResult Visit(NodeSimpleVolumeCollision23& /* node */, VisitMode /* visitMode */) = 0;

    virtual [[nodiscard]] VisitResult Visit(NodeGroupTransformMatrix24& /* node */, VisitMode /* visitMode */) = 0;

    virtual [[nodiscard]] VisitResult Visit(NodeSimpleUnknown25& /* node */, VisitMode /* visitMode */) = 0;

    virtual [[nodiscard]] VisitResult Visit(NodeSimpleFaces28& /* node */, VisitMode /* visitMode */) = 0;

    virtual [[nodiscard]] VisitResult Visit(NodeGroupUnknown29& /* node */, VisitMode /* visitMode */) = 0;

    virtual [[nodiscard]] VisitResult Visit(NodeSimplePortal30& /* node */, VisitMode /* visitMode */) = 0;

    virtual [[nodiscard]] VisitResult Visit(NodeGroupLightingObjects33& /* node */, VisitMode /* visitMode */) = 0;

    virtual [[nodiscard]] VisitResult Visit(NodeSimpleUnknown34& /* node */, VisitMode /* visitMode */) = 0;

    virtual [[nodiscard]] VisitResult Visit(NodeSimpleFaces35& /* node */, VisitMode /* visitMode */) = 0;

    virtual [[nodiscard]] VisitResult Visit(NodeGroupVertexData36& /* node */, VisitMode /* visitMode */) = 0;

    virtual [[nodiscard]] VisitResult Visit(NodeGroupVertexData37& /* node */, VisitMode /* visitMode */) = 0;

    virtual [[nodiscard]] VisitResult Visit(NodeGroupUnknown39& /* node */, VisitMode /* visitMode */) = 0;

    virtual [[nodiscard]] VisitResult Visit(NodeSimpleGeneratedObjects40& /* node */, VisitMode /* visitMode */) = 0;
};


[[nodiscard]] VisitResult VisitNode(const NodePtr& node, NodeVisitorInterface& visitor);

[[nodiscard]] VisitResult VisitTree(const B3dTree& tree, NodeVisitorInterface& visitor);


template <typename BlockType>
class VisitableNodeWithData;

template <typename BlockType>
NodePtr MakeVisitableNode(const B3dTreeWeakPtr& originalRoot, const WeakNodePtr& parent, const block_data::BlockHeader& blockHeader, const BlockType& block);


template <typename BlockType>
class VisitableNodeWithData : public NodeWithData<BlockType>
{
    struct PrivateTag
    {
    };

    template <typename BlockType2>
    friend NodePtr MakeVisitableNode(const B3dTreeWeakPtr& originalRoot, const WeakNodePtr& parent, const block_data::BlockHeader& blockHeader, const BlockType2& block);
public:

    VisitableNodeWithData(const B3dTreeWeakPtr& originalRoot, const block_data::BlockHeader& blockHeader, const BlockType& block, const PrivateTag& /* unused */)
        : NodeWithData<BlockType>(originalRoot, blockHeader, block)
    {
    }

    virtual [[nodiscard]] VisitResult Visit(NodeVisitorInterface& visitor, VisitMode visitMode) override
    {
        return visitor.Visit(*this, visitMode);
    }
};

template <typename BlockType>
NodePtr MakeVisitableNode(const B3dTreeWeakPtr& originalRoot, const WeakNodePtr& parent, const block_data::BlockHeader& blockHeader, const BlockType& block)
{
    auto res = std::make_shared<VisitableNodeWithData<BlockType>>(originalRoot, blockHeader, block, VisitableNodeWithData<BlockType>::PrivateTag());

    auto p = parent.lock();
    if (p)
    {
        p->AddChildNode(res);
    }
    return res;
}


template <typename GenericAction>
class GenericActionVisitor: public NodeVisitorInterface
{
public:

    virtual [[nodiscard]] VisitResult Visit(NodeHierarchyBreaker& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }

    virtual [[nodiscard]] VisitResult Visit(NodeEmpty0& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }

    virtual [[nodiscard]] VisitResult Visit(NodeSimpleObjectConnector1& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }
    
    virtual [[nodiscard]] VisitResult Visit(NodeGroupUnknown2& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }

    virtual [[nodiscard]] VisitResult Visit(NodeGroupRoadInfraObjects4& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }

    virtual [[nodiscard]] VisitResult Visit(NodeGroupObjects5& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }

    virtual [[nodiscard]] VisitResult Visit(NodeGroupVertexData7& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }

    virtual [[nodiscard]] VisitResult Visit(NodeSimpleFaces8& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }

    virtual [[nodiscard]] VisitResult Visit(NodeGroupTrigger9& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }

    virtual [[nodiscard]] VisitResult Visit(NodeGroupLodParameters10& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }

    virtual [[nodiscard]] VisitResult Visit(NodeGroupUnknown12& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }

    virtual [[nodiscard]] VisitResult Visit(NodeSimpleTrigger13& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }

    virtual [[nodiscard]] VisitResult Visit(NodeSimpleUnknown14& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }

    virtual [[nodiscard]] VisitResult Visit(NodeSimpleObjectConnector18& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }

    virtual [[nodiscard]] VisitResult Visit(NodeGroupObjects19& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }

    virtual [[nodiscard]] VisitResult Visit(NodeSimpleFlatCollision20& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }

    virtual [[nodiscard]] VisitResult Visit(NodeGroupObjects21& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }

    virtual [[nodiscard]] VisitResult Visit(NodeSimpleVolumeCollision23& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }

    virtual [[nodiscard]] VisitResult Visit(NodeGroupTransformMatrix24& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }

    virtual [[nodiscard]] VisitResult Visit(NodeSimpleUnknown25& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }

    virtual [[nodiscard]] VisitResult Visit(NodeSimpleFaces28& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }

    virtual [[nodiscard]] VisitResult Visit(NodeGroupUnknown29& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }

    virtual [[nodiscard]] VisitResult Visit(NodeSimplePortal30& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }

    virtual [[nodiscard]] VisitResult Visit(NodeGroupLightingObjects33& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }

    virtual [[nodiscard]] VisitResult Visit(NodeSimpleUnknown34& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }

    virtual [[nodiscard]] VisitResult Visit(NodeSimpleFaces35& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }

    virtual [[nodiscard]] VisitResult Visit(NodeGroupVertexData36& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }

    virtual [[nodiscard]] VisitResult Visit(NodeGroupVertexData37& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }

    virtual [[nodiscard]] VisitResult Visit(NodeGroupUnknown39& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }

    virtual [[nodiscard]] VisitResult Visit(NodeSimpleGeneratedObjects40& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(node, visitMode);
    }
};

struct NoOpAction
{
    template <typename NodeType>
    static [[nodiscard]] VisitResult Perform(NodeType& /* node */, VisitMode /* visitMode */)
    {
        return VisitResult::Continue;
    }
};

using NoOpNodeVisitor = GenericActionVisitor<NoOpAction>;

struct RaiseExceptionAction
{
    template <typename NodeType>
    static [[nodiscard]] VisitResult Perform(NodeType& /* node */, VisitMode /* visitMode */)
    {
        RaiseException(NodeType::Name, NodeType::Value);
        return VisitResult::Stop;
    }

    static void RaiseException(const char* name, std::uint32_t type);
};

using RaiseExceptionVisitor = GenericActionVisitor<RaiseExceptionAction>;


} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack

#endif /* D2_HACK_RESOURCE_DATA_INCLUDE_D2_HACK_RESOURCE_DATA_B3D_VISITOR_H */
