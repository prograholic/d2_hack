d2_hack_add_test(d2_hack.resource.archive.test.res_archive
    test_res_archive.cpp
)
target_link_libraries(d2_hack.resource.archive.test.res_archive
    PRIVATE
        d2_hack.resource
        d2_hack.resource.archive.private
)
