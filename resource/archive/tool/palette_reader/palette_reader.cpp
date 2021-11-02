#include <iostream>
#include <stdexcept>

#include <OgreLogManager.h>
#include <OgreArchiveManager.h>

#include <d2_hack/resource/archive/res.h>
#include <d2_hack/resource/manager/manager.h>
#include <d2_hack/common/reader.h>

namespace d2_hack
{
namespace resource
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

void ReadResourceFromArchive(ResArchive& archive, const std::string& mask)
{
    Ogre::StringVectorPtr resources = archive.find(mask);
    if (!resources)
    {
        throw std::runtime_error("aa: resources is NULL");
    }
    if (resources->empty())
    {
        throw std::runtime_error("aa: resources is empty");
    }

    for (const auto& resourceFileName : *resources)
    {
        Ogre::DataStreamPtr stream = archive.open(resourceFileName);
        if (!stream)
        {
            throw std::runtime_error("stream is NULL for " + resourceFileName);
        }

        std::cout << resourceFileName << std::endl;
        std::cout << "    " << stream->getAsString() << std::endl;
    }
}

void ReadMaterialsAndColorsFromRes()
{
    ResArchive archive{D2_ROOT_DIR "/ENV/ah.res", "test"};

    archive.load();
    ReadResourceFromArchive(archive, "*.d2colorinfo");
    ReadResourceFromArchive(archive, "*.material");
}

} // namespace res
} // namespace archive
} // namespace resource
} // namespace d2_hack



int main()
{
    try
    {
        using namespace d2_hack::resource;
        using namespace d2_hack::resource::archive::res;

        Ogre::LogManager logMgr;
        
        Ogre::ResourceGroupManager rgMgr;

        ResArchive::Factory d2ResArchiveFactory;
        Ogre::ArchiveManager archiveManager;
        manager::Manager mgr;
        
        Ogre::ArchiveManager::getSingleton().addArchiveFactory(&d2ResArchiveFactory);

        rgMgr.createResourceGroup("D2");
        rgMgr.addResourceLocation(D2_ROOT_DIR "/COMMON/common.res", "D2Res", "D2");

        ReadMaterialsAndColorsFromRes();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
