d2_hack_add_test(d2_hack.resource.data.test.b3d_reader
    test_b3d_reader.cpp
)

target_link_libraries(d2_hack.resource.data.test.b3d_reader
    PRIVATE
        d2_hack.resource.data
        d2_hack.resource.data.private
)

cotire(d2_hack.resource.data.test.b3d_reader)
