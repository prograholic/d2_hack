#include <iostream>
#include <fstream>

#include "B3dReader.h"



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

  fileName += ".dump";

  std::ofstream outputFile(fileName.c_str());
  if (!outputFile)
  {
    std::cerr << "failed to open file [" << fileName << "]" << std::endl;
    return 3;
  }

  outputFile << "processing file [" << fileName << "]" << std::endl;
  std::cout  << "processing file [" << fileName << "]" << std::endl;

  B3dReader reader(inputFile, outputFile);
  reader.read();

  return 0;
}
