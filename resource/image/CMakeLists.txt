d2_hack_add_library(d2_hack.resource.image
    raw.cpp
    txr.cpp
    
    include/d2_hack/resource/image/raw.h
    include/d2_hack/resource/image/txr.h
)

target_link_libraries(d2_hack.resource.image
    PRIVATE
        OgreMain
    PUBLIC
        d2_hack.common
)
