d2_hack_add_executable(d2_hack.app.simple_b3d_car_renderer
    simple_b3d_car_renderer.cpp
    simple_b3d_car_renderer.h

    main.cpp
)

target_link_libraries(d2_hack.app.simple_b3d_car_renderer
    PRIVATE
        d2_hack.app
)
