#include <d2_hack/resource/data/b3d_tree_optimization.h>

#include <OgreException.h>

#include <d2_hack/resource/data/b3d_utils.h>
#include <d2_hack/resource/data/b3d_visitor.h>

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

using namespace common;

namespace transformation
{

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
        typename FacesVector::value_type face;
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
void VisitFaces(B3dNodePtr node, const common::SimpleMeshInfo* parentMeshInfo)
{
    if (NodeGroupVertexData7* typedNode7 = node->TryNodeCast<NodeGroupVertexData7>())
    {
        parentMeshInfo = std::addressof(typedNode7->GetBlockData().meshInfo);
    }
    else if (NodeGroupVertexData36* typedNode36 = node->TryNodeCast<NodeGroupVertexData36>())
    {
        parentMeshInfo = std::addressof(typedNode36->GetBlockData().meshInfo);
    }
    else if (NodeGroupVertexData37* typedNode37 = node->TryNodeCast<NodeGroupVertexData37>())
    {
        parentMeshInfo = std::addressof(typedNode37->GetBlockData().meshInfo);
    }
    else if (NodeSimpleFaces8* typedNode8 = node->TryNodeCast<NodeSimpleFaces8>())
    {
        Visitor::Visit(typedNode8->GetBlockData(), parentMeshInfo);
    }
    else if (NodeSimpleFaces28* typedNode28 = node->TryNodeCast<NodeSimpleFaces28>())
    {
        Visitor::Visit(typedNode28->GetBlockData(), parentMeshInfo);
    }
    else if (NodeSimpleFaces35* typedNode35 = node->TryNodeCast<NodeSimpleFaces35>())
    {
        Visitor::Visit(typedNode35->GetBlockData(), parentMeshInfo);
    }

    for (const auto& child : node->GetChildNodeList())
    {
        VisitFaces<Visitor>(std::static_pointer_cast<B3dNode>(child), parentMeshInfo);
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

static void UseOnlyFirstLod(const B3dTree& tree)
{
    for (const auto& node : tree.rootNodes)
    {
        node->SimpleVisit(
            [](common::NodeBase* node)
            {
                if (NodeGroupLodParameters10* typedNode = node->TryNodeCast<NodeGroupLodParameters10>())
                {
                    auto& childs = typedNode->GetChildNodeList();
                    if (childs.size() == 2)
                    {
                        typedNode->GetChildNodeList().pop_back(); // simply drop all other LoDs
                    }
                }
            }
        );
    }
}

static void RemoveLodFromTree(const B3dTree& tree)
{
    for (const auto& node : tree.rootNodes)
    {
        node->SimpleVisit(
            [](common::NodeBase* node)
            {
                auto& childs = node->GetChildNodeList();
                size_t pos = 0;
                for ( ; pos != childs.size(); ++pos)
                {
                    auto& child = childs[pos];
                    if (child->GetType() == block_data::GroupLodParametersBlock10)
                    {
                        assert(child->GetChildNodeList().size() == 1);
                        assert(child->GetChildNodeList().front()->GetType() == block_data::EventEntryBlockXxx);

                        auto& subsubchilds = child->GetChildNodeList().front()->GetChildNodeList(); // got childs of EventEntry

                        for (auto& subsubchild : subsubchilds)
                        {
                            node->AddChildNode(subsubchild);
                        }

                        childs.erase(childs.begin() + pos);

                        break;
                    }
                }
            }
        );
    }
}

static void Remove7_37FromTree(const B3dTree& tree)
{
    static const std::uint32_t nodes7_37[] =
    {
        block_data::GroupVertexDataBlock7,
        block_data::GroupVertexDataBlock37
    };

    for (const auto& node : tree.rootNodes)
    {
        node->SimpleVisit(
            [](common::NodeBase* node)
            {
                auto& childs = node->GetChildNodeList();
                size_t pos = 0;
                for (; pos < childs.size(); ++pos)
                {
                    auto& child = childs[pos];
                    if (std::any_of(std::begin(nodes7_37), std::end(nodes7_37), [&child](std::uint32_t value) {return child->GetType() == value; }))
                    {
                        auto& subchilds = child->GetChildNodeList();

                        for (auto& subchild : subchilds)
                        {
                            node->AddChildNode(subchild);
                        }

                        childs.erase(childs.begin() + pos);

                        break;
                    }
                }
            }
        );
    }
}

////////////////////////////////////////////////////////////////////////////////

static B3dNodePtr GetTopLevelNodeByName(const B3dNodeList& nodes, const std::string &name)
{
    for (const auto& node : nodes)
    {
        if (node->GetName() == name)
        {
            return node;
        }
    }

    return B3dNodePtr{};
}

static B3dNodePtr FindTopLevelNodeByNameInTrees(const B3dTree& tree, const B3dTree& common, const B3dTree& trucks, const std::string& name)
{
    B3dNodePtr res = GetTopLevelNodeByName(tree.rootNodes, name);
    if (!res)
    {
        res = GetTopLevelNodeByName(common.rootNodes, name);
    }

    if (!res)
    {
        res = GetTopLevelNodeByName(trucks.rootNodes, name);
    }

    return res;
}

static void ProcessObjectConnectors(B3dTree& tree, const B3dTree& common, const B3dTree& trucks, const B3dNodePtr& node)
{
    if (node->GetType() == block_data::SimpleObjectConnectorBlock18)
    {
        NodeSimpleObjectConnector18* typedNode = node->NodeCast<NodeSimpleObjectConnector18>();
        auto nodeName = common::ResourceNameToString(typedNode->GetBlockData().object);
        if (!nodeName.empty())
        {
            NodePtr newChildNode = FindTopLevelNodeByNameInTrees(tree, common, trucks, nodeName);
            if (!newChildNode)
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "cannot find node by name \"" + nodeName + "\"");
            }

            NodeList newChildNodes;
            newChildNodes.push_back(newChildNode);
            typedNode->SetChildNodes(std::move(newChildNodes));

            typedNode->GetBlockData().object = common::ResourceName{}; // prevent duplicate processing
        }
    }
    else if (node->GetType() == block_data::GroupObjectsBlock5)
    {
        NodeGroupObjects5* typedNode = node->NodeCast<NodeGroupObjects5>();
        auto nodeName = common::ResourceNameToString(typedNode->GetBlockData().name);
        if (!nodeName.empty())
        {
            NodePtr newChildNode = FindTopLevelNodeByNameInTrees(tree, common, trucks, nodeName);
            if (!newChildNode)
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "cannot find node by name \"" + nodeName + "\"");
            }

            typedNode->AddChildNode(newChildNode);

            typedNode->GetBlockData().name = common::ResourceName{}; // prevent duplicate processing
        }
    }

    for (const auto& child : node->GetChildNodeList())
    {
        ProcessObjectConnectors(tree, common, trucks, std::static_pointer_cast<B3dNode>(child));
    }
}

