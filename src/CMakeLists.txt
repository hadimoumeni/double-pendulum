# List of source files that make up the executable
set(SOURCE_FILES
    main.c
    pendulum.c
    sdl_visuals.c
    arithmetic.c
    sha256.c
)


add_executable(main ${SOURCE_FILES})

target_include_directories(main PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

target_link_libraries(main
    PRIVATE
        ${SDL2_LIBRARIES}
        m              
)
