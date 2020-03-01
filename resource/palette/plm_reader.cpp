// Implementation based on https://github.com/AlexKimov/HardTruck-RignRoll-file-formats

#include "plm_reader.h"

namespace d2_hack
{
namespace resource
{
namespace palette
{

static const size_t SignatureSize = 4;
static const std::uint8_t PlmFileSignature[SignatureSize] = {'P', 'L', 'M', 0};
static const std::uint8_t PaletteSignature[SignatureSize] = {'P', 'A', 'L', 'T'};

PlmReader::PlmReader(Ogre::DataStream& input)
    : common::Reader(input)
{
}

void PlmReader::Read(PlmFile& plmFile)
{
    ReadHeader();

    while (!GetStream().eof())
    {
        std::uint8_t dataType[SignatureSize] = {};
        ReadBytes(dataType, SignatureSize);
        if (std::memcmp(dataType, PaletteSignature, SignatureSize) == 0)
        {
            ReadPalette(plmFile);
        }
        else
        {
            ThrowError("not implemented", "PlmReader::Read");
        }
    }
    
}

void PlmReader::ReadHeader()
{
    std::uint8_t signature[SignatureSize] = {};
    ReadBytes(signature, SignatureSize);
    if (std::memcmp(signature, PlmFileSignature, SignatureSize) != 0)
    {
        ThrowError("Incorrect plm signature", "PlmReader::ReadHeader");
    }

    const std::uint32_t fileSize = ReadUint32();
    if (fileSize != (GetStream().size() - GetStream().tell()))
    {
        ThrowError("Incorrect file size", "PlmReader::ReadHeader");
    }
}

void PlmReader::ReadPalette(PlmFile& plmFile)
{
    const size_t rgbSize = 3;
    const std::uint32_t paletteSize = ReadUint32();
    const size_t paletteCount = paletteSize / rgbSize;
    if ((paletteCount * rgbSize) != paletteSize)
    {
        ThrowError("Incorrect palette size", "PlmReader::ReadPalette");
    }

    std::vector<Ogre::ColourValue> palette{ paletteCount };
    for (std::uint32_t i = 0; i != paletteCount; ++i)
    {
        std::uint8_t rgb[rgbSize];
        ReadBytes(rgb, rgbSize);
        palette[i] = Ogre::ColourValue{rgb[0] / 255.0f, rgb[1] / 255.0f, rgb[2] / 255.0f};
    }

    plmFile.palette = std::move(palette);
}

} // namespace palette
} // namespace resource
} // namespace d2_hack
