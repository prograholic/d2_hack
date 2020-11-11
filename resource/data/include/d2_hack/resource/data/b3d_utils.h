#ifndef D2_HACK_RESOURCE_PLUGIN_D2_B3D_UTILS_H
#define D2_HACK_RESOURCE_PLUGIN_D2_B3D_UTILS_H

#include <d2_hack/resource/data/b3d_types.h>


namespace d2_hack
{
namespace resource
{
namespace data
{
namespace b3d
{

common::SimpleMeshInfo PrepareStandaloneMeshInfo(const block_data::SimpleFaces8& block, const block_data::Face8& face, const common::SimpleMeshInfo& parentMeshInfo);

common::SimpleMeshInfo PrepareStandaloneMeshInfo(const block_data::SimpleFaces28& block, const block_data::Face28& face, const common::SimpleMeshInfo& parentMeshInfo);

common::SimpleMeshInfo PrepareStandaloneMeshInfo(const block_data::SimpleFaces35& block, const block_data::Face35& face, const common::SimpleMeshInfo& parentMeshInfo);

} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack




#endif /* D2_HACK_RESOURCE_PLUGIN_D2_B3D_UTILS_H */
