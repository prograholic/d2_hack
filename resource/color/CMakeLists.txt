d2_hack_add_library(d2_hack.resource.color
    color.cpp

    include/d2_hack/resource/color/color.h
)

target_link_libraries(d2_hack.resource.color
    PRIVATE
        OgreMain
    PUBLIC
        d2_hack.common
)

cotire(d2_hack.resource.color)
