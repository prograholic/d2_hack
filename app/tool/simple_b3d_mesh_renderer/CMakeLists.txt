d2_hack_add_executable(d2_hack.app.simple_b3d_mesh_renderer
    simple_b3d_mesh_renderer.cpp
    simple_b3d_mesh_renderer.h
    
    main.cpp
)

target_link_libraries(d2_hack.app.simple_b3d_mesh_renderer
    PRIVATE
        d2_hack.app
)
