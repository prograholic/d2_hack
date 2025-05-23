#include <iostream>

#include <OgreLogManager.h>

#include <boost/program_options.hpp>

#include <d2_hack/resource/data/b3d_tree.h>
#include <d2_hack/resource/data/b3d_reader.h>
#include <d2_hack/resource/data/b3d_tree_optimization.h>

#include "options.h"

#include "print.h"
#include "analyze.h"


int main(int argc, char* argv[])
{
    try
    {
        namespace po = boost::program_options;

        po::options_description general("Allowed options");

        general.add_options()
            ("help", "Produce help message")
            (options::mode, po::value<std::string>(), "select mode")
            (options::subdir, po::value<std::string>(), "select subdir for b3d file")
            (options::id, po::value<std::string>(), "select id")
            (options::skip_transformation, "Skip transformation")
            (options::skip_optimization, "Skip optimization")
            (options::only_common, "Process only common.b3d")
            (options::only_trucks, "Process ony trucks.b3d")
            (options::use_single_player_registry, "Use single player registry");

        general.add(get_print_options());
        general.add(get_analyze_options());

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, general), vm);
        po::notify(vm);

        Ogre::LogManager logMgr;
        logMgr.createLog("default", true, false, true);

        using namespace d2_hack::resource::data::b3d;

        B3dRegistry registry
        {
            D2_ROOT_DIR,
            "",
            {
            }
        };

        if (vm.count(options::use_single_player_registry))
        {
            registry = SinglePlayerRegistry;
        }
        else
        {
            if (vm.count(options::only_common) || vm.count(options::only_trucks))
            {
                //skip
            }
            else
            {
                registry.dir = vm[options::subdir].as<std::string>();
                registry.entries.push_back(vm[options::id].as<std::string>());
            }
        }

        B3dForest forest = ReadB3d(registry);

        if (!vm.count(options::skip_transformation))
        {
            transformation::Transform(forest);
        }
        if (!vm.count(options::skip_optimization))
        {
            transformation::Optimize(forest);
        }

        const std::string op = vm[options::mode].as<std::string>();

        if (op == "print")
        {
            return print(forest, vm);
        }
        else if (op == "analyze")
        {
            return analyze(forest, vm);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "fail: " << e.what() << std::endl;
    }



    return 0;
}
