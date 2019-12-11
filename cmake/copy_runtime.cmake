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
