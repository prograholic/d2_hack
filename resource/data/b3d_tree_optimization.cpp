#include <d2_hack/resource/data/b3d_tree_optimization.h>


#include <d2_hack/resource/data/b3d_utils.h>
#include <d2_hack/common/log.h>
#include <d2_hack/common/utils.h>

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
namespace predicates
{

static bool IsNodeOfType(NodePtr node, const std::uint32_t* begin, const std::uint32_t* end)
{
    auto nodeType = node->GetType();
    while (begin != end)
    {
        if (*begin == nodeType)
        {
            return true;
        }
        ++begin;
    }

    return false;
}
static bool IsUnusedNode(NodePtr node)
{
    static const std::uint32_t unusedNodeTypes[] =
    {
        block_data::SimpleFlatCollisionBlock20,
        block_data::SimpleVolumeCollisionBlock23
    };

    return IsNodeOfType(node, std::begin(unusedNodeTypes), std::end(unusedNodeTypes));
}

static bool IsVertexNode(NodePtr node)
{
    static const std::uint32_t vertexNodeTypes[] =
    {
        block_data::GroupVertexBlock7,
        block_data::GroupIndexAndTexturesBlock37
    };

    return IsNodeOfType(node, std::begin(vertexNodeTypes), std::end(vertexNodeTypes));
}

} // namespace predicates

static void FilterUnusedNodes(NodePtr node)
{
    NodeList newChildren;
    for (auto child : node->GetChildNodeList())
    {
        if (predicates::IsUnusedNode(child))
        {
            D2_HACK_LOG(FilterUnusedNodes) << "Skipping unused node " << child->GetName() << ", with type: " << child->GetType();
            continue;
        }

        FilterUnusedNodes(child);

        newChildren.push_back(child);
    }

    node->SetChildNodes(std::move(newChildren));
}

static void FilterUnusedNodes(B3dTree& tree)
{
    for (const auto& node : tree.rootNodes)
    {
        FilterUnusedNodes(node);
    }
}

////////////////////////////////////////////////////////////////////////////////

static void SkipLodParametersFor37(NodePtr node, bool hasLod)
{
    if (hasLod)
    {
        NodeList newChildren;
        for (auto child : node->GetChildNodeList())
        {
            if (child->GetType() == block_data::GroupLodParametersBlock10)
            {
                D2_HACK_LOG(SkipLodParametersFor37) << "skipping node " << child->GetName();
                continue;
            }
            newChildren.push_back(child);
        }
        node->SetChildNodes(std::move(newChildren));
    }
    else
    {
        for (auto child : node->GetChildNodeList())
        {
            SkipLodParametersFor37(child, child->GetType() == block_data::GroupLodParametersBlock10);
        }
    }
}

static void SearchFor37(NodePtr node)
{
    if (node->GetType() == block_data::GroupIndexAndTexturesBlock37)
    {
        SkipLodParametersFor37(node, false);
    }
    else
    {
        for (auto child : node->GetChildNodeList())
        {
            SearchFor37(child);
        }
    }
}

static void SkipLodParametersFor37(B3dTree& tree)
{
    for (const auto& node : tree.rootNodes)
    {
        SearchFor37(node);
    }
}

////////////////////////////////////////////////////////////////////////////////

static NodeList OptimizeSequence5(NodePtr node)
{
    if (node->GetType() != block_data::GroupObjectsBlock5)
    {
        NodeList res;
        res.push_back(node);
        return res;
    }

    return node->GetChildNodeList();
}

static NodeList OptimizeSequence10_5(NodePtr node)
{
    if (node->GetType() != block_data::GroupLodParametersBlock10)
    {
        NodeList res;
        res.push_back(node);
        return res;
    }

    NodeList res;
    for (auto child : node->GetChildNodeList())
    {
        NodeList tmp = OptimizeSequence5(child);
        res.insert(res.end(), tmp.begin(), tmp.end());
    }
    
    return res;
}

static void OptimizeSequence37_10_5(NodePtr node)
{
    if (node->GetType() == block_data::GroupIndexAndTexturesBlock37)
    {
        NodeList newChildren;
        for (const auto child : node->GetChildNodeList())
        {
            auto newChild = OptimizeSequence10_5(child);
            newChildren.insert(newChildren.end(), newChild.begin(), newChild.end());
        }

        node->SetChildNodes(std::move(newChildren));
    }
    else
    {
        for (const auto child : node->GetChildNodeList())
        {
            OptimizeSequence37_10_5(child);
        }
    }
}


static void OptimizeSequence37_10_5(B3dTree& tree)
{
    for (const auto& node : tree.rootNodes)
    {
        OptimizeSequence37_10_5(node);
    }
}

////////////////////////////////////////////////////////////////////////////////

static size_t GetLodCount(NodePtr node)
{
    size_t count = 0;
    if (node->GetType() == block_data::GroupLodParametersBlock10)
    {
        count += 1;
    }

    for (auto child : node->GetChildNodeList())
    {
        count += GetLodCount(child);
    }

    return count;
}

static void UseHalfChildFromSingleLod(NodePtr node)
{
    if (node->GetType() == block_data::GroupLodParametersBlock10)
    {
        if (GetLodCount(node) == 1)
        {
            NodeList newChildren = node->GetChildNodeList();
            if (std::all_of(newChildren.begin(), newChildren.end(), predicates::IsVertexNode) && (newChildren.size() >= 2))
            {
                assert((newChildren.size() % 2) == 0);
                newChildren.resize(newChildren.size() / 2);
                node->SetChildNodes(std::move(newChildren));
                D2_HACK_LOG(UseHalfChildFromSingleLod) << "skip second half of child nodes for " << node->GetName();
            }
        }
    }
    else
    {
        for (auto child : node->GetChildNodeList())
        {
            UseHalfChildFromSingleLod(child);
        }
    }
}

