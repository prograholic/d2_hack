cmake_minimum_required(VERSION 2.8)

project(d2_hack)


find_package(Boost REQUIRED COMPONENTS thread date_time system)
include_directories(${Boost_INCLUDE_DIRS})

find_package(OIS REQUIRED)
include_directories(${OIS_INCLUDE_DIRS})

find_package(OGRE REQUIRED)
include_directories(${OGRE_INCLUDE_DIRS})


set(OGRE_ROOT ${OGRE_MEDIA_DIR})

configure_file(data/resources.cfg.in resources.cfg)
configure_file(data/resources_d.cfg.in resources_d.cfg)
configure_file(data/plugins.cfg.in plugins.cfg)
configure_file(data/plugins_d.cfg.in plugins_d.cfg)


set(D2HackSources
  main.cpp

  BaseApplication.cpp
  BaseApplication.h

  TutorialApplication.cpp
  TutorialApplication.h
)

add_executable(d2_hack ${D2HackSources})

target_link_libraries(d2_hack
  ${OGRE_LIBRARIES}
  ${OIS_LIBRARIES}
  ${Boost_LIBRARIES}
)
