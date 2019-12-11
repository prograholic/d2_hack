d2_hack_add_library(d2_hack.codec.image
    raw.cpp
    txr.cpp
    
    include/d2_hack/codec/image/raw.h
    include/d2_hack/codec/image/txr.h
)

target_link_libraries(d2_hack.codec.image
    PRIVATE
        OgreMain
    PUBLIC
        d2_hack.common
)

cotire(d2_hack.codec.image)
