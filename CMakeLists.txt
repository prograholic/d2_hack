cmake_minimum_required(VERSION 2.8)

project(d2_hack)

find_package(OGRE 1.11 REQUIRED PATHS ${OGRE_DIR})
include(${OGRE_CONFIG_DIR}/../CMake/OgreTargets.cmake)


find_package(Boost REQUIRED)# COMPONENTS thread date_time system)
include_directories(${Boost_INCLUDE_DIRS})

#set(OGRE_ROOT ${OGRE_MEDIA_DIR})

set(APP_ROOT_DIRECTORY ${CMAKE_BINARY_DIR}/app_root)
set(APP_BIN_DIRECTORY ${APP_ROOT_DIRECTORY}/bin)
set(APP_CONFIG_DIRECTORY ${APP_ROOT_DIRECTORY}/config)
set(APP_DATA_DIRECTORY ${APP_ROOT_DIRECTORY}/data)

#add_definitions(
#    -DAPP_ROOT_DIRECTORY=${APP_ROOT_DIRECTORY}
#    -DAPP_BIN_DIRECTORY=${APP_BIN_DIRECTORY}
#    -DAPP_CONFIG_DIRECTORY=${APP_CONFIG_DIRECTORY}
#    -DAPP_DATA_DIRECTORY=${APP_DATA_DIRECTORY}
#)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${APP_BIN_DIRECTORY})



configure_file(config/resources.cfg.in ${APP_CONFIG_DIRECTORY}/resources.cfg)
configure_file(config/resources_d.cfg.in ${APP_CONFIG_DIRECTORY}/resources_d.cfg)
configure_file(config/plugins.cfg.in ${APP_CONFIG_DIRECTORY}/plugins.cfg)
configure_file(config/plugins_d.cfg.in ${APP_CONFIG_DIRECTORY}/plugins_d.cfg)

function (copy_ogre)
configure_file()

add_subdirectory(ResourcePlugin)
add_subdirectory(common)
add_subdirectory(tools)
add_subdirectory(d2_hack)
