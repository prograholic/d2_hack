#include "analyze.h"

#include <iostream>

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


void PrintOnlyTypes(const B3dNodePtr& node, const std::vector<std::uint32_t>& sequence, std::set< std::vector<std::uint32_t>>& uniqueSequences, int indent = 0)
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

        PrintOnlyTypes(std::static_pointer_cast<B3dNode>(child), newSequence, uniqueSequences, indent + 1);
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

void PrintOnlyTypes(const B3dTree& tree, std::set< std::vector<std::uint32_t>>& sequences)
{
    std::cout << "Processing " << tree.dir << "/" << tree.id << "..." << std::endl;
    for (auto node : tree.rootNodes)
    {
        PrintOnlyTypes(node, { node->GetType() }, sequences);
    }
}

void PrintOnlyTypes(const B3dForest& forest)
{
    std::set< std::vector<std::uint32_t>> sequences;

    //PrintOnlyTypes(*forest.common, sequences);
    for (const auto& tree : forest.forest)
    {
        PrintOnlyTypes(*tree, sequences);
    }

    PrintOnlyTypes(*forest.common, sequences);
    PrintOnlyTypes(*forest.trucks, sequences);

    PrintSequences(sequences);

    PrintFirstElemsOfSequences(sequences);
}

} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack


using namespace d2_hack::resource::data::b3d;





boost::program_options::options_description get_analyze_options()
{
    boost::program_options::options_description analyze_options("Analysis options");
    analyze_options.add_options()
        (options::analysis::collect_entries_list_patterns, "Collect entries list patterns");

    return analyze_options;
}






int analyze(const d2_hack::resource::data::b3d::B3dForest& forest, const boost::program_options::variables_map& options)
{
    const bool collect_entries_list_patterns = (options.count(options::analysis::collect_entries_list_patterns) > 0);

    if (collect_entries_list_patterns)
    {
        PrintOnlyTypes(forest);
    }

    return 0;
}