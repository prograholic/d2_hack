d2_hack_add_library(d2_hack.resource.data
    b3d_reader.cpp
    
    include/d2_hack/resource/data/b3d_block_data_decl.h
    include/d2_hack/resource/data/b3d_reader.h
    include/d2_hack/resource/data/b3d.h
)

target_link_libraries(d2_hack.resource.data
    PRIVATE
        OgreMain
    PUBLIC
        d2_hack.common
)

cotire(d2_hack.resource.data)

add_subdirectory(test)
add_subdirectory(tool)
