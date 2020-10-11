#ifndef D2_HACK_RESOURCE_DATA_B3D_READER_H
#define D2_HACK_RESOURCE_DATA_B3D_READER_H

#include <d2_hack/common/platform.h>

#include <iosfwd>
#include <cstdint>
#include <vector>
#include <array>

#include <OgreDataStream.h>

#include <d2_hack/common/types.h>

#include <d2_hack/resource/data/b3d_types.h>
#include <d2_hack/resource/data/b3d_tree.h>

namespace d2_hack
{
namespace resource
{
namespace data
{
namespace b3d
{

class B3dReader
{
public:
    B3dTree Read(Ogre::DataStream& input);
};

} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack


#endif /* D2_HACK_RESOURCE_DATA_B3D_READER_H */
