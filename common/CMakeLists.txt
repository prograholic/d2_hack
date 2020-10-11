d2_hack_add_library(d2_hack.common
    offset_data_stream.cpp
    reader.cpp
    resource_mgmt.cpp
    utils.cpp
    
    include/d2_hack/common/log.h
    include/d2_hack/common/memory_mgmt.h
    include/d2_hack/common/numeric_conversion.h
    include/d2_hack/common/offset_data_stream.h
    include/d2_hack/common/platform.h
    include/d2_hack/common/reader.h
    include/d2_hack/common/types.h
    include/d2_hack/common/resource_mgmt.h
)

target_link_libraries(d2_hack.common
    PUBLIC
        Boost::boost
        OgreMain
)
