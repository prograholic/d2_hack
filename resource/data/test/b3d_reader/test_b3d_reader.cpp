#include <iostream>
#include <fstream>
#include <stdexcept>

#include <d2_hack/resource/data/b3d_reader.h>

namespace d2_hack
{
namespace resource
{
namespace data
{
namespace b3d
{


int LoadB3dFile(const char* b3dFileName, const size_t expectedMaterialsCount)
{
    std::ifstream inputFile{b3dFileName, std::ios_base::binary};
    if (!inputFile)
    {
        std::cerr << "failed to open file [" << b3dFileName << "]" << std::endl;
        return 1;
    }

    Ogre::FileStreamDataStream dataStream(&inputFile, false);

    d2_hack::resource::data::b3d::B3dReader reader;
    B3dTree tree = reader.Read(dataStream);

    if (tree.materials.size() != expectedMaterialsCount)
    {
        throw std::runtime_error("Expected " + std::to_string(expectedMaterialsCount) + " materials, got " + std::to_string(tree.materials.size()));
    }


    return 0;
}


} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack



#define TEST_B3D_READER(b3dFileName, expectedMaterialsSize) \
do \
{ \
    using namespace d2_hack::resource::data::b3d; \
    int res = LoadB3dFile(D2_ROOT_DIR b3dFileName, expectedMaterialsSize); \
    if (res != 0) \
    { \
        std::cerr << "failed  to load " b3dFileName << std::endl; \
        return 1; \
    } \
} while (0)


int main()
{
    try
    {
        TEST_B3D_READER("/ENV/aa.b3d", 114);
        TEST_B3D_READER("/ENV/ab.b3d", 53);
        TEST_B3D_READER("/ENV/ac.b3d", 85);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
