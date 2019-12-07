cmake_minimum_required(VERSION 2.8)

project(d2_hack CXX)

find_package(Boost REQUIRED)

find_package(OGRE 1.11 REQUIRED PATHS ${OGRE_DIR})
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

#add_definitions(
#    -DAPP_ROOT_DIRECTORY=${APP_ROOT_DIRECTORY}
#    -DAPP_BIN_DIRECTORY=${APP_BIN_DIRECTORY}
#    -DAPP_CONFIG_DIRECTORY=${APP_CONFIG_DIRECTORY}
#    -DAPP_DATA_DIRECTORY=${APP_DATA_DIRECTORY}
#)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${APP_BIN_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${APP_BIN_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${APP_BIN_DIRECTORY})

configure_file(config/resources.cfg.in ${APP_CONFIG_DIRECTORY}/resources.cfg)
configure_file(config/resources_d.cfg.in ${APP_CONFIG_DIRECTORY}/resources_d.cfg)
configure_file(config/plugins.cfg.in ${APP_CONFIG_DIRECTORY}/plugins.cfg)
configure_file(config/plugins_d.cfg.in ${APP_CONFIG_DIRECTORY}/plugins_d.cfg)

function(copy_ogre_target_runtime target)
    get_target_property(target_location
        ${target}
        LOCATION
    )
    
    configure_file(${target_location} ${APP_BIN_DIRECTORY} COPYONLY)
endfunction()


function(copy_runtime dll_name suffix)
    set(runtime_dir ${OGRE_MEDIA_DIR}/../bin)
    set(runtime_path ${runtime_dir}/${dll_name}${suffix}${CMAKE_SHARED_LIBRARY_SUFFIX})
    if (EXISTS ${runtime_path})
        configure_file(${runtime_path} ${APP_BIN_DIRECTORY} COPYONLY)
    else()
        message(FATAL_ERROR "unknown location of ${dll_name}")
    endif()
endfunction()


function(copy_3rd_party_runtime dll_name)
    copy_runtime(${dll_name} "")
endfunction()

function(copy_ogre_runtime dll_name)
    set(should_copy_debug OFF)
    set(should_copy_release OFF)
    
    if ((CMAKE_CONFIGURATION_TYPES MATCHES "Debug") OR (CMAKE_BUILD_TYPE MATCHES "Debug"))
        set(should_copy_debug ON)
    endif()
    
    if ((CMAKE_CONFIGURATION_TYPES MATCHES "Rel") OR (CMAKE_BUILD_TYPE MATCHES "Rel"))
        set(should_copy_release ON)
    endif()
    
    if (should_copy_debug)
        copy_runtime(${dll_name} "_d")
    endif()
    
    if (should_copy_release)
        copy_runtime(${dll_name} "")
    endif()
    
endfunction()

copy_ogre_target_runtime(OgreBites)
copy_ogre_target_runtime(OgreMain)
copy_ogre_target_runtime(OgreOverlay)
copy_ogre_target_runtime(OgreRTShaderSystem)

copy_ogre_runtime(RenderSystem_GL)


copy_3rd_party_runtime(zlib)
copy_3rd_party_runtime(sdl2)



if (CMAKE_CXX_COMPILER MATCHES "MSVC")
    add_definitions(/W4 /WX)
else()
    message(FATAL_ERROR "Unsupported compiler, please enable highest warning level and enable warning as error")
endif()


add_subdirectory(resource_plugin)
add_subdirectory(common)
add_subdirectory(tools)
add_subdirectory(d2_hack)
