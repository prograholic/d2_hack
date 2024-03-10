function(d2_hack_set_common_target_props target_name)
    message(STATUS "Processing target ${target_name} ...")
    
    set_target_properties(${target_name}
        PROPERTIES
            COTIRE_PREFIX_HEADER_INCLUDE_PATH "${Boost_INCLUDE_DIR};${OGRE_DIR}../include/OGRE"
    )
    
    if(NOT DEFINED D2_ROOT_DIR)
        message(FATAL_ERROR "D2_ROOT_DIR is not set. Should point to root directory of D2 installation")
    endif()
    file(TO_CMAKE_PATH ${D2_ROOT_DIR} D2_ROOT_DIR_CMAKE_DIR)
    
    target_compile_definitions(${target_name}
        PRIVATE
            -DD2_ROOT_DIR="${D2_ROOT_DIR_CMAKE_DIR}"
    )
    
    get_target_property(target_source_dir ${target_name} SOURCE_DIR)
    file(RELATIVE_PATH rel_path ${CMAKE_SOURCE_DIR} ${target_source_dir})
    
    set_target_properties(${target_name}
        PROPERTIES
            FOLDER "${rel_path}"
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
    
    add_library(${target_name}.public_headers INTERFACE)
    target_include_directories(${target_name}.public_headers
        INTERFACE
            ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    add_library(${target_name} ${ARGN})
    
    target_link_libraries(${target_name}
        PRIVATE
            ${target_name}.private
        PUBLIC
            ${target_name}.public_headers
    )
    
    d2_hack_set_common_target_props(${target_name} ${ARGN})
endfunction()
