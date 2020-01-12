d2_hack_add_library(d2_hack.codec.data
    b3d_reader.cpp
    b3d_reader2.cpp
    
    include/d2_hack/codec/data/b3d_block_data_decl.h
    include/d2_hack/codec/data/b3d_reader.h
    include/d2_hack/codec/data/b3d_block_data_decl2.h
    include/d2_hack/codec/data/b3d_reader2.h
    include/d2_hack/codec/data/b3d.h
)

target_link_libraries(d2_hack.codec.data
    PRIVATE
        OgreMain
    PUBLIC
        d2_hack.common
)

cotire(d2_hack.codec.data)

add_subdirectory(test)
add_subdirectory(tool)
