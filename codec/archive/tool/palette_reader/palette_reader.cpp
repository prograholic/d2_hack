#include <iostream>
#include <stdexcept>

#include <d2_hack/codec/archive/res.h>
#include <d2_hack/common/reader.h>

namespace d2_hack
{
namespace codec
{
namespace archive
{
namespace res
{

struct PaletteEntry
{
    std::uint8_t red;
    std::uint8_t green;
    std::uint8_t blue;
};

typedef std::vector<PaletteEntry> Palette;

static const size_t PlmSignatureSize = 4;

struct PlmHeader
{
    char signature[PlmSignatureSize];
    std::uint32_t fileSize;
};

struct Plm
{
    PlmHeader header;

    Palette palette;
};


class PlmReader : private common::Reader
{
public:
    explicit PlmReader(Ogre::DataStream& stream)
        : common::Reader(stream)
    {
    }

    void Read(Plm& plm)
    {
        ReadBytes(plm.header.signature, PlmSignatureSize);
        plm.header.fileSize = ReadUint32();

        while (GetStream().tell() < GetStream().size())
        {
            char entryName[PlmSignatureSize];
            ReadBytes(entryName, PlmSignatureSize);

            if (strncmp(entryName, "PALT", PlmSignatureSize) == 0)
            {
                ReadPalette(plm.palette);
            }
            else
            {
                ThrowError("Unknown ID " + std::string(entryName, PlmSignatureSize), "PlmReader::Read");
            }
        }
    }

private:

    void ReadPalette(Palette& palette)
    {
        const std::uint32_t size = ReadUint32();
        const size_t paletteEntriesCount = size / sizeof(PaletteEntry);

        if (paletteEntriesCount * sizeof(PaletteEntry) != size)
        {
            ThrowError("Incorrect palette size: " + std::to_string(size), "PlmReader::ReadPalette");
        }

        palette.resize(paletteEntriesCount);
        ReadBytes(palette.data(), size);
    }
};

void ReadPalleteFromCommon()
{
    ResArchive archive{ D2_ROOT_DIR "/COMMON/common.res", "test" };

    archive.load();
    Ogre::DataStreamPtr stream = archive.open("common.plm");
    if (!stream)
    {
        throw std::runtime_error("common: stream is NULL");
    }

    PlmReader plmReader{ *stream };

    Plm plm;
    plmReader.Read(plm);
}


void ReadMaterialsFromAa()
{
    ResArchive archive{ D2_ROOT_DIR "/ENV/aa.res", "test" };

    archive.load();
    Ogre::StringVectorPtr materials = archive.find("*.material");
    if (!materials)
    {
        throw std::runtime_error("aa: materials is NULL");
    }
    if (materials->empty())
    {
        throw std::runtime_error("aa: materials is empty");
    }

    for (const auto& materialFileName : *materials)
    {
        Ogre::DataStreamPtr stream = archive.open(materialFileName);
        if (!stream)
        {
            throw std::runtime_error("stream is NULL for " + materialFileName);
        }

        std::cout << materialFileName << std::endl;
        std::cout << "    " << stream->getAsString() << std::endl;
    }
}

} // namespace res
} // namespace archive
} // namespace codec
} // namespace d2_hack



int main()
{
    try
    {
        using namespace d2_hack::codec::archive::res;

        ReadPalleteFromCommon();
        ReadMaterialsFromAa();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
