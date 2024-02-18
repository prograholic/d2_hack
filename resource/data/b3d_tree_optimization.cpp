#include <d2_hack/resource/data/b3d_tree_optimization.h>

#include <OgreException.h>

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
namespace transformation
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
        block_data::SimpleTriggerBlock13,
        block_data::SimpleFlatCollisionBlock20,
        block_data::SimpleVolumeCollisionBlock23
    };

    return IsNodeOfType(node, std::begin(unusedNodeTypes), std::end(unusedNodeTypes));
}

static bool IsUnusedTopLevelNode(NodePtr node)
{
    static const std::uint32_t unusedTopLevelNodeTypes[] =
    {
        block_data::GroupRoadInfraObjectsBlock4,
        block_data::GroupObjectsBlock5,
        block_data::HierarchyBreakerBlockXxx
    };

    return IsNodeOfType(node, std::begin(unusedTopLevelNodeTypes), std::end(unusedTopLevelNodeTypes));
}

template <typename Iterator>
static bool IsAllNodesOfType(Iterator first, Iterator last, const std::uint32_t* firstType, const std::uint32_t* lastType)
{
    while (first != last)
    {
        if (!IsNodeOfType(*first, firstType, lastType))
        {
            return false;
        }

        ++first;
    }

    return true;
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

static void UseFirstLod(NodePtr node)
{
    if (node->GetType() == block_data::GroupLodParametersBlock10)
    {
        D2_HACK_LOG(UseFirstLod) << "Got LOD node: " << node->GetName();
        auto& childs = node->GetChildNodeList();
        auto pos = childs.begin();
        while (pos != childs.end())
        {
            if ((*pos)->GetType() == block_data::HierarchyBreakerBlockXxx)
            {
                D2_HACK_LOG(UseFirstLod) << "  Got breaker node: " << (*pos)->GetName();
                break;
            }
            ++pos;
        }

        childs.erase(pos, childs.end());
    }

    for (auto child : node->GetChildNodeList())
    {
        UseFirstLod(child);
    }
}

static void UseFirstLod(B3dTree& tree)
{
    for (const auto& node : tree.rootNodes)
    {
        UseFirstLod(node);
    }
}

////////////////////////////////////////////////////////////////////////////////

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

template <typename FacesVector>
static FacesVector MergeMeshInfoListForMaterial(const std::vector<common::SimpleMeshInfo>& data, std::uint32_t materialIndex)
{
    std::map<size_t, std::vector<common::SimpleMeshInfo>> meshInfoForMerging;

    for (const auto& item: data)
    {
        size_t id =
            (item.positions.empty() ? 0 : 1) +
            (item.texCoords.empty() ? 0 : 2) +
            (item.normals.empty() ? 0 : 4);

        meshInfoForMerging[id].push_back(item);
    }

    FacesVector res;
    for (auto entry : meshInfoForMerging)
    {
        FacesVector::value_type face;
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


template <typename Visitor>
void VisitFaces(NodePtr node, const common::SimpleMeshInfo* parentMeshInfo)
{
    if (node->GetType() == block_data::GroupVertexDataBlock7)
    {
        NodeGroupVertexData7* typedNode = node->NodeCast<NodeGroupVertexData7>();

        parentMeshInfo = std::addressof(typedNode->GetBlockData().meshInfo);
    }
    else if (node->GetType() == block_data::GroupVertexDataBlock36)
    {
        NodeGroupVertexData36* typedNode = node->NodeCast<NodeGroupVertexData36>();

        parentMeshInfo = std::addressof(typedNode->GetBlockData().meshInfo);
    }
    else if (node->GetType() == block_data::GroupVertexDataBlock37)
    {
        NodeGroupVertexData37* typedNode = node->NodeCast<NodeGroupVertexData37>();

        parentMeshInfo = std::addressof(typedNode->GetBlockData().meshInfo);
    }
    else if (node->GetType() == block_data::SimpleFacesBlock8)
    {
        NodeSimpleFaces8* typedNode = node->NodeCast<NodeSimpleFaces8>();
        Visitor::Visit(typedNode->GetBlockData(), parentMeshInfo);
    }
    else if (node->GetType() == block_data::SimpleFacesBlock28)
    {
        NodeSimpleFaces28* typedNode = node->NodeCast<NodeSimpleFaces28>();
        Visitor::Visit(typedNode->GetBlockData(), parentMeshInfo);
    }
    else if (node->GetType() == block_data::SimpleFacesBlock35)
    {
        NodeSimpleFaces35* typedNode = node->NodeCast<NodeSimpleFaces35>();
        Visitor::Visit(typedNode->GetBlockData(), parentMeshInfo);
    }

    for (const auto& child : node->GetChildNodeList())
    {
        VisitFaces<Visitor>(child, parentMeshInfo);
    }
}

struct PrepareStandaloneMeshInfoVisitor
{
    template <typename BlockType>
    static void Visit(BlockType& blockData, const common::SimpleMeshInfo* parentMeshInfo)
    {
        assert(parentMeshInfo);
        assert(parentMeshInfo->indices.empty());

        for (auto& mesh : blockData.faces)
        {
            mesh.meshInfo = PrepareStandaloneMeshInfo(blockData, mesh, *parentMeshInfo);
        }
    }

};

static void MergeFacesWithVertices(const B3dTree& tree)
{
    for (const auto& node : tree.rootNodes)
    {
        VisitFaces<PrepareStandaloneMeshInfoVisitor>(node, nullptr);
    }
}

struct MergeFacesWithSameMaterialVisitor
{
    template <typename BlockType>
    static void Visit(BlockType& blockData, const common::SimpleMeshInfo* /* parentMeshInfo */)
    {
        std::map<std::uint32_t, std::vector<common::SimpleMeshInfo>> faceMapping;

        for (const auto& face : blockData.faces)
        {
            faceMapping[face.materialIndex].push_back(face.meshInfo);
        }

        decltype(blockData.faces) tmp;
        for (auto faceEntry : faceMapping)
        {
            auto merged = MergeMeshInfoListForMaterial<decltype(blockData.faces)>(faceEntry.second, faceEntry.first);
            tmp.insert(tmp.end(), merged.begin(), merged.end());
        }

        blockData.faces = std::move(tmp);
    }

};

static void MergeFacesWithSameMaterial(const B3dTree& tree)
{
    for (const auto& node : tree.rootNodes)
    {
        VisitFaces<MergeFacesWithSameMaterialVisitor>(node, nullptr);
    }
}

////////////////////////////////////////////////////////////////////////////////

static bool RemoveEmptyNodes(const NodePtr& node)
{
    const auto& children = node->GetChildNodeList();

    NodeList newChildList;
    for (auto child : children)
    {
        bool needToRemove = RemoveEmptyNodes(child);
        if (!needToRemove)
        {
            newChildList.push_back(child);
        }
        else
        {
            D2_HACK_LOG(RemoveEmptyNodes) << "removed node: " << child->GetName() << " with type: " << child->GetType();
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
        RemoveEmptyNodes(node);
    }
}

static NodePtr GetTopLevelNodeByName(const NodeList& nodes, const std::string &name)
{
    for (const auto& node : nodes)
    {
        if (node->GetName() == name)
        {
            return node;
        }
    }

    return NodePtr{};
}

static void ProcessObjectConnectors(const B3dTree& tree, const B3dTree& common, const B3dTree& trucks, const NodePtr& node)
{
    if (node->GetType() == block_data::SimpleObjectConnectorBlock18)
    {
        NodeSimpleObjectConnector18* typedNode = node->NodeCast<NodeSimpleObjectConnector18>();
        auto nodeName = common::ResourceNameToString(typedNode->GetBlockData().object);
        NodePtr newChildNode = GetTopLevelNodeByName(tree.rootNodes, nodeName);
        if (!newChildNode)
        {
            D2_HACK_LOG(ProcessObjectConnectors) << "cannot find node by name \"" + nodeName + "\", trying to find in `common`...";
            newChildNode = GetTopLevelNodeByName(common.rootNodes, nodeName);
        }
        if (!newChildNode)
        {
            D2_HACK_LOG(ProcessObjectConnectors) << "cannot find node by name \"" + nodeName + "\", trying to find in `trucks`...";
            newChildNode = GetTopLevelNodeByName(trucks.rootNodes, nodeName);
        }
        if (newChildNode)
        {
            NodeList newChildNodes;
            newChildNodes.push_back(newChildNode);
            typedNode->SetChildNodes(std::move(newChildNodes));
        }
        else
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "cannot find node by name \"" + nodeName + "\"");
        }
    }
    else
    {
        for (const auto& child : node->GetChildNodeList())
        {
            ProcessObjectConnectors(tree, common, trucks, child);
        }
    }
}

static void ProcessObjectConnectors(const B3dTree& common, const B3dTree& trucks, const B3dTree& tree)
{
    for (const auto& node : tree.rootNodes)
    {
        ProcessObjectConnectors(tree, common, trucks, node);
    }
}

static void SkipTopLevelNodes(B3dTree& tree)
{
    NodeList newRoots;
    for (const auto& node : tree.rootNodes)
    {
        if (predicates::IsUnusedTopLevelNode(node))
        {
            /**
             * На верхнем уровне эти узлы не нужны,
             * они должны быть прицеплены к соотв. объектам с помощью SimpleObjectConnector18
             * с помощью ProcessObjectConnectors
             */
            continue;
        }

        newRoots.push_back(node);
    }

    std::swap(tree.rootNodes, newRoots);
}

////////////////////////////////////////////////////////////////////////////////

void Transform(B3dForest& forest)
{
    MergeFacesWithVertices(*forest.common);
    ProcessObjectConnectors(*forest.common, *forest.trucks, *forest.common);

    MergeFacesWithVertices(*forest.trucks);
    ProcessObjectConnectors(*forest.common, *forest.trucks, *forest.trucks);

    for (auto& tree : forest.forest)
    {
        MergeFacesWithVertices(*tree);
        ProcessObjectConnectors(*forest.common, *forest.trucks, *tree);
    }
}


static void Optimize(B3dTree& tree)
{
    SkipTopLevelNodes(tree);
    FilterUnusedNodes(tree);
    UseFirstLod(tree);
    MergeFacesWithSameMaterial(tree);
    RemoveEmptyNodes(tree);
}

void Optimize(B3dForest& forest)
{
    // TODO: нужно ли оптимизировать `common` и `trucks`? Кажется что нет, так как на этапе трансформации мы из common надергаем узлы в другие деревья.
    for (auto& tree : forest.forest)
    {
        Optimize(*tree);
    }
}

} // namespace transformation
} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack
