#include <d2_hack/resource/data/b3d_visitor.h>

namespace d2_hack
{
namespace resource
{
namespace data
{
namespace b3d
{

void VisitNode(const NodePtr& node, NodeVisitorInterface& visitor)
{
    node->Visit(visitor, VisitMode::PreOrder);

    const auto& children = node->GetChildNodeList();
    for (auto child : children)
    {
        VisitNode(child, visitor);
    }

    node->Visit(visitor, VisitMode::PostOrder);
}

void VisitTree(const B3dTree& tree, NodeVisitorInterface& visitor)
{
    for (auto node : tree.rootNodes)
    {
        VisitNode(node, visitor);
    }
}


void NoOpNodeVisitor::Visit(NodeHierarchyBreaker& /* node */, VisitMode /* visitMode */)
{
}

void NoOpNodeVisitor::Visit(NodeEmpty0& /* node */, VisitMode /* visitMode */)
{
}

void NoOpNodeVisitor::Visit(NodeSimpleObjectConnector1& /* node */, VisitMode /* visitMode */)
{
}

void NoOpNodeVisitor::Visit(NodeGroupUnknown2& /* node */, VisitMode /* visitMode */)
{
}

void NoOpNodeVisitor::Visit(NodeGroupRoadInfraObjects4& /* node */, VisitMode /* visitMode */)
{
}

void NoOpNodeVisitor::Visit(NodeGroupObjects5& /* node */, VisitMode /* visitMode */)
{
}

void NoOpNodeVisitor::Visit(NodeGroupVertexData7& /* node */, VisitMode /* visitMode */)
{
}

void NoOpNodeVisitor::Visit(NodeSimpleFaces8& /* node */, VisitMode /* visitMode */)
{
}

void NoOpNodeVisitor::Visit(NodeGroupTrigger9& /* node */, VisitMode /* visitMode */)
{
}

void NoOpNodeVisitor::Visit(NodeGroupLodParameters10& /* node */, VisitMode /* visitMode */)
{
}

void NoOpNodeVisitor::Visit(NodeGroupUnknown12& /* node */, VisitMode /* visitMode */)
{
}

void NoOpNodeVisitor::Visit(NodeSimpleTrigger13& /* node */, VisitMode /* visitMode */)
{
}

void NoOpNodeVisitor::Visit(NodeSimpleUnknown14& /* node */, VisitMode /* visitMode */)
{
}

void NoOpNodeVisitor::Visit(NodeSimpleObjectConnector18& /* node */, VisitMode /* visitMode */)
{
}

void NoOpNodeVisitor::Visit(NodeGroupObjects19& /* node */, VisitMode /* visitMode */)
{
}

void NoOpNodeVisitor::Visit(NodeSimpleFlatCollision20& /* node */, VisitMode /* visitMode */)
{
}

void NoOpNodeVisitor::Visit(NodeGroupObjects21& /* node */, VisitMode /* visitMode */)
{
}

void NoOpNodeVisitor::Visit(NodeSimpleVolumeCollision23& /* node */, VisitMode /* visitMode */)
{
}

void NoOpNodeVisitor::Visit(NodeGroupTransformMatrix24& /* node */, VisitMode /* visitMode */)
{
}

void NoOpNodeVisitor::Visit(NodeSimpleUnknown25& /* node */, VisitMode /* visitMode */)
{
}

void NoOpNodeVisitor::Visit(NodeSimpleFaces28& /* node */, VisitMode /* visitMode */)
{
}

void NoOpNodeVisitor::Visit(NodeGroupUnknown29& /* node */, VisitMode /* visitMode */)
{
}

void NoOpNodeVisitor::Visit(NodeSimplePortal30& /* node */, VisitMode /* visitMode */)
{
}

void NoOpNodeVisitor::Visit(NodeGroupLightingObjects33& /* node */, VisitMode /* visitMode */)
{
}

void NoOpNodeVisitor::Visit(NodeSimpleUnknown34& /* node */, VisitMode /* visitMode */)
{
}

void NoOpNodeVisitor::Visit(NodeSimpleFaces35& /* node */, VisitMode /* visitMode */)
{
}

void NoOpNodeVisitor::Visit(NodeGroupVertexData36& /* node */, VisitMode /* visitMode */)
{
}

void NoOpNodeVisitor::Visit(NodeGroupVertexData37& /* node */, VisitMode /* visitMode */)
{
}

void NoOpNodeVisitor::Visit(NodeGroupUnknown39& /* node */, VisitMode /* visitMode */)
{
}

void NoOpNodeVisitor::Visit(NodeSimpleGeneratedObjects40& /* node */, VisitMode /* visitMode */)
{
}

} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack
