d2_hack_add_library(d2_hack.resource.archive
    res_archive.cpp
    res_extensions.cpp
    res_file_info.cpp
    res_file_info.h
    res_material_parser.cpp
    res_material_parser.h
    ogre_material_provider.cpp
    
    include/d2_hack/resource/archive/res_archive.h
    include/d2_hack/resource/archive/res_extensions.h
    include/d2_hack/resource/archive/ogre_material_provider.h
)
target_link_libraries(d2_hack.resource.archive
    PRIVATE
        OgreMain
        d2_hack.resource.manager.public_headers
    PUBLIC
        d2_hack.common
)

add_subdirectory(test)
add_subdirectory(tool)
