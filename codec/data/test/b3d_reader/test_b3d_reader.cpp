#include <iostream>
#include <fstream>

#include <d2_hack/codec/data/b3d_reader.h>

int LoadB3dFile(const char* b3dFileName, const size_t expectedMaterialsSize)
{
    std::ifstream inputFile{b3dFileName, std::ios_base::binary};
    if (!inputFile)
    {
        std::cerr << "failed to open file [" << b3dFileName << "]" << std::endl;
        return 1;
    }

    d2_hack::codec::data::b3d::B3dReader reader;
    d2_hack::codec::data::b3d::B3d data;
    reader.Read(inputFile, data);

    if (data.materials.size() != expectedMaterialsSize)
    {
        std::cerr << "incorrect materials size: " << data.materials.size() << std::endl;
        return 1;
    }

    return 0;
}


#define TEST_B3D_READER(b3dFileName, expectedMaterialsSize) \
do \
{ \
    int res = LoadB3dFile(D2_ROOT_DIR b3dFileName, expectedMaterialsSize); \
    if (res != 0) \
    { \
        std::cerr << "failed  to load " b3dFileName << std::endl; \
        return 1; \
    } \
} while (0)

int main()
{
    TEST_B3D_READER("/ENV/aa.b3d", 114);
    TEST_B3D_READER("/ENV/ab.b3d", 53);
    //TEST_B3D_READER("/ENV/ac.b3d", 53);


    return 0;
}
