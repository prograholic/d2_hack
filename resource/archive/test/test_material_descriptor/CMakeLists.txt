d2_hack_add_test(d2_hack.resource.archive.test.test_material_descriptor
    test_material_descriptor.cpp
)
target_link_libraries(d2_hack.resource.archive.test.test_material_descriptor
    PRIVATE
        d2_hack.resource
        d2_hack.resource.archive.private
)
