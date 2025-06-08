#include <d2_hack/resource/data/b3d_tree.h>

#include <d2_hack/common/utils.h>
#include <d2_hack/common/resource_mgmt.h>

#include "b3d_reader_visitors.h"

namespace d2_hack
{
namespace resource
{
namespace data
{
namespace b3d
{
#define USE_ALL

const B3dRegistry SinglePlayerRegistry
{
	D2_ROOT_DIR,
	"ENV",
	{
#ifdef USE_ALL
		"aa",
		"ab",
		"ac",
		"ad",
		"ae",
		"af",
		"ag",
		"ah",
		"aj",
		"ak",
		"al",
		"am",
#endif
		"ap",
#ifdef USE_ALL
		"aq",
		"ar",
		"as",
		"at",
		"au",
		"av",
		"aw",
		"ax",
		"ba",
		"bb",
		"bc",
		"bd",
		"be",
		"bf",
		"bg",
		"ca",
		"cb",
		"cc",
		"ce",
		"cf",
		"ch",
		"da",
		"db",
		"dc",
		"dq",
		"dr"
#endif //USE_ALL
	}
};

std::string B3dTree::GetMaterialNameByIndex(std::uint32_t materialIndex) const
{
	return common::GetMaterialFileName(id, common::ResourceNameToString(materials[materialIndex]));
}


void AddTransformMatrix(const B3dNodePtr& root, TransformationMap& transformations)
{
	TransformVisitor visitor{transformations};
	auto visitResult = VisitNode(root, visitor);
	(void)visitResult;
}

void B3dTree::AddRootNode(const B3dNodePtr& root)
{
	switch (root->GetType())
	{
	case block_data::EmptyBlock0:
		// skip empty block
		break;

	case block_data::SimpleObjectConnectorBlock1:
	case block_data::GroupRoadInfraObjectsBlock4:
	case block_data::GroupObjectsBlock5:
	case block_data::GroupVertexDataBlock7:
	case block_data::GroupObjectsBlock19:
	case block_data::GroupObjectsBlock21:
	case block_data::GroupLightingObjectBlock33:
	case block_data::GroupVertexDataBlock37:
		rootNodes.push_back(root);
		break;

	case block_data::SimpleVolumeCollisionBlock23:
		volumeCollisions.push_back(root->NodeCast<NodeSimpleVolumeCollision23>()->GetBlockData());
		break;

	case block_data::HierarchyBreakerBlockXxx:
		// skip hierarchy breaker for root nodes
		break;

	case block_data::GroupTransformMatrixBlock24:
		AddTransformMatrix(root, transformations);
		break;

	default:
		OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "unsupported root node type: " + std::to_string(root->GetType()), "B3dTree::AddRootNode");
	}
}

} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack
