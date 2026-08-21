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

    virtual VisitResult Visit(const std::shared_ptr<NodeHierarchyBreaker>& /*node */, VisitMode /* visidMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeEventEntry>& /*node */, VisitMode /* visidMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeEmpty0>& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleObjectConnector1>& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupUnknown2>& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupRoadInfraObjects4>& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupObjects5>& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupVertexData7>& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleFaces8>& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupTrigger9>& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupLodParameters10>& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupUnknown12>& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleTrigger13>& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleUnknown14>& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleObjectConnector18>& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupObjects19>& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleFlatCollision20>& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupObjects21>& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleVolumeCollision23>& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupTransformMatrix24>& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleUnknown25>& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleFaces28>& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupUnknown29>& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeSimplePortal30>& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupLightingObjects33>& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleUnknown34>& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleFaces35>& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupVertexData36>& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupVertexData37>& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupUnknown39>& /* node */, VisitMode /* visitMode */) = 0;

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleGeneratedObjects40>& /* node */, VisitMode /* visitMode */) = 0;
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
        return visitor.Visit(std::static_pointer_cast<VisitableNodeWithData<BlockType>>(this->shared_from_this()), visitMode);
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

    virtual VisitResult Visit(const std::shared_ptr<NodeHierarchyBreaker>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeEventEntry>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeEmpty0>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleObjectConnector1>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }
    
    virtual VisitResult Visit(const std::shared_ptr<NodeGroupUnknown2>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupRoadInfraObjects4>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupObjects5>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupVertexData7>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleFaces8>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupTrigger9>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupLodParameters10>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupUnknown12>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleTrigger13>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleUnknown14>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleObjectConnector18>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupObjects19>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleFlatCollision20>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupObjects21>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleVolumeCollision23>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupTransformMatrix24>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleUnknown25>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleFaces28>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupUnknown29>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeSimplePortal30>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupLightingObjects33>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleUnknown34>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleFaces35>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupVertexData36>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupVertexData37>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeGroupUnknown39>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }

    virtual VisitResult Visit(const std::shared_ptr<NodeSimpleGeneratedObjects40>& node, VisitMode visitMode) override
    {
        return GenericAction::Perform(static_cast<SelfType*>(this), node, visitMode);
    }
};

struct NoOpAction
{
    template <typename NodePtrType>
    static VisitResult Perform(const void* /* self*/, NodePtrType& /* node */, VisitMode /* visitMode */)
    {
        return VisitResult::Continue;
    }
};

class NoOpNodeVisitor : public GenericActionVisitor<NoOpNodeVisitor, NoOpAction>
{
};

struct RaiseExceptionAction
{
    template <typename NodePtrType>
    static VisitResult Perform(const void* /* self*/, NodePtrType& /* node */, VisitMode /* visitMode */)
    {
        using NodeType = NodePtrType::element_type;
        RaiseException(NodeType::Name, NodeType::Value);
        return VisitResult::Stop;
    }

    static void RaiseException(const std::string_view& name, std::uint32_t type);
};

class RaiseExceptionVisitor : public GenericActionVisitor<RaiseExceptionVisitor, RaiseExceptionAction>
{
};


} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack

#endif /* D2_HACK_RESOURCE_DATA_INCLUDE_D2_HACK_RESOURCE_DATA_B3D_VISITOR_H */
