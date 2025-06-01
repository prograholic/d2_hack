#include <iostream>
#include <stdexcept>
#include <filesystem>

#include <OgreLogManager.h>

#include <boost/program_options.hpp>

#include <d2_hack/resource/archive/res.h>
#include <d2_hack/resource/manager/manager.h>
#include <d2_hack/common/reader.h>
#include <d2_hack/common/types.h>

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

void ReadPalleteFromCommon()
{
    ResArchive archive{D2_ROOT_DIR "/COMMON/common.res", "test"};

    archive.load();
    Ogre::DataStreamPtr stream = archive.open("common\\common.plm");
    if (!stream)
    {
        throw std::runtime_error("common: stream is NULL");
    }

    PlmReader plmReader{ *stream };

    Plm plm;
    plmReader.Read(plm);
}


void ReadResourceFromArchive(ResArchive& archive, const std::string& mask)
{
    Ogre::StringVectorPtr resources = archive.find(mask);
    if (!resources)
    {
        throw std::runtime_error("resources is NULL");
    }
    if (resources->empty())
    {
        throw std::runtime_error("resources is empty");
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

void ReadMaterialsAndColorsFromResFile(const std::string& resName)
{
    ResArchive archive{D2_ROOT_DIR "/ENV/" + resName, "test"};

    archive.load();
    ReadResourceFromArchive(archive, "*.d2colorinfo");
    ReadResourceFromArchive(archive, "*.material");
}

} // namespace res
} // namespace archive
} // namespace resource
} // namespace d2_hack

namespace options
{

static const char help[] = "help";
static const char res_name[] = "res_name";
static const char print_all_resources[] = "print_all_resources";


} //namespace options

namespace po = boost::program_options;

std::list<std::string> GetResFileList(const po::variables_map& vm)
{
    std::list<std::string> res;
    if (vm.contains(options::res_name))
    {
        res.push_back(vm[options::res_name].as<std::string>());
        return res;
    }

    if (vm.contains(options::print_all_resources))
    {
        for (const auto& path : std::filesystem::directory_iterator(D2_ROOT_DIR "/ENV/"))
        {
            if (path.path().extension() == ".res")
            {
                res.push_back(path.path().filename().string());
            }
        }
    }

    return res;
}

int main(int argc, char* argv[])
{
    try
    {
        po::options_description general("Allowed options");

        general.add_options()
            (options::help, "Produce help message")
            (options::res_name, po::value<std::string>(), "resource file name")
            (options::print_all_resources, "print all resources in subdirectory");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, general), vm);
        po::notify(vm);

        if (vm.count(options::help))
        {
            std::cerr << general << std::endl;
            return 0;
        }

        using namespace d2_hack::resource;
        using namespace d2_hack::resource::archive::res;


        Ogre::LogManager logMgr;
        Ogre::ResourceGroupManager rgMgr;
        rgMgr.createResourceGroup(d2_hack::common::DefaultResourceGroup);

        manager::Manager mgr;
        ResArchive::Factory factory;

        Ogre::ArchiveManager arcMgr;
        arcMgr.addArchiveFactory(&factory);


        rgMgr.addResourceLocation(D2_ROOT_DIR "/COMMON/COMMON.RES", "D2Res", d2_hack::common::DefaultResourceGroup);

        ReadPalleteFromCommon();


        auto resFileList = GetResFileList(vm);

        for (const auto& resFile : resFileList)
        {
            ReadMaterialsAndColorsFromResFile(resFile);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
