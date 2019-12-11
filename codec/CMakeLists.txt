add_subdirectory(image)
add_subdirectory(archive)
add_subdirectory(data)

add_library(d2_hack.codec INTERFACE)

target_link_libraries(d2_hack.codec
    INTERFACE
        d2_hack.codec.image
        d2_hack.codec.archive
        d2_hack.codec.data
)
