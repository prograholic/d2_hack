set(d2_hack_common_files
    d2_hack_common.h
    d2_hack_common.cpp
    d2_hack_platform_support.h
)

add_library(d2_hack_common ${d2_hack_common_files})
target_link_libraries(d2_hack_common OgreMain)

target_include_directories(d2_hack_common
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
