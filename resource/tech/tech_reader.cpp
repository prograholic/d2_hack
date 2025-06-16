#include <d2_hack/resource/tech/tech_reader.h>

#include <fstream>

#include <d2_hack/common/reader.h>

namespace d2_hack
{
namespace resource
{
namespace tech
{

class TechReader : public common::Reader
{
public:
    explicit TechReader(Ogre::DataStream& input)
        : Reader(input)
    {
    }

    Tech Read()
    {
        Tech res;

        ReadHeader(res.header);

        return res;
    }

private:

    void ReadHeader(TechHeader& header)
    {
        header.length = ReadUint32();
        ReadCount(header.filename, header.length);

        header.timeUnknown = ReadInt<std::uint32_t>();
    }
};

Tech ReadTech(const char* filename)
{
    std::ifstream inputFile{ filename, std::ios_base::binary };
    if (!inputFile)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_FILE_NOT_FOUND, std::format("failed to open file: `{}`", filename));
    }

    Ogre::FileStreamDataStream dataStream(&inputFile, false);

    TechReader techReader{dataStream};

    return techReader.Read();
}

} // namespace tech
} // namespace resource
} // namespace d2_hack
