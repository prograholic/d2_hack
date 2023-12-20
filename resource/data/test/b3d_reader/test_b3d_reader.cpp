#include <iostream>
#include <fstream>
#include <stdexcept>

#include <d2_hack/resource/data/b3d_reader.h>


using namespace d2_hack::resource::data::b3d;

void TestTree(const B3dTree& tree, const std::string& dir, const std::string& id, size_t expectedMaterialCount)
{
    if (tree.dir != dir)
    {
        throw std::runtime_error("Incorrect tree dir, expected  \"" + dir + "\", got " + tree.dir + "\"");
    }

    if (tree.id != id)
    {
        throw std::runtime_error("Incorrect tree id, expected  \"" + id + "\", got " + tree.id + "\"");
    }

    if (tree.materials.size() != expectedMaterialCount)
    {
        throw std::runtime_error("Incorrect tree materials count, expected " + std::to_string(expectedMaterialCount) + ", got " + std::to_string(tree.materials.size()));
    }
}

void TestForestEntry(const B3dForest& forest, size_t pos, const std::string & dir, const std::string & id, size_t expectedMaterialCount)
{
    if (pos >= forest.forest.size())
    {
        throw std::runtime_error("Incorrect forest size, expected at least " + std::to_string(pos) + ", got " + std::to_string(forest.forest.size()));
    }

    TestTree(*forest.forest[pos], dir, id, expectedMaterialCount);
}


int main()
{
    try
    {
        B3dRegistry registry
        {
            D2_ROOT_DIR,
            "ENV",
            {
                "aa",
                "ab",
                "ac",
            }
        };

        B3dForest forest = ReadB3d(registry);

        TestTree(*forest.common, "COMMON", "common", 171);

        TestForestEntry(forest, 0, "ENV", "aa", 114);
        TestForestEntry(forest, 1, "ENV", "ab", 53);
        TestForestEntry(forest, 2, "ENV", "ac", 85);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
