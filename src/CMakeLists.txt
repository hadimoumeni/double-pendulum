# Define the source files for the main simulation
# We assume main.c is in this folder along with the modules
add_executable(main
    main.c
    arithmetic.c
    pendulum.c
    sdl_visuals.c
)

# Link the libraries: SDL2 for graphics, 'm' for math
target_link_libraries(main ${SDL2_LIBRARIES} m)