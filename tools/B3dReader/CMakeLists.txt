
set (B3dReaderSources
  main.cpp
  B3dReader.cpp
  B3dReader.h
)

add_executable(b3d_reader ${B3dReaderSources})

target_link_libraries(b3d_reader
  ${OGRE_LIBRARIES}
  ${Boost_LIBRARIES}
  d2_hack_common
)
