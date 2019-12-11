d2_hack_add_library(d2_hack.codec.archive
    res.cpp
    res_file_info.cpp
    res_file_info.h
    
    include/d2_hack/codec/archive/res.h
)
target_link_libraries(d2_hack.codec.archive
    PRIVATE
        OgreMain
    PUBLIC
        d2_hack.common
)

cotire(d2_hack.codec.archive)

add_subdirectory(test)