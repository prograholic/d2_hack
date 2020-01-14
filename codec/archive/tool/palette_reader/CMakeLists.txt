d2_hack_add_test(d2_hack.codec.archive.tool.palette_reader
    palette_reader.cpp
)
target_link_libraries(d2_hack.codec.archive.tool.palette_reader
    PRIVATE
        d2_hack.codec.archive
        d2_hack.codec.archive.private
)

target_compile_definitions(d2_hack.codec.archive.tool.palette_reader
    PRIVATE
        -DD2_ROOT_DIR="${D2_ROOT_DIR_CMAKE_DIR}"
)

cotire(d2_hack.codec.archive.tool.palette_reader)
