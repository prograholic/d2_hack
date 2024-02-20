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


void RaiseExceptionAction::RaiseException(const char* name, std::uint32_t /* type */)
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "`Visit` method is not implemented for type " + std::string(name));
}

} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack
