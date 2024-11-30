cmake_minimum_required(VERSION 3.29)
project(AoC_2024)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(external/AoC_Lib)

add_executable(AoC_2024 main.cpp)

target_link_libraries(AoC_2024 LINK_PUBLIC AoC)