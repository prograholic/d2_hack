#include <iostream>

#include <d2_hack/resource/tech/tech_reader.h>

namespace d2_hack
{
namespace resource
{
namespace tech
{

void PrintTechHeader(const TechHeader& header)
{
    std::cout << "length:      " << header.length << std::endl;
    std::cout << "filename:    " << header.filename.data() << std::endl;
    std::cout << "timeUnknown: " << header.timeUnknown << std::endl;
}

void PrintTechFile()
{
    Tech t = ReadTech(D2_ROOT_DIR "/vehicle.tech");

    PrintTechHeader(t.header);
}

} // namespace tech
} // namespace resource
} // namespace d2_hack






using namespace d2_hack::resource::tech;

int main(int /* argc */, char* /* argv */[])
{
    PrintTechFile();
}
