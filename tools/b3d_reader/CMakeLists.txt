set (b3d_reader_sources
    main.cpp
    b3d_reader.cpp
    b3d_reader.h
)

add_executable(b3d_reader ${b3d_reader_sources})

target_link_libraries(b3d_reader
    PRIVATE
        d2_hack_common
)
