cmake_minimum_required(VERSION 3.27)
project(z4 C)

set(CMAKE_C_STANDARD 11)
add_compile_options(-Wall -Wextra -Wuninitialized -pedantic -g)

add_executable(z4 src/data.c src/z4.c)
target_include_directories(z4 PRIVATE include)
target_link_libraries(z4 -lm)





