#ifndef D2_HACK_RESOURCE_DATA_B3D_TREE_OPTIMIZATION_H
#define D2_HACK_RESOURCE_DATA_B3D_TREE_OPTIMIZATION_H

#include <d2_hack/resource/data/b3d_tree.h>

namespace d2_hack
{
namespace resource
{
namespace data
{
namespace b3d
{
namespace transformation
{

/**
 * Делает трансформацию узлов без изменения их количества
 * 
 */
void Transform(B3dForest& forest);

void Optimize(B3dForest& forest);

} // namespace transformation
} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack


#endif /* D2_HACK_RESOURCE_DATA_B3D_TREE_OPTIMIZATION_H */
