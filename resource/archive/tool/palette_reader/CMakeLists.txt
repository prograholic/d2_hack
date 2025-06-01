d2_hack_add_executable(d2_hack.resource.archive.tool.palette_reader
    palette_reader.cpp
)
target_link_libraries(d2_hack.resource.archive.tool.palette_reader
    PRIVATE
        d2_hack.resource
        Boost::program_options
)
