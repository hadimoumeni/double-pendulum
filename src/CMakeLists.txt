# src/CMakeLists.txt

# Single executable built from your three source files
add_executable(main
    main.c
    sdl_visuals.c
    renderer.c
)

# Your own headers
target_include_directories(main PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# Add the system include directory so SDL2/SDL.h is visible
target_include_directories(main PRIVATE
    /usr/include
)

# Link against system SDL2 library
target_link_libraries(main PRIVATE SDL2)
