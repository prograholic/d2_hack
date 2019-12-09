add_executable(test_d2_b3d_reader
    test_d2_b3d_reader.cpp
)

target_link_libraries(test_d2_b3d_reader
    PRIVATE
        d2_hack_resource_loader
)

target_compile_definitions(test_d2_b3d_reader
    PRIVATE
        -DD2_AA_B3D_FILENAME="${D2_ROOT_DIR_CMAKE_DIR}/ENV/aa.b3d"
)

add_test(
    NAME
        test_d2_b3d_reader
    COMMAND
        test_d2_b3d_reader
    WORKING_DIRECTORY
        ${APP_ROOT_DIRECTORY}
)
