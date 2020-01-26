add_subdirectory(image)
add_subdirectory(archive)
add_subdirectory(data)
add_subdirectory(manager)
add_subdirectory(color)

add_library(d2_hack.resource INTERFACE)

target_link_libraries(d2_hack.resource
    INTERFACE
        d2_hack.resource.image
        d2_hack.resource.archive
        d2_hack.resource.data
        d2_hack.resource.manager
        d2_hack.resource.color
)
