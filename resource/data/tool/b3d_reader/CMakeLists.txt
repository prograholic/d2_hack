d2_hack_add_executable(d2_hack.resource.data.tool.b3d_reader
    main.cpp

    options.h

    print.cpp
    print.h

    analyze.cpp
    analyze.h
)

target_link_libraries(d2_hack.resource.data.tool.b3d_reader
    PRIVATE
        d2_hack.resource.data
        Boost::program_options
)
