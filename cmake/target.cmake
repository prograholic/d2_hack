function(d2_hack_set_common_target_props target_name)
    message(STATUS "Processing target ${target_name} ...")
    
    set_target_properties(${target_name}
        PROPERTIES
            COTIRE_PREFIX_HEADER_INCLUDE_PATH "${Boost_INCLUDE_DIR};${OGRE_DIR}../include/OGRE"
    )
endfunction()

function(d2_hack_add_executable target_name)
    add_executable(${target_name} ${ARGN})
    
    set_target_properties(${target_name}
        PROPERTIES
            VS_DEBUGGER_WORKING_DIRECTORY "${APP_BIN_DIRECTORY}"
    )

    d2_hack_set_common_target_props(${target_name} ${ARGN})
endfunction()

function(d2_hack_add_test target_name)
    d2_hack_add_executable(${target_name} ${ARGN})
    
    add_test(
        NAME
            ${target_name}
        COMMAND
            ${target_name}
        WORKING_DIRECTORY
            ${APP_ROOT_DIRECTORY}
    )
endfunction()

function(d2_hack_add_library target_name)
    add_library(${target_name}.private INTERFACE)
    target_include_directories(${target_name}.private
        INTERFACE
            ${CMAKE_CURRENT_SOURCE_DIR}
    )

    add_library(${target_name} ${ARGN})
    
    target_link_libraries(${target_name}
        PRIVATE
            ${target_name}.private
    )
    
    target_include_directories(${target_name}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    
    d2_hack_set_common_target_props(${target_name} ${ARGN})
endfunction()