static void UseHalfChildFromSingleLod(B3dTree& tree)
{
    for (const auto& node : tree.rootNodes)
    {
        UseHalfChildFromSingleLod(node);
    }
}

////////////////////////////////////////////////////////////////////////////////

static void UseFirstLod(NodePtr node, bool insideLod)
{
    if (node->GetType() == block_data::GroupLodParametersBlock10)
    {
        insideLod = true;
    }

    NodeList newChildren;
    for (auto child : node->GetChildNodeList())
    {
        if ((child->GetType() == block_data::GroupLodParametersBlock10) && insideLod)
        {
            D2_HACK_LOG(UseFirstLod) << "skipping LOD entry: " << child->GetName();
            continue;
        }

        newChildren.push_back(child);

        UseFirstLod(child, insideLod);
    }

    node->SetChildNodes(std::move(newChildren));
}

static void UseFirstLod(B3dTree& tree)
{
    for (const auto& node : tree.rootNodes)
    {
        UseFirstLod(node, false);
    }
}

////////////////////////////////////////////////////////////////////////////////

template <typename Item>
Item DoMerge(const Item& item, const Item& parentItem, const common::IndexList& indices)
{
    if (!item.empty())
    {
        return item;
    }

    if (!parentItem.empty())
    {
        assert(parentItem.size() == indices.size());

        Item res;
        for (auto index : indices)
        {
            res.push_back(parentItem[index]);
        }

        return res;
    }

    return item;
}

static void MergeAndOptimizeMeshInfo(const common::SimpleMeshInfo& parentMeshInfo, common::SimpleMeshInfo& meshInfo)
{
    assert(!meshInfo.indices.empty());

    common::SimpleMeshInfo newMeshInfo;
    newMeshInfo.positions = DoMerge(meshInfo.positions, parentMeshInfo.positions, meshInfo.indices);
    newMeshInfo.texCoords = DoMerge(meshInfo.texCoords, parentMeshInfo.texCoords, meshInfo.indices);
    newMeshInfo.normals = DoMerge(meshInfo.normals, parentMeshInfo.normals, meshInfo.indices);

    for (std::uint32_t i = 0; i != meshInfo.indices.size(); ++i)
    {
        newMeshInfo.indices.push_back(i);
    }

    meshInfo = std::move(newMeshInfo);
}


template <typename Item>
void MergeItem(Item& dest, const Item& src)
{
    if (dest.empty())
    {
        dest = src;
        return;
    }

    assert(!src.empty());
    dest.insert(dest.end(), src.begin(), src.end());
}

template <typename FaceType>
static std::vector<FaceType> MergeMeshInfoListForMaterial(const std::vector<common::SimpleMeshInfo>& data, std::uint32_t materialIndex)
{
    std::map<size_t, std::vector<common::SimpleMeshInfo>> meshInfoMapping;

    for (const auto& item: data)
    {
        size_t id =
            (item.positions.empty() ? 0 : 1) +
            (item.texCoords.empty() ? 0 : 2) +
            (item.normals.empty() ? 0 : 4) +
            (item.indices.empty() ? 0 : 8);

        meshInfoMapping[id].push_back(item);
    }

    std::vector<FaceType> res;
    for (auto entry : meshInfoMapping)
    {
        FaceType face;
        face.materialIndex = materialIndex;
        face.meshInfo.indices = common::IndexList{};

        for (const auto meshInfo : entry.second)
        {
            MergeItem(face.meshInfo.positions, meshInfo.positions);
            MergeItem(face.meshInfo.texCoords, meshInfo.texCoords);
            MergeItem(face.meshInfo.normals, meshInfo.normals);

            std::uint32_t prevIndexSize = static_cast<std::uint32_t>(face.meshInfo.indices.size());
            assert(prevIndexSize == face.meshInfo.indices.size());

            for (auto index : meshInfo.indices)
            {
                face.meshInfo.indices.push_back(index + prevIndexSize);
            }
        }
        res.push_back(face);
    }

    return res;
}

template <typename FacesVector>
void MergeFacesWithSameMaterial(FacesVector& faces)
{
    std::map<std::uint32_t, std::vector<common::SimpleMeshInfo>> faceMapping;
    for (const auto& face : faces)
    {
        faceMapping[face.materialIndex].push_back(face.meshInfo);
    }

    FacesVector tmp;
    for (auto faceEntry : faceMapping)
    {
        auto merged = MergeMeshInfoListForMaterial<FacesVector::value_type>(faceEntry.second, faceEntry.first);
        tmp.insert(tmp.end(), merged.begin(), merged.end());
    }

    faces = std::move(tmp);
}

template <typename BlockType>
void DoMerge(BlockType& blockData, const common::SimpleMeshInfo* parentMeshInfo)
{
    assert(parentMeshInfo);

    for (auto& mesh : blockData.faces)
    {
        mesh.meshInfo.indices = PrepareIndices(blockData, mesh);
        MergeAndOptimizeMeshInfo(*parentMeshInfo, mesh.meshInfo);
    }

    MergeFacesWithSameMaterial(blockData.faces);
}

static void MergeFacesWithVertices(NodePtr node, const common::SimpleMeshInfo* parentMeshInfo)
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
    FilterUnusedNodes(tree);
    SkipLodParametersFor37(tree);
    OptimizeSequence37_10_5(tree);
    UseHalfChildFromSingleLod(tree);
    UseFirstLod(tree);
    MergeFacesWithVertices(tree);
    RemoveEmptyNodes(tree);
}

} // namespace optimization
} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack
