d2_hack_add_library(d2_hack.resource.data
    b3d_reader.cpp
    b3d_tree.cpp
    b3d_node.cpp
    b3d_visitor.cpp
    b3d_tree_optimization.cpp
    b3d_utils.cpp
    b3d_reader_visitors.cpp
    b3d_reader_visitors.h
    
    include/d2_hack/resource/data/b3d_types.h
    include/d2_hack/resource/data/b3d_reader.h
    include/d2_hack/resource/data/b3d_tree.h
    include/d2_hack/resource/data/b3d_node.h
    include/d2_hack/resource/data/b3d_visitor.h
    include/d2_hack/resource/data/b3d_tree_optimization.h
    include/d2_hack/resource/data/b3d_utils.h
)

target_link_libraries(d2_hack.resource.data
    PRIVATE
        OgreMain
    PUBLIC
        d2_hack.common
)

add_subdirectory(test)
add_subdirectory(tool)
