#include <d2_hack/resource/data/b3d_tree_optimization.h>


#include <d2_hack/resource/data/b3d_utils.h>
#include <d2_hack/common/log.h>

namespace d2_hack
{
namespace resource
{
namespace data
{
namespace b3d
{
namespace optimization
{

template <typename BlockType>
void DoMerge(BlockType& blockData, const common::SimpleMeshInfo* parentMeshInfo)
{
    assert(parentMeshInfo);

    for (auto& mesh : blockData.faces)
    {
        if (!mesh.meshInfo.positions)
        {
            mesh.meshInfo.positions = parentMeshInfo->positions;
        }
        if (!mesh.meshInfo.texCoords)
        {
            mesh.meshInfo.texCoords = parentMeshInfo->texCoords;
        }
        if (!mesh.meshInfo.normals)
        {
            mesh.meshInfo.normals = parentMeshInfo->normals;
        }
        if (!mesh.meshInfo.indices)
        {
            mesh.meshInfo.indices = parentMeshInfo->indices;
        }
    }
}

static void MergeFacesWithVertices(const NodePtr& node, const common::SimpleMeshInfo* parentMeshInfo)
{
    if (node->GetType() == block_data::GroupIndexAndTexturesBlock37)
    {
        NodeGroupVertexData37* typedNode = node->NodeCast<NodeGroupVertexData37>();

        parentMeshInfo = std::addressof(typedNode->GetBlockData().meshInfo);
    }
    else if (node->GetType() == block_data::GroupVertexBlock7)
    {
        NodeGroupVertex7* typedNode = node->NodeCast<NodeGroupVertex7>();

        parentMeshInfo = std::addressof(typedNode->GetBlockData().meshInfo);
    }
    else if (node->GetType() == block_data::SimpleFacesBlock8)
    {
        NodeSimpleFaces8* typedNode = node->NodeCast<NodeSimpleFaces8>();
        DoMerge(typedNode->GetBlockData(), parentMeshInfo);
    }
    else if (node->GetType() == block_data::SimpleFacesBlock28)
    {
        NodeSimpleFaces28* typedNode = node->NodeCast<NodeSimpleFaces28>();
        DoMerge(typedNode->GetBlockData(), parentMeshInfo);
    }
    else if (node->GetType() == block_data::SimpleFacesBlock35)
    {
        NodeSimpleFaces35* typedNode = node->NodeCast<NodeSimpleFaces35>();
        DoMerge(typedNode->GetBlockData(), parentMeshInfo);
    }

    for (const auto& child : node->GetChildNodeList())
    {
        MergeFacesWithVertices(child, parentMeshInfo);
    }
}

static void MergeFacesWithVertices(const B3dTree& tree)
{
    for (const auto& node : tree.rootNodes)
    {
        MergeFacesWithVertices(node, nullptr);
    }
}

////////////////////////////////////////////////////////////////////////////////

static bool NeedToRemove(const NodePtr& node)
{
    const auto& children = node->GetChildNodeList();

    NodeList newChildList;
    for (auto child : children)
    {
        bool needToRemove = NeedToRemove(child);
        if (!needToRemove)
        {
            newChildList.push_back(child);
        }
        else
        {
            D2_HACK_LOG(NeedToRemove) << "removed node: " << child->GetName() << " with type: " << child->GetType();
        }
    }

    node->SetChildNodes(std::move(newChildList));

    if (node->GetChildNodeList().empty())
    {
        switch (node->GetType())
        {
        case block_data::GroupRoadInfraObjectsBlock4:
        case block_data::GroupObjectsBlock5:
        case block_data::GroupObjectsBlock19:
        case block_data::GroupObjectsBlock21:
            return true;
        };
    }

    return false;
}

static void RemoveEmptyNodes(B3dTree& tree)
{
    for (const auto& node : tree.rootNodes)
    {
        NeedToRemove(node);
    }
}

////////////////////////////////////////////////////////////////////////////////

void Optimize(B3dTree& tree)
{
    MergeFacesWithVertices(tree);
    RemoveEmptyNodes(tree);
}

} // namespace optimization
} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack
