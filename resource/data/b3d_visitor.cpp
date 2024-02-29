#include <d2_hack/resource/data/b3d_visitor.h>

namespace d2_hack
{
namespace resource
{
namespace data
{
namespace b3d
{

VisitResult VisitNode(const NodePtr& node, NodeVisitorInterface& visitor)
{
    auto preOrderVisitResult = node->Visit(visitor, VisitMode::PreOrder);
    switch (preOrderVisitResult)
    {
    case VisitResult::Continue:
        break;

    case VisitResult::Stop:
        return preOrderVisitResult;
    default:
        break;
    }

    if ((preOrderVisitResult != VisitResult::SkipChildren) && (preOrderVisitResult != VisitResult::SkipChildrenAndPostOrder))
    {
        const auto& children = node->GetChildNodeList();
        for (auto child : children)
        {
            auto childVisitResult = VisitNode(child, visitor);
            if (childVisitResult == VisitResult::Stop)
            {
                return childVisitResult;
            }
        }
    }

    if (preOrderVisitResult != VisitResult::SkipChildrenAndPostOrder)
    {
        auto postOrderVisitResult = node->Visit(visitor, VisitMode::PostOrder);
        assert((postOrderVisitResult == VisitResult::Continue) || (postOrderVisitResult == VisitResult::Stop));

        return postOrderVisitResult;
    }
    
    return preOrderVisitResult;
}

VisitResult VisitTree(const B3dTree& tree, NodeVisitorInterface& visitor)
{
    for (auto node : tree.rootNodes)
    {
        auto visitResult = VisitNode(node, visitor);
        if (visitResult == VisitResult::Stop)
        {
            return VisitResult::Stop;
        }

        assert(visitResult == VisitResult::Continue);
    }

    return VisitResult::Continue;
}


void RaiseExceptionAction::RaiseException(const char* name, std::uint32_t /* type */)
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "`Visit` method is not implemented for type " + std::string(name));
}

} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack
