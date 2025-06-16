#include <iostream>
#include <fstream>
#include <stdexcept>

#include <OgreLogManager.h>

#include <d2_hack/resource/data/b3d_reader.h>
#include <d2_hack/resource/data/b3d_tree_optimization.h>


using namespace d2_hack::resource::data::b3d;

void TestTree(const B3dTree& tree, const std::string_view& dir, const std::string_view& id, size_t expectedMaterialCount)
{
    if (tree.dir != dir)
    {
        throw std::runtime_error(std::format("Incorrect tree dir, expected  \"{}\", got \"{}\"", dir, tree.dir));
    }

    if (tree.id != id)
    {
        throw std::runtime_error(std::format("Incorrect tree id, expected  \"{}\", got \"{}\"", id, tree.id));
    }

    if (tree.materials.size() != expectedMaterialCount)
    {
        throw std::runtime_error(std::format("Incorrect tree materials count, expected {}, got {}",expectedMaterialCount, tree.materials.size()));
    }
}

void TestForestEntry(const B3dForest& forest, size_t pos, const std::string_view & dir, const std::string_view & id, size_t expectedMaterialCount)
{
    if (pos >= forest.forest.size())
    {
        throw std::runtime_error(std::format("Incorrect forest size, expected at least {}, got {}", pos, forest.forest.size()));
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

        Ogre::LogManager logMgr;
        logMgr.createLog("default", true, false, true);

        B3dForest forest = ReadB3d(registry);

        transformation::Transform(forest);
        transformation::Optimize(forest);

        TestTree(*forest.common, "COMMON", "COMMON", 171);

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
