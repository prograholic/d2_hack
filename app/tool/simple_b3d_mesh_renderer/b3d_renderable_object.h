#ifndef D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_RENDERABLE_OBJECT_H
#define D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_RENDERABLE_OBJECT_H

#include <d2_hack/common/platform.h>

#include <memory>
#include <vector>

#include <d2_hack/resource/data/b3d_node.h>

#include "b3d_scene_builder.h"

namespace d2_hack
{
namespace app
{

/**
 * 37 идет только в 8 28 35
 * остальное - это триггеры, лоды или ненужная группировка
 * поэтому для 37 делаем отдельный класс, в нем держим scenenode на 8, 28, 35
 * возможно, сам 37 оборачиваем  в scenenode
 *
 * также для каждой scenenode внутри 37 нужно еще сделать триггер для visibility
 *
 * TODO:
 *
 * Кажется, что для 7 примерно такая-же ситуация, посмотреть отдельно в types.log
 */
class B3dRenderableObject
{
public:
    B3dRenderableObject(const resource::data::b3d::B3dNodePtr& b3dNode, B3dSceneBuilder& sceneBuilder);
};

typedef std::unique_ptr<B3dRenderableObject> B3dRenderableObjectPtr;

typedef std::vector<B3dRenderableObjectPtr> B3dRenderableObjectList;

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_TOOL_SIMPLE_B3D_MESH_RENDERER_B3D_RENDERABLE_OBJECT_H */
