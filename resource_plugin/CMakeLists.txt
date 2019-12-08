set(d2_hack_resource_plugin_sources
    d2_res_archive.cpp
    d2_res_archive.h

    d2_res_file_info.cpp
    d2_res_file_info.h
    
    offset_data_stream.cpp
    offset_data_stream.h
    
    txr_image_codec.cpp
    txr_image_codec.h
    
    raw_image_codec.cpp
    raw_image_codec.h
)

add_library(d2_hack_resource_loader STATIC ${d2_hack_resource_plugin_sources})
target_link_libraries(d2_hack_resource_loader
    PRIVATE
        d2_hack_common
        Boost::boost
)

target_include_directories(d2_hack_resource_loader
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)