# src/CMakeLists.txt

# Explicitly list source files for better control (Good code hygiene over GLOB)
set(SOURCE_FILES
    main.c
    pendulum.c
    sdl_visuals.c
    arithmetic.c
)

# Define the executable
add_executable(double_pendulum_sim ${SOURCE_FILES})

# Link Libraries 
# Link the executable with SDL2 and the math library 'm'
target_link_libraries(double_pendulum_sim 
    PRIVATE 
        ${SDL2_LIBRARIES} 
        m
)