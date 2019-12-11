d2_hack_add_test(d2_hack.codec.data.test.b3d_reader
    test_b3d_reader.cpp
)

target_link_libraries(d2_hack.codec.data.test.b3d_reader
    PRIVATE
        d2_hack.codec.data
        d2_hack.codec.data.private
)

target_compile_definitions(d2_hack.codec.data.test.b3d_reader
    PRIVATE
        -DD2_AA_B3D_FILENAME="${D2_ROOT_DIR_CMAKE_DIR}/ENV/aa.b3d"
)

cotire(d2_hack.codec.data.test.b3d_reader)
