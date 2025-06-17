#ifndef D2_HACK_RESOURCE_DATA_B3D_TREE_H
#define D2_HACK_RESOURCE_DATA_B3D_TREE_H

#include <span>

#include <d2_hack/resource/data/b3d_node.h>

namespace d2_hack
{
namespace resource
{
namespace data
{
namespace b3d
{

// List of file names without extension
typedef std::vector <std::string> B3dRegistryEntries;

struct B3dRegistry
{
    std::string rootDir;
    std::string dir;
    B3dRegistryEntries entries;
};

extern const B3dRegistry SinglePlayerRegistry;

typedef std::span<const std::string_view> CarNameList;

extern const CarNameList AllCarNames;

struct B3dTree
{
    std::string dir;
    std::string id;
    common::Materials materials;

    block_data::VolumeCollisionList volumeCollisions;

    TransformationMap transformations;

    B3dNodeList rootNodes;

    std::string GetMaterialNameByIndex(std::uint32_t materialIndex) const;

    void AddRootNode(const B3dNodePtr& node);
};

typedef std::weak_ptr<B3dTree> B3dTreeWeakPtr;
typedef std::shared_ptr<B3dTree> B3dTreePtr;

struct B3dForest
{
    B3dTreePtr common;
    B3dTreePtr trucks;
    std::vector<B3dTreePtr> forest;
};

} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack


#endif /* D2_HACK_RESOURCE_DATA_B3D_TREE_H */
