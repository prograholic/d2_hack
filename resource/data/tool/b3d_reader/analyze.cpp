#include "analyze.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <set>
#include <map>
#include <format>

#include <d2_hack/resource/data/b3d_visitor.h>
#include <d2_hack/common/utils.h>

#include "options.h"

namespace d2_hack
{
namespace resource
{
namespace data
{
namespace b3d
{


std::string GetOffsetString(int indent)
{
    std::string res;
    res.resize(indent * 4, ' ');

    return res;
}

std::ostream& GetStream(int indent, std::ostream& stream)
{
    stream << GetOffsetString(indent);
    return stream;
}

void PrintSequence(const std::vector<std::uint32_t>& sequence, std::ostream& stream)
{
    if (!sequence.empty())
    {
        auto it = sequence.begin();
        stream << *it;
        ++it;
        while (it != sequence.end())
        {
            stream << " -> " << *it;
            ++it;
        }
        stream << std::endl;
    }
}


void CollectEntriesListPatterns(const B3dNodePtr& node, const std::vector<std::uint32_t>& sequence, std::set< std::vector<std::uint32_t>>& uniqueSequences, int indent, std::ostream& stream)
{
    GetStream(indent, stream) << node->GetTypeName() << "(" << node->GetName() << ")";
    if (!node->GetChildNodeList().empty())
    {
        stream << std::endl;
    }

    for (auto child : node->GetChildNodeList())
    {
        std::vector<std::uint32_t> newSequence = sequence;
        newSequence.push_back(child->GetType());

        CollectEntriesListPatterns(std::static_pointer_cast<B3dNode>(child), newSequence, uniqueSequences, indent + 1, stream);
    }

    if (node->GetChildNodeList().empty())
    {
        uniqueSequences.insert(sequence);
        PrintSequence(sequence, stream);
    }
}


void PrintSequences(const std::set< std::vector<std::uint32_t>>& sequences, std::ostream& stream)
{
    for (const auto& seq : sequences)
    {
        PrintSequence(seq, stream);
    }
}

void PrintFirstElemsOfSequences(const std::set< std::vector<std::uint32_t>>& sequences, std::ostream& stream)
{
    std::set<std::uint32_t> roots;
    for (const auto& seq : sequences)
    {
        if (!seq.empty())
        {
            roots.insert(seq[0]);
        }
    }

    stream << "Print root types: ";
    for (auto root : roots)
    {
        stream << root << ", ";
    }

    stream << std::endl;
}

void CollectEntriesListPatterns(const B3dTree& tree, std::set< std::vector<std::uint32_t>>& sequences, std::ostream& stream)
{
    std::cout << "Processing " << tree.dir << "/" << tree.id << "..." << std::endl;
    for (auto node : tree.rootNodes)
    {
        CollectEntriesListPatterns(node, { node->GetType() }, sequences, 0, stream);
    }
}

void CollectEntriesListPatterns(const B3dForest& forest, std::ostream& stream)
{
    std::set< std::vector<std::uint32_t>> sequences;

    for (const auto& tree : forest.forest)
    {
        CollectEntriesListPatterns(*tree, sequences, stream);
    }

    CollectEntriesListPatterns(*forest.common, sequences, stream);
    CollectEntriesListPatterns(*forest.trucks, sequences, stream);

    PrintSequences(sequences, stream);

    PrintFirstElemsOfSequences(sequences, stream);
}

////////////////////////////////////////////////////////////////

// [{tree materials}, { tree generator params }]
typedef
std::pair<
    std::set<std::string>, // {tree materials}
    std::set<std::uint32_t> // {tree generator params}
> TreeGeneratorMaterialsEntry;

// ID -> [{tree materials}, {tree generator params}]
typedef std::map<std::string, TreeGeneratorMaterialsEntry> TreeGeneratorMaterialsData;

typedef std::map<std::uint32_t, std::set<std::string>> MatchedTreeMaterials1;
typedef std::map<std::string, std::set<std::uint32_t>> MatchedTreeMaterials2;

class MatchTreeGeneratorMaterialsVisitor : public NoOpNodeVisitor
{
public:
    explicit MatchTreeGeneratorMaterialsVisitor(TreeGeneratorMaterialsEntry& data)
        : m_data(data)
    {
    }

