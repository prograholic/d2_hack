#ifndef D2_HACK_RESOURCE_DATA_TOOL_B3D_READER_OPTIONS_H
#define D2_HACK_RESOURCE_DATA_TOOL_B3D_READER_OPTIONS_H


namespace options
{
static const char use_single_player_registry[] = "use_single_player_registry";
static const char subdir[] = "subdir";
static const char id[] = "id";
static const char skip_transformation[] = "skip_transformation";
static const char skip_optimization[] = "skip_optimization";
static const char only_common[] = "only_common";
static const char only_trucks[] = "only_trucks";
static const char mode[] = "mode";


namespace printing
{
static const char skip_vector_data[] = "skip_vector_data";
static const char skip_bounding_sphere[] = "skip_bounding_sphere";
static const char skip_face_info[] = "skip_face_info";
static const char skip_mesh_info[] = "skip_mesh_info";
static const char print_only_names[] = "print_only_names";

} // namespace printing

namespace analysis
{

static const char collect_entries_list_patterns[] = "collect_entries_list_patterns";

} // namespace analysis





} // namespace options





#endif /* D2_HACK_RESOURCE_DATA_TOOL_B3D_READER_OPTIONS_H */
