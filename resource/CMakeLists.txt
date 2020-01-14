add_subdirectory(image)
add_subdirectory(archive)
add_subdirectory(data)
add_subdirectory(manager)

add_library(d2_hack.resource INTERFACE)

target_link_libraries(d2_hack.resource
    INTERFACE
        d2_hack.resource.image
        d2_hack.resource.archive
        d2_hack.resource.data
)
