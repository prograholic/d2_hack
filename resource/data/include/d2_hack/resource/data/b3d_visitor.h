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

    virtual VisitResult Visit(NodeHierarchyBreaker& /*node */, VisitMode /* visidMode */) = 0;

    virtual VisitResult Visit(NodeEventEntry& /*node */, VisitMode /* visidMode */) = 0;

    virtual VisitResult Visit(NodeEmpty0& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(NodeSimpleObjectConnector1& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(NodeGroupUnknown2& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(NodeGroupRoadInfraObjects4& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(NodeGroupObjects5& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(NodeGroupVertexData7& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(NodeSimpleFaces8& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(NodeGroupTrigger9& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(NodeGroupLodParameters10& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(NodeGroupUnknown12& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(NodeSimpleTrigger13& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(NodeSimpleUnknown14& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(NodeSimpleObjectConnector18& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(NodeGroupObjects19& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(NodeSimpleFlatCollision20& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(NodeGroupObjects21& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(NodeSimpleVolumeCollision23& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(NodeGroupTransformMatrix24& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(NodeSimpleUnknown25& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(NodeSimpleFaces28& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(NodeGroupUnknown29& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(NodeSimplePortal30& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(NodeGroupLightingObjects33& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(NodeSimpleUnknown34& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(NodeSimpleFaces35& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(NodeGroupVertexData36& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(NodeGroupVertexData37& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(NodeGroupUnknown39& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(NodeSimpleGeneratedObjects40& /* node */, VisitMode /* visitMode */) = 0;
};


VisitResult VisitNode(const B3dNodePtr& node, NodeVisitorInterface& visitor);

VisitResult VisitTree(const B3dTree& tree, NodeVisitorInterface& visitor);


template <typename BlockType>
class VisitableNodeWithData;

template <typename BlockType>
B3dNodePtr MakeVisitableNode(const B3dTreeWeakPtr& originalRoot, const common::WeakNodePtr& parent, const block_data::BlockHeader& blockHeader, const BlockType& block);


template <typename BlockType>
class VisitableNodeWithData : public NodeWithData<BlockType>
{
    struct PrivateTag
    {
    };

    template <typename BlockType2>
    friend B3dNodePtr MakeVisitableNode(const B3dTreeWeakPtr& originalRoot, const common::WeakNodePtr& parent, const block_data::BlockHeader& blockHeader, const BlockType2& block);
public:

    VisitableNodeWithData(const B3dTreeWeakPtr& originalRoot, const block_data::BlockHeader& blockHeader, const BlockType& block, const PrivateTag& /* unused */)
        : NodeWithData<BlockType>(originalRoot, blockHeader, block)
    {
    }

    virtual VisitResult Visit(NodeVisitorInterface& visitor, VisitMode visitMode) override
    {
        return visitor.Visit(*this, visitMode);
    }
};

template <typename BlockType>
B3dNodePtr MakeVisitableNode(const B3dTreeWeakPtr& originalRoot, const common::WeakNodePtr& parent, const block_data::BlockHeader& blockHeader, const BlockType& block)
{
    auto res = std::make_shared<VisitableNodeWithData<BlockType>>(originalRoot, blockHeader, block, VisitableNodeWithData<BlockType>::PrivateTag());

    auto p = parent.lock();
    if (p)
    {
        p->AddChildNode(res);
    }
    return res;
}


template <typename SelfType, typename GenericAction>
class GenericActionVisitor: public NodeVisitorInterface
{
public:

    virtual VisitResult Visit(NodeHierarchyBreaker& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(NodeEventEntry& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(NodeEmpty0& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(NodeSimpleObjectConnector1& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }
    
    virtual VisitResult Visit(NodeGroupUnknown2& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(NodeGroupRoadInfraObjects4& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(NodeGroupObjects5& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(NodeGroupVertexData7& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(NodeSimpleFaces8& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(NodeGroupTrigger9& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(NodeGroupLodParameters10& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(NodeGroupUnknown12& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(NodeSimpleTrigger13& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(NodeSimpleUnknown14& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(NodeSimpleObjectConnector18& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(NodeGroupObjects19& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(NodeSimpleFlatCollision20& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(NodeGroupObjects21& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(NodeSimpleVolumeCollision23& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(NodeGroupTransformMatrix24& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(NodeSimpleUnknown25& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(NodeSimpleFaces28& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(NodeGroupUnknown29& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(NodeSimplePortal30& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(NodeGroupLightingObjects33& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(NodeSimpleUnknown34& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(NodeSimpleFaces35& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(NodeGroupVertexData36& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(NodeGroupVertexData37& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(NodeGroupUnknown39& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(NodeSimpleGeneratedObjects40& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }
};

struct NoOpAction
{
    template <typename NodeType>
    static VisitResult Perform(const void* /* self*/, NodeType& /* node */, VisitMode /* visitMode */)
    {
        return VisitResult::Continue;
    }
};

class NoOpNodeVisitor : public GenericActionVisitor<NoOpNodeVisitor, NoOpAction>
{
};

struct RaiseExceptionAction
{
    template <typename NodeType>
    static VisitResult Perform(const void* /* self*/, NodeType& /* node */, VisitMode /* visitMode */)
    {
        RaiseException(NodeType::Name, NodeType::Value);
        return VisitResult::Stop;
    }

    static void RaiseException(const char* name, std::uint32_t type);
};

class RaiseExceptionVisitor : public GenericActionVisitor<RaiseExceptionVisitor, RaiseExceptionAction>
{
};


} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack

#endif /* D2_HACK_RESOURCE_DATA_INCLUDE_D2_HACK_RESOURCE_DATA_B3D_VISITOR_H */
