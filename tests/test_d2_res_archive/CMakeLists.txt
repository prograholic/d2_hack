add_executable(test_d2_res_archive
    test_d2_res_archive.cpp
)

target_link_libraries(test_d2_res_archive
    PRIVATE
        d2_hack_resource_loader
)

target_compile_definitions(test_d2_res_archive
    PRIVATE
        -DD2_AA_RES_ARCHIVE_NAME="${D2_ROOT_DIR_CMAKE_DIR}/ENV/aa.res"
)

add_test(
    NAME
        test_d2_res_archive
    COMMAND
        test_d2_res_archive
    WORKING_DIRECTORY
        ${APP_ROOT_DIRECTORY}
)
