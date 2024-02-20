d2_hack_add_executable(d2_hack.app.simple_b3d_mesh_renderer
    simple_b3d_mesh_renderer.cpp
    simple_b3d_mesh_renderer.h
    b3d_tree_visitor.cpp
    b3d_tree_visitor.h
    b3d_scene_builder.cpp
    b3d_scene_builder.h
    b3d_room.cpp
    b3d_room.h
    b3d_road.cpp
    b3d_road.h
    b3d_hit.cpp
    b3d_hit.h

    main.cpp
)

target_link_libraries(d2_hack.app.simple_b3d_mesh_renderer
    PRIVATE
        d2_hack.app
)