    virtual VisitResult Visit(NodeSimpleGeneratedObjects40& node, VisitMode visitMode) override
    {
        if (visitMode == VisitMode::PreOrder)
        {
            const auto& blockData = node.GetBlockData();
            auto str = common::ResourceNameToStringView(blockData.name);
            if (str.find("$$TreeGenerator") != str.npos)
            {
                m_data.second.insert(blockData.unknown0.type);
            }
        }

        return VisitResult::Continue;
    }

private:
    TreeGeneratorMaterialsEntry& m_data;
};

void CollectTreeGeneratorMaterials(const B3dTree& tree, TreeGeneratorMaterialsData& data)
{
    std::cout << "Processing " << tree.dir << "/" << tree.id << "..." << std::endl;

    auto& subData = data[tree.id];
    MatchTreeGeneratorMaterialsVisitor visitor{subData};

    auto visitResult = VisitTree(tree, visitor);
    (void)visitResult;

    for (const auto& material : tree.materials)
    {
        auto str = common::ResourceNameToStringView(material);
        if (str.find("tree") != str.npos)
        {
            subData.first.insert(std::string{str});
        }
    }
}

void CollectTreeGeneratorMaterials(const B3dForest& forest, TreeGeneratorMaterialsData& data)
{
    for (const auto& tree : forest.forest)
    {
        CollectTreeGeneratorMaterials(*tree, data);
    }

    CollectTreeGeneratorMaterials(*forest.common, data);
    CollectTreeGeneratorMaterials(*forest.trucks, data);
}

void MatchTreeGeneratorMaterials(const TreeGeneratorMaterialsData& data, MatchedTreeMaterials1& matchedMaterials)
{
    for (const auto& entry : data)
    {
        const auto& materials = entry.second.first;
        const auto& types = entry.second.second;
        for (auto type: types)
        {
            auto pos = matchedMaterials.find(type);
            if (pos == matchedMaterials.end())
            {
                matchedMaterials[type] = materials;
            }
            else
            {
                std::set<std::string> intersection;
                const auto& matched = matchedMaterials[type];
                std::set_intersection(
                    matched.begin(), matched.end(),
                    materials.begin(), materials.end(),
                    std::inserter(intersection, intersection.begin())
                );
                matchedMaterials[type] = intersection;
            }
        }
    }
}

void MatchTreeGeneratorMaterials(const TreeGeneratorMaterialsData& data, MatchedTreeMaterials2& matchedMaterials)
{
    for (const auto& entry : data)
    {
        const auto& materials = entry.second.first;
        const auto& types = entry.second.second;
        for (auto material : materials)
        {
            auto pos = matchedMaterials.find(material);
            if (pos == matchedMaterials.end())
            {
                matchedMaterials[material] = types;
            }
            else
            {
                std::set<std::uint32_t> intersection;
                const auto& matched = matchedMaterials[material];
                std::set_intersection(
                    matched.begin(), matched.end(),
                    types.begin(), types.end(),
                    std::inserter(intersection, intersection.begin())
                );
                matchedMaterials[material] = intersection;
            }
        }
    }
}


std::string ExtraPrintInfo(const std::string& /* data */, bool /* printExtraInfo */)
{
    return std::string{};
}

std::string ExtraPrintInfo(std::uint32_t data, bool printExtraInfo)
{
    return printExtraInfo ? ": " + std::format("{:#010x}", data) : "";
}

std::string ExtraPrintInfo(size_t data, bool printExtraInfo)
{
    return printExtraInfo ? ": " + std::format("{:#020x}", data) : "";
}


template <typename T, typename P, typename A>
void PrintData(const std::set<T, P, A>& data, int /* indent */, std::ostream& stream, bool printExtraInfo)
{
    stream << "{";
    if (!data.empty())
    {
        auto pos = data.begin();
        stream << "(" << *pos << ExtraPrintInfo(*pos, printExtraInfo) << ")";
        ++pos;

        for (; pos != data.end(); ++pos)
        {
            stream << ", (" << *pos << ExtraPrintInfo(*pos, printExtraInfo) << ")";
        }
    }
    stream << "}";
}

void PrintData(const TreeGeneratorMaterialsEntry& data, int indent, std::ostream& stream, bool printExtraInfo)
{
    auto& subStream = GetStream(indent, stream) << "[";

    PrintData(data.first, indent + 1, subStream, printExtraInfo);
    subStream << ", ";
    PrintData(data.second, indent + 1, subStream, printExtraInfo);
    subStream << "]" << std::endl;
}

void PrintData(const TreeGeneratorMaterialsData& data, int indent, std::ostream& stream, bool printExtraInfo)
{
    for (const auto& entry : data)
    {
        GetStream(indent, stream) << entry.first << ":" << std::endl;
        GetStream(indent, stream) << "{" << std::endl;
        PrintData(entry.second, indent + 1, stream, printExtraInfo);
        GetStream(indent, stream) << "}" << std::endl;
    }
}

template <typename K, typename V, typename P, typename A>
void PrintData(const std::map<K, V, P, A>& data, int indent, std::ostream& stream, bool printExtraInfo)
{
    for (const auto& entry : data)
    {
        auto& subStream = GetStream(indent, stream);
        subStream << "(" << entry.first << ExtraPrintInfo(entry.first, printExtraInfo) << ")" << ": ";
        PrintData(entry.second, indent + 1, subStream, printExtraInfo);
        subStream << std::endl;
    }
}

void MatchTreeGeneratorMaterials(const B3dForest& forest, std::ostream& stream, bool printExtraInfo = true)
{
    TreeGeneratorMaterialsData data;
    MatchedTreeMaterials1 matchedMaterials1;
    MatchedTreeMaterials2 matchedMaterials2;

    CollectTreeGeneratorMaterials(forest, data);
    MatchTreeGeneratorMaterials(data, matchedMaterials1);
    MatchTreeGeneratorMaterials(data, matchedMaterials2);

    PrintData(data, 0, stream, printExtraInfo);
    PrintData(matchedMaterials1, 0, stream, printExtraInfo);
    PrintData(matchedMaterials2, 0, stream, printExtraInfo);
}


typedef std::map<std::uint32_t, std::map < size_t, std::set<std::string>>> NodeChildInfo;

static void CountChildrenInNodes(common::NodeBase& node, NodeChildInfo& info)
{
    info[node.GetType()][node.GetChildNodeList().size()].insert(std::move(std::string{ node.GetName() }));

    for (auto& child : node.GetChildNodeList())
    {
        CountChildrenInNodes(*child, info);
    }
}

static void CountChildrenInNodes(const B3dTree& tree, NodeChildInfo& info)
{
    for (const auto& node : tree.rootNodes)
    {
        CountChildrenInNodes(*node, info);
    }
}

static void CountChildrenInNodes(const B3dForest& forest, std::ostream& stream, bool printExtraInfo = false)
{
    NodeChildInfo info;

    for (const auto& tree : forest.forest)
    {
        CountChildrenInNodes(*tree, info);
    }

    CountChildrenInNodes(*forest.common, info);
    CountChildrenInNodes(*forest.trucks, info);

    PrintData(info, 0, stream, printExtraInfo);
}

typedef std::map <std::uint32_t, std::set<std::uint32_t>> ChildTypesInNodes;

static void GetChildTypesInNodes(const B3dTree& tree, ChildTypesInNodes& info)
{
    for (auto& rootNode : tree.rootNodes)
    {
        rootNode->VisitPreOrder(
            [&info](auto node)
            {
                for (auto& child : node->GetChildNodeList())
                {
                    info[node->GetType()].insert(child->GetType());
                }
            }
        );
    }
}

static void GetChildTypesInNodes(const B3dForest& forest, std::ostream& stream, bool printExtraInfo = false)
{
    ChildTypesInNodes info;

    for (const auto& tree : forest.forest)
    {
        GetChildTypesInNodes(*tree, info);
    }

    GetChildTypesInNodes(*forest.common, info);
    GetChildTypesInNodes(*forest.trucks, info);

    PrintData(info, 0, stream, printExtraInfo);
}

} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack


using namespace d2_hack::resource::data::b3d;



namespace options
{
namespace analysis
{

static const char collect_entries_list_patterns[] = "collect_entries_list_patterns";
static const char match_tree_generator_materials[] = "match_tree_generator_materials";
static const char count_children_in_nodes[] = "count_children_in_nodes";
static const char get_child_types_in_nodes[] = "get_child_types_in_nodes";

} // namespace analysis
} // namespace options




boost::program_options::options_description get_analyze_options()
{
    boost::program_options::options_description analyze_options("Analysis options");
    analyze_options.add_options()
        (options::analysis::collect_entries_list_patterns, "Collect entries list patterns")
        (options::analysis::match_tree_generator_materials, "Match tree generator materials and parameters")
        (options::analysis::count_children_in_nodes, "Count children in nodes")
        (options::analysis::get_child_types_in_nodes, "Get child types in nodes");

    return analyze_options;
}



static std::filesystem::path ConstructOutputPathName(const std::string& operation, const boost::program_options::variables_map& vm)
{
    std::filesystem::path dir = vm[options::generic::output_dir].as<std::string>();
    std::filesystem::create_directories(dir);

    using namespace options::generic;

    std::string filename = operation;

    if (vm.contains(skip_transformation))
    {
        filename += "_notf";
    }
    if (vm.contains(skip_optimization))
    {
        filename += "_noopt";
    }

    return dir / (filename + ".txt");
}




int analyze(const d2_hack::resource::data::b3d::B3dForest& forest, const boost::program_options::variables_map& vm)
{
    const bool collect_entries_list_patterns = vm.contains(options::analysis::collect_entries_list_patterns);
    const bool match_tree_generator_materials = vm.contains(options::analysis::match_tree_generator_materials);
    const bool count_children_in_nodes = vm.contains(options::analysis::count_children_in_nodes);
    const bool get_child_types_in_nodes = vm.contains(options::analysis::get_child_types_in_nodes);

    if (collect_entries_list_patterns)
    {
        std::filesystem::path outputFile = ConstructOutputPathName(options::analysis::collect_entries_list_patterns, vm);
        std::ofstream stream{ outputFile };
        CollectEntriesListPatterns(forest, stream);
    }
    if (match_tree_generator_materials)
    {
        std::filesystem::path outputFile = ConstructOutputPathName(options::analysis::match_tree_generator_materials, vm);
        std::ofstream stream{ outputFile };
        MatchTreeGeneratorMaterials(forest, stream);
    }
    if (count_children_in_nodes)
    {
        std::filesystem::path outputFile = ConstructOutputPathName(options::analysis::count_children_in_nodes, vm);
        std::ofstream stream{ outputFile };
        CountChildrenInNodes(forest, stream);
    }
    if (get_child_types_in_nodes)
    {
        std::filesystem::path outputFile = ConstructOutputPathName(options::analysis::get_child_types_in_nodes, vm);
        std::ofstream stream{ outputFile };
        GetChildTypesInNodes(forest, stream);
    }

    return 0;
}