d2_hack_add_test(d2_hack.codec.archive.test.res_archive
    test_res_archive.cpp
)
target_link_libraries(d2_hack.codec.archive.test.res_archive
    PRIVATE
        d2_hack.codec.archive
        d2_hack.codec.archive.private
)

target_compile_definitions(d2_hack.codec.archive.test.res_archive
    PRIVATE
        -DD2_AA_RES_ARCHIVE_NAME="${D2_ROOT_DIR_CMAKE_DIR}/ENV/aa.res"
)

cotire(d2_hack.codec.archive.test.res_archive)