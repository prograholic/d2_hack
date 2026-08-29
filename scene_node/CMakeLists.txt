d2_hack_add_library(d2_hack.scene_node
    scene_node_base.cpp
    generator_scene_nodes.cpp
    switchable_scene_nodes.cpp
    
    include/d2_hack/scene_node/scene_node_base.h
    include/d2_hack/scene_node/generator_scene_nodes.h
    include/d2_hack/scene_node/switchable_scene_nodes.h
)

target_link_libraries(d2_hack.scene_node
    PRIVATE
        OgreMain
    PUBLIC
        d2_hack.common
        d2_hack.resource
)
