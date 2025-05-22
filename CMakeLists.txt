cmake_minimum_required(VERSION 3.16)

project(d2_hack CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(copy_runtime)
include(setup_toolchain)
include(target)
include(setup_ide)

set(Boost_ADDITIONAL_VERSIONS "1.74.0" "1.74")

find_package(Boost COMPONENTS program_options REQUIRED)

find_package(OGRE 14 REQUIRED PATHS ${OGRE_DIR})
include(${OGRE_CONFIG_DIR}/../CMake/OgreTargets.cmake)

set(APP_ROOT_DIRECTORY ${CMAKE_BINARY_DIR}/app_root)
set(APP_BIN_DIRECTORY ${APP_ROOT_DIRECTORY})
set(APP_CONFIG_DIRECTORY ${APP_ROOT_DIRECTORY})
set(APP_DATA_DIRECTORY ${APP_ROOT_DIRECTORY}/data)

file(MAKE_DIRECTORY
    ${APP_ROOT_DIRECTORY}
    ${APP_BIN_DIRECTORY}
    ${APP_CONFIG_DIRECTORY}
    ${APP_DATA_DIRECTORY}
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${APP_BIN_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${APP_BIN_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${APP_BIN_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${APP_BIN_DIRECTORY})

copy_ogre_target_runtime(OgreBites)
copy_ogre_target_runtime(OgreMain)
copy_ogre_target_runtime(OgreOverlay)
copy_ogre_target_runtime(OgreRTShaderSystem)
copy_ogre_target_runtime(OgreTerrain)
copy_ogre_target_runtime(OgrePaging)

copy_ogre_runtime(RenderSystem_GL)

copy_3rd_party_runtime(zlib1 "")
copy_3rd_party_runtime(sdl2 "")
copy_3rd_party_runtime(freetype "")


enable_testing()

add_subdirectory(data)
add_subdirectory(common)
add_subdirectory(resource)
add_subdirectory(app)
add_subdirectory(scripts)
add_subdirectory(scene_node)
