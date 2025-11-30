
# Explicitly list source files for better control (Good code hygiene over GLOB)
set(SOURCE_FILES
    main.c
    pendulum.c
    sdl_visuals.c
    arithmetic.c
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
