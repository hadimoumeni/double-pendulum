find_package(SDL2 REQUIRED)

add_executable(main make.c)
target_link_libraries(main SDL2::SDL2 m) # Added 'm' for math library

add_library(library library.c)