function(copy_target_runtime target)
    set(should_copy_debug OFF)
    set(should_copy_release OFF)
    set(should_copy_release_with_debug OFF)
    
    if ((CMAKE_CONFIGURATION_TYPES MATCHES "Debug") OR (CMAKE_BUILD_TYPE MATCHES "Debug"))
        set(should_copy_debug ON)
    endif()
    
    if ((CMAKE_CONFIGURATION_TYPES MATCHES "Release") OR (CMAKE_BUILD_TYPE MATCHES "Release"))
        set(should_copy_release ON)
    endif()
    
    if ((CMAKE_CONFIGURATION_TYPES MATCHES "RelWithDebInfo") OR (CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo"))
        set(should_copy_release_with_debug ON)
    endif()

    if(should_copy_debug)
        get_target_property(target_location
            ${target}
            IMPORTED_LOCATION_DEBUG
        )
        
        message(STATUS "Copy Debug location: ${target_location}")
        
        configure_file(${target_location} ${APP_BIN_DIRECTORY} COPYONLY)
    endif()
    
    if(should_copy_release)
        get_target_property(target_location
            ${target}
            IMPORTED_LOCATION_RELEASE
        )
        
        message(STATUS "Copy Release location: ${target_location}")
        
        configure_file(${target_location} ${APP_BIN_DIRECTORY} COPYONLY)
    endif()
    
    if(should_copy_release_with_debug)
        get_target_property(target_location
            ${target}
            IMPORTED_LOCATION_RELWITHDEBINFO
        )
        
        message(STATUS "Copy RelWithdebInfo location: ${target_location}")
        
        configure_file(${target_location} ${APP_BIN_DIRECTORY} COPYONLY)
    endif()
endfunction()


function(copy_runtime dll_name suffix suffix2 runtime_dir)
    set(runtime_path ${runtime_dir}/${dll_name}${suffix}${CMAKE_SHARED_LIBRARY_SUFFIX})
    if (NOT EXISTS ${runtime_path})
        set(runtime_path ${runtime_dir}/${dll_name}${suffix2}${CMAKE_SHARED_LIBRARY_SUFFIX})
        if (NOT EXISTS ${runtime_path})
            message(FATAL_ERROR "unknown location of ${dll_name}")
        endif()
    endif()
    
    message(STATUS "copy (${runtime_path} to ${APP_BIN_DIRECTORY} ...")
    configure_file(${runtime_path} ${APP_BIN_DIRECTORY} COPYONLY)
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
        copy_runtime(${dll_name} "_d" "_d" "${OGRE_MEDIA_DIR}/../bin")
    endif()
    
    if (should_copy_release)
        copy_runtime(${dll_name} "" "" "${OGRE_MEDIA_DIR}/../bin")
    endif()
    
endfunction()

function (copy_zlib_runtime)
    copy_runtime("zlib" "1" "d1" "${ZLIB_ROOT}/bin")
endfunction()

function (copy_sdl2_runtime)
    copy_target_runtime(SDL2::SDL2)
endfunction()

function (copy_freetype_runtime)
    #copy_target_runtime(Freetype::Freetype) # does not work as expected (copies only implib)
    copy_runtime("freetype" "" "d" "$ENV{FREETYPE_DIR}/bin")
endfunction()
