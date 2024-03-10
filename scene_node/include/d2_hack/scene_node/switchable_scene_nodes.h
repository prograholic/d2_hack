#ifndef D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_SWITCHABLE_SCENE_NODE_H
#define D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_SWITCHABLE_SCENE_NODE_H

#include <d2_hack/scene_node/scene_node_base.h>

#include <d2_hack/resource/data/b3d_types.h>

namespace d2_hack
{
namespace scene_node
{


class SwitchableSceneNodeBase : public SceneNodeBase
{
public:
    SwitchableSceneNodeBase(const std::string& name, std::uint32_t type, size_t activeNode);

    virtual void SetVisible(bool visible) override;

    void SetActive(size_t activeNode);

    size_t GetActiveNode() const;

private:
    size_t m_activeNode;
};

using GroupUnknown2 = OgreSceneNode<resource::data::b3d::block_data::GroupUnknownBlock2, SwitchableSceneNodeBase>;
using GroupTrigger9 = OgreSceneNode<resource::data::b3d::block_data::GroupTriggerBlock9, SwitchableSceneNodeBase>;
using GroupLod10 = OgreSceneNode<resource::data::b3d::block_data::GroupLodParametersBlock10, SwitchableSceneNodeBase>;
using SceneNodeEvent21 = OgreSceneNode<resource::data::b3d::block_data::GroupObjectsBlock21, SwitchableSceneNodeBase>;
using GroupUnknown29 = OgreSceneNode<resource::data::b3d::block_data::GroupUnknownBlock29, SwitchableSceneNodeBase>;


} // namespace scene_node
} // namespace d2_hack

#endif /* D2_HACK_SCENE_NODE_INCLUDE_D2_HACK_SCENE_NODE_SWITCHABLE_SCENE_NODE_H */
