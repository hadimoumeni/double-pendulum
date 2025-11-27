find_package(SDL2 REQUIRED)

# New double pendulum main executable
add_executable(main 
    main.c
    physics.c
    visualizer.c
)
target_link_libraries(main SDL2::SDL2 m)
target_include_directories(main PRIVATE ../include)

# Keep the old library example if needed
# We are replacing the main target definition.

add_library(library library.c)
target_include_directories(library PUBLIC ../include)
