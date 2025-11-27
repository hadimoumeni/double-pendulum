# src/CMakeLists.txt

# Gather all source files in this directory (main.c, pendulum.c, visualizer.c, arithmetic.c)
file(GLOB SOURCE_FILES "*.c")

# Define the executable
add_executable(double_pendulum_sim ${SOURCE_FILES})

# Link Libraries 
# Link the executable with the SDL2 libraries found in the parent scope
target_link_libraries(double_pendulum_sim PUBLIC ${SDL2_LIBRARIES} m)