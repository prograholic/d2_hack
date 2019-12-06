set(D2HackSources
  BaseApplication.cpp
  BaseApplication.h

  TutorialApplication.cpp
  TutorialApplication.h
)

add_executable(d2_hack ${D2HackSources})

target_link_libraries(d2_hack
  OgreBites
  d2_hack_resource_loader
  d2_hack_common
)