static void ProcessObjectConnectors(const B3dTree& common, const B3dTree& trucks, B3dTree& tree)
{
    for (const auto& node : tree.rootNodes)
    {
        ProcessObjectConnectors(tree, common, trucks, node);
    }
}

static NodePtr CreateEventEntryNode(const B3dTreePtr& tree)
{
    return MakeVisitableNode(tree, NodePtr{}, MakeBlockHeader(common::ResourceName{}, block_data::EventEntryBlockXxx), block_data::EventEntry{});
}



static std::list<NodeList> SplitNodes(const NodeList& childs, bool preserveEmptyRanges)
{
    std::list<NodeList> res;

    NodeList tmp;

    for (const auto& child : childs)
    {
        if (child->GetType() != block_data::HierarchyBreakerBlockXxx)
        {
            tmp.push_back(child);
        }
        else
        {
            if (!tmp.empty() || preserveEmptyRanges)
            {
                res.emplace_back(std::move(tmp));
            }
        }
    }

    if (!tmp.empty() || preserveEmptyRanges)
    {
        res.emplace_back(std::move(tmp));
    }

    return res;
}

static void InjectEventNode(const B3dNodePtr& node)
{
    static const std::uint32_t nodesWithHierarchyBreaker[] =
    {
        block_data::GroupUnknownBlock2,
        block_data::GroupTriggerBlock9,
        block_data::GroupLodParametersBlock10,
        block_data::GroupObjectsBlock21,
        block_data::GroupUnknownBlock29,
    };

    if (std::any_of(std::begin(nodesWithHierarchyBreaker), std::end(nodesWithHierarchyBreaker), [node](std::uint32_t value) {return node->GetType() == value;}))
    {
        NodeList newChilds;
        auto categorizedNodes = SplitNodes(node->GetChildNodeList(), true);

        for (auto& category : categorizedNodes)
        {
            auto eventEntryNode = CreateEventEntryNode(node->GetOriginalRoot());
            eventEntryNode->SetChildNodes(std::move(category));
            newChilds.push_back(eventEntryNode);
        }

        node->SetChildNodes(std::move(newChilds));
    }


    for (const auto& child : node->GetChildNodeList())
    {
        InjectEventNode(std::static_pointer_cast<B3dNode>(child));
    }
}

static void InjectEventNode(B3dTree& tree)
{
    for (const auto& node : tree.rootNodes)
    {
        InjectEventNode(node);
    }
}


static void TransformTree(const B3dTree& common, const B3dTree& trucks, B3dTree& tree)
{
    InjectEventNode(tree);
    MergeFacesWithVertices(tree);
    ProcessObjectConnectors(common, trucks, tree);
}

////////////////////////////////////////////////////////////////////////////////

void Transform(B3dForest& forest)
{
    TransformTree(*forest.common, *forest.trucks, *forest.common);

    TransformTree(*forest.common, *forest.trucks, *forest.trucks);

    for (auto& tree : forest.forest)
    {
        TransformTree(*forest.common, *forest.trucks, *tree);
    }
}


static void Optimize(B3dTree& tree)
{
    MergeFacesWithSameMaterial(tree);
    UseOnlyFirstLod(tree);
    RemoveLodFromTree(tree);
    Remove7_37FromTree(tree);
}

void Optimize(B3dForest& forest)
{
    // TODO: нужно ли оптимизировать `common` и `trucks`?  ажетс€ что нет, так как на этапе трансформации мы из common надергаем узлы в другие деревь€.
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
