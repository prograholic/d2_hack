#include "analyze.h"

boost::program_options::options_description get_analyze_options()
{
    boost::program_options::options_description print_options("Analysis options");

    return print_options;
}



int analyze(const d2_hack::resource::data::b3d::B3dForest& /* forest */, const boost::program_options::variables_map& /* options */)
{
    return 0;
}