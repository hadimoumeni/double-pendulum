# List of source files that make up the executable
set(SOURCE_FILES
    main.c
    pendulum.c
    sdl_visuals.c
    arithmetic.c
)

# Create the executable target called "main"
add_executable(main ${SOURCE_FILES})

# Where to find our header files (pendulum.h, arithmetic.h, sdl_visuals.h, etc.)
target_include_directories(main PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Link libraries to the "main" executable
target_link_libraries(main
    PRIVATE
        ${SDL2_LIBRARIES}
        m              # math library for sin/cos/sqrt/etc
)
