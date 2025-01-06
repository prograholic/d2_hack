d2_hack_add_test(d2_hack.common.test.utils
    test_utils.cpp
)
target_link_libraries(d2_hack.common.test.utils
    PRIVATE
        d2_hack.common
)
