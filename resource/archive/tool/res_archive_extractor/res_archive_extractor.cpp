#include <iostream>
#include <stdexcept>
#include <filesystem>

#include <OgreLogManager.h>
#include <OgreArchiveManager.h>

#include <boost/program_options.hpp>

#include <d2_hack/resource/archive/res_archive.h>
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

void ReadResourceFromArchiveToFile(ResArchive& archive, const std::filesystem::path& outputDir)
{
    Ogre::StringVectorPtr resources = archive.list();
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

        std::filesystem::path outputFilename = outputDir / resourceFileName;
        std::filesystem::create_directories(outputFilename.parent_path());

        std::ofstream outputStream{ outputFilename, std::ios_base::binary };
        if (!outputStream)
        {
            throw std::runtime_error(std::format("cannot open {} for writing", outputFilename.string()));
        }

        auto data = stream->getAsString();
        outputStream.write(data.data(), data.size());

        std::cout << "processed: " << resourceFileName << std::endl;
    }
}

void ReadMaterialsAndColorsFromResFile(const std::filesystem::path& resName, const std::filesystem::path& outputDir)
{
    ResArchive archive{resName.string(), "test"};

    archive.load();

    auto outputSubDir = outputDir / std::filesystem::relative(resName, D2_ROOT_DIR);
    ReadResourceFromArchiveToFile(archive, outputSubDir);
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
static const char output_dir[] = "output_dir";


} //namespace options

namespace po = boost::program_options;

std::list<std::filesystem::path> GetResFileList(const po::variables_map& vm)
{
    std::list<std::filesystem::path> res;
    if (vm.contains(options::res_name))
    {
        res.push_back(D2_ROOT_DIR / vm[options::res_name].as<std::filesystem::path>());
        return res;
    }

    if (vm.contains(options::print_all_resources))
    {
        for (const auto& path : std::filesystem::recursive_directory_iterator(D2_ROOT_DIR))
        {
            if (path.path().extension() == ".res")
            {
                res.push_back(path.path());
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
            (options::res_name, po::value<std::string>(), "resource file name (relative to " D2_ROOT_DIR ")")
            (options::output_dir, po::value<std::string>()->required()->default_value("res_output"), "output directory")
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
        logMgr.createLog("log", true, true, true);
        Ogre::ResourceGroupManager rgMgr;
        rgMgr.createResourceGroup(d2_hack::common::DefaultResourceGroup);

        manager::Manager mgr;
        ResArchive::Factory factory;

        Ogre::ArchiveManager arcMgr;
        arcMgr.addArchiveFactory(&factory);


        rgMgr.addResourceLocation(D2_ROOT_DIR "/COMMON/COMMON.RES", "D2Res", d2_hack::common::DefaultResourceGroup);

        auto resFileList = GetResFileList(vm);

        std::filesystem::path outputDir = vm[options::output_dir].as<std::string>();

        for (const auto& resFile : resFileList)
        {
            ReadMaterialsAndColorsFromResFile(resFile, outputDir);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
