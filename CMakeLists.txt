cmake_minimum_required(VERSION 2.8)

project(d2_hack)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/utils)


find_package(Boost REQUIRED COMPONENTS thread date_time system)
include_directories(${Boost_INCLUDE_DIRS})

find_package(OIS REQUIRED)
include_directories(${OIS_INCLUDE_DIRS})

find_package(OGRE REQUIRED)
include_directories(${OGRE_INCLUDE_DIRS})
include_directories(${OGRE_Overlay_INCLUDE_DIRS})


set(OGRE_ROOT ${OGRE_MEDIA_DIR})

configure_file(data/resources.cfg.in resources.cfg)
configure_file(data/resources_d.cfg.in resources_d.cfg)
configure_file(data/plugins.cfg.in plugins.cfg)
configure_file(data/plugins_d.cfg.in plugins_d.cfg)

include_directories(ResourcePlugin)
include_directories(common)


add_subdirectory(ResourcePlugin)
add_subdirectory(common)

add_subdirectory(tools)


add_definitions(-DDEBUG)

set(D2HackSources
  BaseApplication.cpp
  BaseApplication.h

  TutorialApplication.cpp
  TutorialApplication.h
)

add_executable(d2_hack ${D2HackSources})

target_link_libraries(d2_hack
  ${OGRE_LIBRARIES}
  ${OGRE_Overlay_LIBRARIES}
  ${OIS_LIBRARIES}
  ${Boost_LIBRARIES}
  d2_hack_resource_loader
  d2_hack_common
)
