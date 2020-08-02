d2_hack_add_executable(d2_hack.app.terrain_viewer
    terrain_viewer.cpp
    terrain_viewer.h
    
    main.cpp
)

target_link_libraries(d2_hack.app.terrain_viewer
    PRIVATE
        d2_hack.app
)
