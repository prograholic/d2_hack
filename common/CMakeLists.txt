set(D2HackCommonFiles
  D2HackCommon.h
  D2HackCommon.cpp
)

add_library(d2_hack_common ${D2HackCommonFiles})
target_link_libraries(d2_hack_common OgreMain)

target_include_directories(d2_hack_common
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
