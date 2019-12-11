d2_hack_add_executable(d2_hack.codec.data.tool.b3d_reader
    main.cpp
)

target_link_libraries(d2_hack.codec.data.tool.b3d_reader
    PRIVATE
        d2_hack.codec.data
)

cotire(d2_hack.codec.data.tool.b3d_reader)
