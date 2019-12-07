set(d2_hack_sources
  base_application.cpp
  base_application.h

  d2_hack_application.cpp
  d2_hack_application.h
  
  main.cpp
)

add_executable(d2_hack ${d2_hack_sources})

target_link_libraries(d2_hack
    PRIVATE
        OgreBites
        d2_hack_resource_loader
        d2_hack_common
)
