#include "analyze.h"

#include <iostream>
#include <set>
#include <map>

#include <d2_hack/resource/data/b3d_visitor.h>
#include <d2_hack/common/utils.h>


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

std::ostream& GetStream(int indent = 0, std::ostream& ostream = std::cout)
{
    ostream << GetOffsetString(indent);
    return ostream;
}

void PrintSequence(const std::vector<std::uint32_t>& sequence)
{
    if (!sequence.empty())
    {
        auto it = sequence.begin();
        std::cout << *it;
        ++it;
        while (it != sequence.end())
        {
            std::cout << " -> " << *it;
            ++it;
        }
        std::cout << std::endl;
    }
}


void CollectEntriesListPatterns(const B3dNodePtr& node, const std::vector<std::uint32_t>& sequence, std::set< std::vector<std::uint32_t>>& uniqueSequences, int indent = 0)
{
    GetStream(indent) << node->GetTypeName() << "(" << node->GetName() << ")";
    if (!node->GetChildNodeList().empty())
    {
        std::cout << std::endl;
    }

    for (auto child : node->GetChildNodeList())
    {
        std::vector<std::uint32_t> newSequence = sequence;
        newSequence.push_back(child->GetType());

        CollectEntriesListPatterns(std::static_pointer_cast<B3dNode>(child), newSequence, uniqueSequences, indent + 1);
    }

    if (node->GetChildNodeList().empty())
    {
        uniqueSequences.insert(sequence);
        PrintSequence(sequence);
    }
}


void PrintSequences(const std::set< std::vector<std::uint32_t>>& sequences)
{
    for (const auto& seq : sequences)
    {
        PrintSequence(seq);
    }
}

void PrintFirstElemsOfSequences(const std::set< std::vector<std::uint32_t>>& sequences)
{
    std::set<std::uint32_t> roots;
    for (const auto& seq : sequences)
    {
        if (!seq.empty())
        {
            roots.insert(seq[0]);
        }
    }

    std::cout << "Print root types: ";
    for (auto root : roots)
    {
        std::cout << root << ", ";
    }

    std::cout << std::endl;
}

void CollectEntriesListPatterns(const B3dTree& tree, std::set< std::vector<std::uint32_t>>& sequences)
{
    std::cout << "Processing " << tree.dir << "/" << tree.id << "..." << std::endl;
    for (auto node : tree.rootNodes)
    {
        CollectEntriesListPatterns(node, { node->GetType() }, sequences);
    }
}

void CollectEntriesListPatterns(const B3dForest& forest)
{
    std::set< std::vector<std::uint32_t>> sequences;

    for (const auto& tree : forest.forest)
    {
        CollectEntriesListPatterns(*tree, sequences);
    }

    CollectEntriesListPatterns(*forest.common, sequences);
    CollectEntriesListPatterns(*forest.trucks, sequences);

    PrintSequences(sequences);

    PrintFirstElemsOfSequences(sequences);
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
                m_data.second.insert(blockData.type);
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


template <typename T, typename P, typename A>
void PrintData(const std::set<T, P, A>& data, std::ostream& stream)
{
    stream << "{";
    if (!data.empty())
    {
        auto pos = data.begin();
        stream << *pos;
        ++pos;

        for (; pos != data.end(); ++pos)
        {
            stream << ", " << *pos;
        }
    }
    stream << "}";
}

void PrintData(const TreeGeneratorMaterialsEntry& data, int indent, std::ostream& stream)
{
    auto& subStream = GetStream(indent, stream) << "[";

    PrintData(data.first, subStream);
    subStream << ", ";
    PrintData(data.second, subStream);
    subStream << "]" << std::endl;
}

void PrintData(const TreeGeneratorMaterialsData& data, int indent, std::ostream& stream)
{
    for (const auto& entry : data)
    {
        GetStream(indent, stream) << entry.first << ":" << std::endl;
        GetStream(indent, stream) << "{" << std::endl;
        PrintData(entry.second, indent + 1, stream);
        GetStream(indent, stream) << "}" << std::endl;
    }
}

template <typename K, typename V, typename P, typename A>
void PrintData(const std::map<K, V, P, A>& data, int indent, std::ostream& stream)
{
    for (const auto& entry : data)
    {
        auto& subStream = GetStream(indent, stream);
        subStream << entry.first << ": ";
        PrintData(entry.second, subStream);
        subStream << std::endl;
    }
}

void MatchTreeGeneratorMaterials(const B3dForest& forest)
{
    TreeGeneratorMaterialsData data;
    MatchedTreeMaterials1 matchedMaterials1;
    MatchedTreeMaterials2 matchedMaterials2;

    CollectTreeGeneratorMaterials(forest, data);
    MatchTreeGeneratorMaterials(data, matchedMaterials1);
    MatchTreeGeneratorMaterials(data, matchedMaterials2);

    PrintData(data, 0, std::cout);
    PrintData(matchedMaterials1, 0, std::cout);
    PrintData(matchedMaterials2, 0, std::cout);
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

} // namespace analysis
} // namespace options




boost::program_options::options_description get_analyze_options()
{
    boost::program_options::options_description analyze_options("Analysis options");
    analyze_options.add_options()
        (options::analysis::collect_entries_list_patterns, "Collect entries list patterns")
        (options::analysis::match_tree_generator_materials, "Match tree generator materials and parameters");

    return analyze_options;
}






int analyze(const d2_hack::resource::data::b3d::B3dForest& forest, const boost::program_options::variables_map& options)
{
    const bool collect_entries_list_patterns = (options.count(options::analysis::collect_entries_list_patterns) > 0);
    const bool match_tree_generator_materials = (options.count(options::analysis::match_tree_generator_materials) > 0);

    if (collect_entries_list_patterns)
    {
        CollectEntriesListPatterns(forest);
    }
    else if (match_tree_generator_materials)
    {
        MatchTreeGeneratorMaterials(forest);
    }
    else
    {
        std::cerr << "No analysis option was given" << std::endl;
        return 1;
    }

    return 0;
}