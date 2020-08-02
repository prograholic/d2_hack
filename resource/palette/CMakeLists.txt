d2_hack_add_library(d2_hack.resource.palette
    plm_reader.cpp
    plm_reader.h
    palette.cpp

    include/d2_hack/resource/palette/palette.h
)

target_link_libraries(d2_hack.resource.palette
    PRIVATE
        OgreMain
    PUBLIC
        d2_hack.common
)
