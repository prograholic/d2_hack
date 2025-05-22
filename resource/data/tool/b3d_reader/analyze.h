#ifndef D2_HACK_RESOURCE_DATA_TOOL_B3D_READER_ANALYZE_H
#define D2_HACK_RESOURCE_DATA_TOOL_B3D_READER_ANALYZE_H

#include <string>

#include <boost/program_options.hpp>

#include <d2_hack/resource/data/b3d_tree.h>

boost::program_options::options_description get_analyze_options();

int analyze(const d2_hack::resource::data::b3d::B3dForest& forest, const boost::program_options::variables_map& options);

#endif /* D2_HACK_RESOURCE_DATA_TOOL_B3D_READER_ANALYZE_H */
