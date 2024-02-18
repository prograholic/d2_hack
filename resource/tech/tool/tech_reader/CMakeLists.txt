d2_hack_add_executable(d2_hack.resource.tech.tool.tech_reader
    main.cpp
)

target_link_libraries(d2_hack.resource.tech.tool.tech_reader
    PRIVATE
        d2_hack.resource.tech
)
