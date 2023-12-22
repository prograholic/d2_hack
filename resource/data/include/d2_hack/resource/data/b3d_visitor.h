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

    virtual void Visit(NodeEmpty0& /* node */, VisitMode /* visitMode */) = 0;

    virtual void Visit(NodeSimpleObjectConnector1& /* node */, VisitMode /* visitMode */) = 0;

    virtual void Visit(NodeGroupUnknown2& /* node */, VisitMode /* visitMode */) = 0;

    virtual void Visit(NodeGroupRoadInfraObjects4& /* node */, VisitMode /* visitMode */) = 0;

    virtual void Visit(NodeGroupObjects5& /* node */, VisitMode /* visitMode */) = 0;

    virtual void Visit(NodeGroupVertexData7& /* node */, VisitMode /* visitMode */) = 0;

    virtual void Visit(NodeSimpleFaces8& /* node */, VisitMode /* visitMode */) = 0;

    virtual void Visit(NodeGroupTrigger9& /* node */, VisitMode /* visitMode */) = 0;

    virtual void Visit(NodeGroupLodParameters10& /* node */, VisitMode /* visitMode */) = 0;

    virtual void Visit(NodeGroupUnknown12& /* node */, VisitMode /* visitMode */) = 0;

    virtual void Visit(NodeSimpleTrigger13& /* node */, VisitMode /* visitMode */) = 0;

    virtual void Visit(NodeSimpleUnknown14& /* node */, VisitMode /* visitMode */) = 0;

    virtual void Visit(NodeSimpleObjectConnector18& /* node */, VisitMode /* visitMode */) = 0;

    virtual void Visit(NodeGroupObjects19& /* node */, VisitMode /* visitMode */) = 0;

    virtual void Visit(NodeSimpleFlatCollision20& /* node */, VisitMode /* visitMode */) = 0;

    virtual void Visit(NodeGroupObjects21& /* node */, VisitMode /* visitMode */) = 0;

    virtual void Visit(NodeSimpleVolumeCollision23& /* node */, VisitMode /* visitMode */) = 0;

    virtual void Visit(NodeGroupTransformMatrix24& /* node */, VisitMode /* visitMode */) = 0;

    virtual void Visit(NodeSimpleUnknown25& /* node */, VisitMode /* visitMode */) = 0;

    virtual void Visit(NodeSimpleFaces28& /* node */, VisitMode /* visitMode */) = 0;

    virtual void Visit(NodeGroupUnknown29& /* node */, VisitMode /* visitMode */) = 0;

    virtual void Visit(NodeSimplePortal30& /* node */, VisitMode /* visitMode */) = 0;

    virtual void Visit(NodeGroupLightingObjects33& /* node */, VisitMode /* visitMode */) = 0;

    virtual void Visit(NodeSimpleUnknown34& /* node */, VisitMode /* visitMode */) = 0;

    virtual void Visit(NodeSimpleFaces35& /* node */, VisitMode /* visitMode */) = 0;

    virtual void Visit(NodeGroupVertexData36& /* node */, VisitMode /* visitMode */) = 0;

    virtual void Visit(NodeGroupVertexData37& /* node */, VisitMode /* visitMode */) = 0;

    virtual void Visit(NodeGroupUnknown39& /* node */, VisitMode /* visitMode */) = 0;

    virtual void Visit(NodeSimpleGeneratedObjects40& /* node */, VisitMode /* visitMode */) = 0;
};


template <typename BlockType>
class VisitableNodeWithData : public NodeWithData<BlockType>
{
public:

    VisitableNodeWithData(const B3dTreeWeakPtr& originalRoot, const WeakNodePtr& parent, const block_data::BlockHeader& blockHeader, const BlockType& block)
        : NodeWithData<BlockType>(originalRoot, parent, blockHeader, block)
    {
    }

    virtual void Visit(NodeVisitorInterface& visitor, VisitMode visitMode) override
    {
        visitor.Visit(*this, visitMode);
    }
};


class NoOpNodeVisitor: public NodeVisitorInterface
{
public:
    virtual void Visit(NodeEmpty0& /* node */, VisitMode /* visitMode */) override;

    virtual void Visit(NodeGroupUnknown2& /* node */, VisitMode /* visitMode */) override;

    virtual void Visit(NodeGroupRoadInfraObjects4& /* node */, VisitMode /* visitMode */) override;

    virtual void Visit(NodeGroupObjects5& /* node */, VisitMode /* visitMode */) override;

    virtual void Visit(NodeGroupVertexData7& /* node */, VisitMode /* visitMode */) override;

    virtual void Visit(NodeSimpleFaces8& /* node */, VisitMode /* visitMode */) override;

    virtual void Visit(NodeGroupTrigger9& /* node */, VisitMode /* visitMode */) override;

    virtual void Visit(NodeGroupLodParameters10& /* node */, VisitMode /* visitMode */) override;

    virtual void Visit(NodeGroupUnknown12& /* node */, VisitMode /* visitMode */) override;

    virtual void Visit(NodeSimpleTrigger13& /* node */, VisitMode /* visitMode */) override;

    virtual void Visit(NodeSimpleUnknown14& /* node */, VisitMode /* visitMode */) override;

    virtual void Visit(NodeSimpleObjectConnector18& /* node */, VisitMode /* visitMode */) override;

    virtual void Visit(NodeGroupObjects19& /* node */, VisitMode /* visitMode */) override;

    virtual void Visit(NodeSimpleFlatCollision20& /* node */, VisitMode /* visitMode */) override;

    virtual void Visit(NodeGroupObjects21& /* node */, VisitMode /* visitMode */) override;

    virtual void Visit(NodeSimpleVolumeCollision23& /* node */, VisitMode /* visitMode */) override;

    virtual void Visit(NodeGroupTransformMatrix24& /* node */, VisitMode /* visitMode */) override;

    virtual void Visit(NodeSimpleUnknown25& /* node */, VisitMode /* visitMode */) override;

    virtual void Visit(NodeSimpleFaces28& /* node */, VisitMode /* visitMode */) override;

    virtual void Visit(NodeGroupUnknown29& /* node */, VisitMode /* visitMode */) override;

    virtual void Visit(NodeSimplePortal30& /* node */, VisitMode /* visitMode */) override;

    virtual void Visit(NodeGroupLightingObjects33& /* node */, VisitMode /* visitMode */) override;

    virtual void Visit(NodeSimpleUnknown34& /* node */, VisitMode /* visitMode */) override;

    virtual void Visit(NodeSimpleFaces35& /* node */, VisitMode /* visitMode */) override;

    virtual void Visit(NodeGroupVertexData36& /* node */, VisitMode /* visitMode */) override;

    virtual void Visit(NodeGroupVertexData37& /* node */, VisitMode /* visitMode */) override;

    virtual void Visit(NodeSimpleGeneratedObjects40& /* node */, VisitMode /* visitMode */) override;
};

} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack

#endif /* D2_HACK_RESOURCE_DATA_INCLUDE_D2_HACK_RESOURCE_DATA_B3D_VISITOR_H */
