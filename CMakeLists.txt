cmake_minimum_required(VERSION 3.22)
project(AoC_2024)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(external)
add_subdirectory(src)

add_executable(AoC_2024 main.cpp)

target_link_libraries(AoC_2024 PRIVATE AoC_Lib)