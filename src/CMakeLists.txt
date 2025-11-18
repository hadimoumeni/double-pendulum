find_package(SDL2 REQUIRED)

add_executable(main make.c)
target_link_libraries(main SDL2::SDL2) #linking the library with the correct executable

add_library(library library.c)