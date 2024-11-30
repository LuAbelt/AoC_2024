add_executable(day01 main.cpp)

add_custom_target( "Day01-Part-1-Sample"
        COMMAND day01 --one < ${PROJECT_SOURCE_DIR}/external/AoC-Inputs/2024/day01-sample
        DEPENDS day01
)

add_custom_target( "Day01-Part-2-Sample"
        COMMAND day01 --two < ${PROJECT_SOURCE_DIR}/external/AoC-Inputs/2024/day01-sample
        DEPENDS day01
)

add_custom_target( "Day01-Part-1"
        COMMAND day01 --one < ${PROJECT_SOURCE_DIR}/external/AoC-Inputs/2024/day01
        DEPENDS day01
)

add_custom_target( "Day01-Part-2"
        COMMAND day01 --two < ${PROJECT_SOURCE_DIR}/external/AoC-Inputs/2024/day01
        DEPENDS day01
)

target_link_libraries(day01 PRIVATE AoC_Lib)