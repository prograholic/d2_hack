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

static bool IsVertexNode(NodePtr node)
{
    static const std::uint32_t vertexNodeTypes[] =
    {
        block_data::GroupVertexDataBlock7,
        block_data::GroupVertexDataBlock36,
        block_data::GroupVertexDataBlock37
    };

    return IsNodeOfType(node, std::begin(vertexNodeTypes), std::end(vertexNodeTypes));
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
    if (node->GetType() == block_data::GroupVertexDataBlock37)
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

 void SkipLodParametersFor37(B3dTree& tree)
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
    if (node->GetType() == block_data::GroupVertexDataBlock37)
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
                // TODO: ����������� ���������� ������� ��� LOD - 
                //assert((newChildren.size() % 2) == 0);
                //newChildren.resize(newChildren.size() / 2);
                //node->SetChildNodes(std::move(newChildren));
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

static void UseFirstLod(NodePtr node)
{
    if (node->GetType() == block_data::GroupLodParametersBlock10)
    {
        D2_HACK_LOG(UseFirstLod) << "Got LOD node: " << node->GetName();
        auto& childs = node->GetChildNodeList();
        auto pos = childs.begin();
        while (pos != childs.end())
        {
            D2_HACK_LOG(UseFirstLod) << "child node " << (*pos)->GetName() << ", with type: " << (*pos)->GetType();
            if ((*pos)->GetType() == block_data::HierarchyBreakerBlockXxx)
            {
                D2_HACK_LOG(UseFirstLod) << "  Got breaker node: " << (*pos)->GetName();
                break;
            }
            ++pos;
        }

        D2_HACK_LOG(UseFirstLod) << "before: " << childs.size();
        childs.erase(pos, childs.end());
        D2_HACK_LOG(UseFirstLod) << "after: " << childs.size();
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


static void SelectLodForPattern37_10_10(NodePtr node)
{
    if (node->GetType() == block_data::GroupVertexDataBlock37)
    {
        auto children = node->GetChildNodeList();
        if (children.size() == 1)
        {
            if (children[0]->GetType() == block_data::GroupLodParametersBlock10)
            {
                D2_HACK_LOG(SelectLodForPattern37_10_10) << "Found child Log for 37 block: " << node->GetName();

                auto childsLevel2 = children[0]->GetChildNodeList();
                if (!childsLevel2.empty())
                {
                    static const std::uint32_t lod_nodes[] = { block_data::GroupLodParametersBlock10 };
                    if (predicates::IsAllNodesOfType(childsLevel2.begin(), childsLevel2.end(), std::begin(lod_nodes), std::end(lod_nodes)))
                    {
                        NodeGroupLodParameters10* closestLod = childsLevel2[0]->NodeCast<NodeGroupLodParameters10>();
                        for (auto lod2 : childsLevel2)
                        {
                            NodeGroupLodParameters10* typedNode = lod2->NodeCast<NodeGroupLodParameters10>();
                            if (typedNode->GetBlockData().distanceToPlayer < closestLod->GetBlockData().distanceToPlayer)
                            {
                                closestLod = typedNode;
                            }
                        }

                        NodeList newChildren;
                        for (auto face : closestLod->GetChildNodeList())
                        {
                            if (face->GetType() == block_data::SimpleFacesBlock8)
                            {
                                D2_HACK_LOG(SelectLodForPattern37_10_10) << "skipping Face8 node: " << face->GetName();
                                continue;
                            }
                            newChildren.push_back(face);
                        }
                        node->SetChildNodes(std::move(newChildren));
                        return;
                    }
                }
            }
        }

    }

    for (auto child : node->GetChildNodeList())
    {
        SelectLodForPattern37_10_10(child);
    }
}

/**
 * ������� ������� �� ���, ��� ���� � GroupVertexData37 ���� ���� �������� ���� ���� Lod,
 * � � ����, � ���� �������, ���� ������ Lod � ���-�� ��������, �� �������� Lod � �������� ������� ����������,
 * � ��������� ����������.
 * �����, ����� �������� Face8, ��� ��� ��� ���� Lod
 * 
 * ������ ��������:
 * GroupVertexData37(AnmObj0203)
 *   GroupLodParameters10(176)
 *       GroupLodParameters10(177)
 *           SimpleFaces35(178)37 -> 10 -> 10 -> 35
 *           SimpleFaces35(179)37 -> 10 -> 10 -> 35
 *           SimpleFaces8(180)37 -> 10 -> 10 -> 8
 *       GroupLodParameters10(181)
 *           SimpleFaces8(182)37 -> 10 -> 10 -> 8
 *           SimpleFaces8(183)37 -> 10 -> 10 -> 8
 * 
 * ��������� �� ad_AnmObj0203_scene_node
 */
static void SelectLodForPattern37_10_10(B3dTree& tree)
{
    for (const auto& node : tree.rootNodes)
    {
        SelectLodForPattern37_10_10(node);
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

static void ProcessObjectConnectors(const B3dTree& tree, const B3dTree& common, const NodePtr& node)
{
    if (node->GetType() == block_data::SimpleObjectConnectorBlock18)
    {
        NodeSimpleObjectConnector18* typedNode = node->NodeCast<NodeSimpleObjectConnector18>();
        auto nodeName = common::ResourceNameToString(typedNode->GetBlockData().object);
        NodePtr newChildNode = GetTopLevelNodeByName(tree.rootNodes, nodeName);
        if (!newChildNode)
        {
            D2_HACK_LOG(ProcessObjectConnectors) << "cannot find node by name \"" + nodeName + "\", trying to find in common...";
            newChildNode = GetTopLevelNodeByName(common.rootNodes, nodeName);
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
            ProcessObjectConnectors(tree, common, child);
        }
    }
}

static void ProcessObjectConnectors(const B3dTree& common, const B3dTree& tree)
{
    for (const auto& node : tree.rootNodes)
    {
        ProcessObjectConnectors(tree, common, node);
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
             * �� ������� ������ ��� ���� �� �����,
             * ��� ������ ���� ���������� � �����. �������� � ������� SimpleObjectConnector18
             * � ������� ProcessObjectConnectors
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
    ProcessObjectConnectors(*forest.common, *forest.common);

    for (auto& tree : forest.forest)
    {
        MergeFacesWithVertices(*tree);
        ProcessObjectConnectors(*forest.common, *tree);
    }
}


void Optimize(B3dForest& forest)
{
    // TODO: ����� �� �������������� common? ������� ��� ���, ��� ��� �� ����� ������������� �� �� common ��������� ���� � ������ �������.
    for (auto& tree : forest.forest)
    {
        SkipTopLevelNodes(*tree);
        FilterUnusedNodes(*tree);
        UseFirstLod(*tree);
        MergeFacesWithSameMaterial(*tree);
        RemoveEmptyNodes(*tree);


        if (!tree) //always false at runtime, remove in prod
        {
            // TODO: ���������� ����������� � LOD-���, �������, ��� ��� ����������� � LOD-��� ������������.
            SkipLodParametersFor37(*tree);
            OptimizeSequence37_10_5(*tree);
            UseHalfChildFromSingleLod(*tree);
            //UseFirstLod(*tree);
            SelectLodForPattern37_10_10(*tree);
        }
    }
}

} // namespace transformation
} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack
