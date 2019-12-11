#include <iostream>
#include <fstream>

#include <b3d_reader.h>

int main()
{
    std::ifstream inputFile{ D2_AA_B3D_FILENAME, std::ios_base::binary};
    if (!inputFile)
    {
        std::cerr << "failed to open file [" << D2_AA_B3D_FILENAME << "]" << std::endl;
        return 1;
    }

    d2_hack::codec::data::b3d::B3dReader reader;
    d2_hack::codec::data::b3d::B3d data;
    reader.Read(inputFile, data);

    if (data.materials.size() != 114)
    {
        std::cerr << "incorrect materials size: " << data.materials.size() << std::endl;
        return 1;
    }

    return 0;
}
