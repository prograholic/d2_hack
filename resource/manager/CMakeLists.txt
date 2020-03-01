d2_hack_add_library(d2_hack.resource.manager
    manager.cpp

    include/d2_hack/resource/manager/manager.h
)

target_link_libraries(d2_hack.resource.manager
    PRIVATE
        OgreMain
    PUBLIC
        d2_hack.common
)

target_link_libraries(d2_hack.resource.manager.public_headers
    INTERFACE
        d2_hack.resource.palette.public_headers
)

cotire(d2_hack.resource.manager)
