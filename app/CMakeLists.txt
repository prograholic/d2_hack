d2_hack_add_executable(d2_hack.app
  base_application.cpp
  base_application.h

  application.cpp
  application.h
  
  main.cpp
)

target_link_libraries(d2_hack.app
    PRIVATE
        OgreBites
        OgreTerrain
        d2_hack.codec
)

cotire(d2_hack.app)
