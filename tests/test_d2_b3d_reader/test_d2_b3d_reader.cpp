#include <iostream>
#include <fstream>

#include "d2_b3d_reader.h"

int main()
{
    std::ifstream inputFile{ D2_AA_B3D_FILENAME };
    if (!inputFile)
    {
        std::cerr << "failed to open file [" << D2_AA_B3D_FILENAME << "]" << std::endl;
        return 1;
    }

    b3d::B3dReader reader;
    b3d::B3d data;
    reader.Read(inputFile, data);

    if (data.materials.size() != 114)
    {
        std::cerr << "incorrect materials size: " << data.materials.size() << std::endl;
        return 1;
    }

    return 0;
}
