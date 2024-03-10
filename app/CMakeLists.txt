d2_hack_add_library(d2_hack.app
    base_application.cpp
  
    include/d2_hack/app/base_application.h
)

target_link_libraries(d2_hack.app
    PUBLIC
        OgreBites
        OgreTerrain
        d2_hack.resource
        d2_hack.scene_node
)

add_subdirectory(tool)
