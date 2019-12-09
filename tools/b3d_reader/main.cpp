#include <iostream>
#include <fstream>
#include <string>

#include "d2_b3d_reader.h"



int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::string fileName = argv[1];

    std::ifstream inputFile(fileName.c_str());
    if (!inputFile)
    {
        std::cerr << "failed to open file [" << fileName << "]" << std::endl;
        return 2;
    }

    b3d::B3dReader reader;
    b3d::B3d data;
    reader.Read(inputFile, data);

    return 0;
}
