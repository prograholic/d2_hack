#include "b3d_reader_visitors.h"

namespace d2_hack
{
namespace resource
{
namespace data
{
namespace b3d
{

TransformVisitor::TransformVisitor(TransformationMap& transformations)
    : NoOpNodeVisitor()
    , m_transformQueue()
    , m_transformMap(transformations)
{
}

TransformVisitor::~TransformVisitor()
{
    assert(m_transformQueue.empty());
}

VisitResult TransformVisitor::Visit(NodeGroupTransformMatrix24& node, VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        m_transformQueue.push_back(node.GetBlockData());
    }
    else
    {
        TransformList transformList;
        for (const auto& item : m_transformQueue)
        {
            Transform transform;

            transform.matrix.FromAxes(item.x, item.y, item.z);
            transform.position = item.position;

            transformList.push_back(transform);
        }

        m_transformMap[node.GetName()] = transformList;
        m_transformQueue.pop_back();
    }

    return VisitResult::Continue;
}



} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack
