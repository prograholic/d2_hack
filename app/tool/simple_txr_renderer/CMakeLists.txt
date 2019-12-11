d2_hack_add_executable(d2_hack.app.simple_txr_renderer
    simple_txr_renderer.cpp
    simple_txr_renderer.h
    
    main.cpp
)

target_link_libraries(d2_hack.app.simple_txr_renderer
    PRIVATE
        d2_hack.app
)

cotire(d2_hack.app.simple_txr_renderer)
