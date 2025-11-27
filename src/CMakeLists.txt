
add_library(arithmetic STATIC
    arithmetic.c
)

target_include_directories(arithmetic
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(arithmetic
    PUBLIC m
)

add_executable(main
    main.c
    sdl_visuals.c
    renderer.c
)

target_include_directories(main PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_include_directories(main PRIVATE
    /usr/include
)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(main
    PRIVATE
        ${SDL2_LIBRARIES}
        arithmetic
)
