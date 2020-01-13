#include <iostream>
#include <fstream>
#include <stdexcept>

#include <d2_hack/codec/data/b3d_reader.h>

namespace d2_hack
{
namespace codec
{
namespace data
{
namespace b3d
{

class TestB3dListener : public VoidB3dListener
{
public:
    TestB3dListener(const size_t materialsCount)
        : m_materialsCount(materialsCount)
    {
    }

    virtual void OnMaterials(Materials&& materials) override
    {
        if (materials.size() != m_materialsCount)
        {
            throw std::runtime_error("Expected " + std::to_string(m_materialsCount) + " materials, got " + std::to_string(materials.size()));
        }
    }

private:
    const size_t m_materialsCount;
};


int LoadB3dFile(const char* b3dFileName, const size_t expectedMaterialsCount)
{
    std::ifstream inputFile{b3dFileName, std::ios_base::binary};
    if (!inputFile)
    {
        std::cerr << "failed to open file [" << b3dFileName << "]" << std::endl;
        return 1;
    }

    Ogre::FileStreamDataStream dataStream(&inputFile, false);

    TestB3dListener listener{ expectedMaterialsCount };

    d2_hack::codec::data::b3d::B3dReader reader;
    reader.Read(dataStream, listener);

    return 0;
}


} // namespace b3d
} // namespace data
} // namespace codec
} // namespace d2_hack



#define TEST_B3D_READER(b3dFileName, expectedMaterialsSize) \
do \
{ \
    using namespace d2_hack::codec::data::b3d; \
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
