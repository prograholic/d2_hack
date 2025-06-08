d2_hack_add_executable(d2_hack.resource.archive.tool.res_archive_extractor
    res_archive_extractor.cpp
)
target_link_libraries(d2_hack.resource.archive.tool.res_archive_extractor
    PRIVATE
        d2_hack.resource
        Boost::program_options
)
