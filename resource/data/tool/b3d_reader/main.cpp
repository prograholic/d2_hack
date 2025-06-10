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
            (options::generic::help, "Produce help message")
            (options::generic::mode, po::value<std::string>(), "select mode")
            (options::generic::output_dir, boost::program_options::value<std::string>()->required()->default_value("b3d_output"), "Output dir")
            (options::generic::subdir, po::value<std::string>(), "select subdir for b3d file")
            (options::generic::id, po::value<std::string>(), "select id")
            (options::generic::skip_transformation, "Skip transformation")
            (options::generic::skip_optimization, "Skip optimization")
            (options::generic::with_common, "Process common.b3d")
            (options::generic::with_trucks, "Process trucks.b3d")
            (options::generic::use_single_player_registry, "Use single player registry");

        general.add(get_print_options());
        general.add(get_analyze_options());

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, general), vm);
        po::notify(vm);

        if (vm.count(options::generic::help))
        {
            std::cerr << general << std::endl;
            return 0;
        }

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

        if (vm.count(options::generic::use_single_player_registry))
        {
            registry = SinglePlayerRegistry;
        }
        else
        {
            if (vm.contains(options::generic::subdir) && vm.contains(options::generic::id))
            {
                registry.dir = vm[options::generic::subdir].as<std::string>();
                registry.entries.push_back(vm[options::generic::id].as<std::string>());
            }
        }

        B3dForest forest = ReadB3d(registry);

        if (!vm.count(options::generic::skip_transformation))
        {
            transformation::Transform(forest);
        }
        if (!vm.count(options::generic::skip_optimization))
        {
            transformation::Optimize(forest);
        }

        const std::string op = vm[options::generic::mode].as<std::string>();

        if (op == options::modes::print)
        {
            return print(forest, vm);
        }
        else if (op == options::modes::analyze)
        {
            return analyze(forest, vm);
        }
        else
        {
            std::cerr << "Allowed modes: {" << options::modes::print << ", " << options::modes::analyze << "}" << std::endl;
            return 1;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "fail: " << e.what() << std::endl;
    }

    return 1;
}
