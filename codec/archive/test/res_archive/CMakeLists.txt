d2_hack_add_test(d2_hack.codec.archive.test.res_archive
    test_res_archive.cpp
)
target_link_libraries(d2_hack.codec.archive.test.res_archive
    PRIVATE
        d2_hack.codec.archive
        d2_hack.codec.archive.private
)

cotire(d2_hack.codec.archive.test.res_archive)