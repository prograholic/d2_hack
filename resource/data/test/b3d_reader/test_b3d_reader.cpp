#include <iostream>
#include <fstream>
#include <stdexcept>

#include <d2_hack/resource/data/b3d_reader.h>


using namespace d2_hack::resource::data::b3d;

void TestForestEntry(const B3dForest& forest, size_t pos, const std::string & dir, const std::string & id, size_t expectedMaterialCount)
{
    if (pos >= forest.size())
    {
        throw std::runtime_error("Incorrect forest size, expected at least " + std::to_string(pos) + ", got " + std::to_string(forest.size()));
    }

    const auto& tree = forest[pos];

    if (tree.registryEntry.dir != dir)
    {
        throw std::runtime_error("Incorrect tree dir, expected  \"" + dir + "\", got " + tree.registryEntry.dir + "\"");
    }

    if (tree.registryEntry.id != id)
    {
        throw std::runtime_error("Incorrect tree id, expected  \"" + id + "\", got " + tree.registryEntry.id + "\"");
    }

    if (tree.materials.size() != expectedMaterialCount)
    {
        throw std::runtime_error("Incorrect tree materials count, expected " + std::to_string(expectedMaterialCount) + ", got " + std::to_string(tree.materials.size()));
    }
}


int main()
{
    try
    {
        B3dRegistry registry
        {
            D2_ROOT_DIR,
            {
                {"ENV", "aa"},
                {"ENV", "ab"},
                {"ENV", "ac"},
                {"COMMON", "common"},
            }
        };

        B3dForest forest = ReadB3d(registry);

        TestForestEntry(forest, 0, "ENV", "aa", 114);
        TestForestEntry(forest, 1, "ENV", "ab", 53);
        TestForestEntry(forest, 2, "ENV", "ac", 85);
        TestForestEntry(forest, 3, "COMMON", "common", 171);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
