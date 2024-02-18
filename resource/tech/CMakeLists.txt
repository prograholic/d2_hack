d2_hack_add_library(d2_hack.resource.tech
    tech_reader.cpp

    include/d2_hack/resource/tech/tech_types.h
    include/d2_hack/resource/tech/tech_reader.h
)

target_link_libraries(d2_hack.resource.tech
    PUBLIC
        d2_hack.common
)

add_subdirectory(tool)
