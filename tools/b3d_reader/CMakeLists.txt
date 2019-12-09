set (b3d_reader_sources
    main.cpp
)

add_executable(b3d_reader ${b3d_reader_sources})

target_link_libraries(b3d_reader
    PRIVATE
        d2_hack_resource_loader
)
