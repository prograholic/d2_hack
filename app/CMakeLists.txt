d2_hack_add_library(d2_hack.app
    include/d2_hack/app/base_application.h
    base_application.cpp

    include/d2_hack/app/base_b3d_application.h
    base_b3d_application.cpp

    include/d2_hack/app/b3d_car.h
    b3d_car.cpp

    include/d2_hack/app/b3d_room.h
    b3d_room.cpp

    include/d2_hack/app/base_game_object.h
    base_game_object.cpp

    include/d2_hack/app/b3d_scene_builder_context.h

    base_game_object_visitor.cpp
    base_game_object_visitor.h
    
    car_visitor.cpp
    car_visitor.h
    
    room_visitor.cpp
    room_visitor.h
)

target_link_libraries(d2_hack.app
    PUBLIC
        OgreBites
        OgreTerrain
        d2_hack.resource
        d2_hack.scene_node
)

add_subdirectory(tool)
