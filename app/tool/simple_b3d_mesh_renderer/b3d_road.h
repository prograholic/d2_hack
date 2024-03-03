#ifndef D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_ROAD_H
#define D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_ROAD_H

#include <d2_hack/common/platform.h>

#include <memory>

#include <OgreSceneNode.h>

#include <d2_hack/resource/data/b3d_node.h>

#include "b3d_renderable_object.h"
#include "b3d_hit.h"
#include "terrain.h"

namespace d2_hack
{
namespace app
{

class B3dRoad
{
public:
    B3dRoad(const resource::data::b3d::B3dNodePtr& b3dNode, B3dSceneBuilder& sceneBuilder);

private:
    B3dRenderableObjectList m_renderables;

    B3dHitPtr m_hit;
    TerrainPtr m_terrain;
};

typedef std::unique_ptr<B3dRoad> B3dRoadPtr;

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_ROAD_H */
