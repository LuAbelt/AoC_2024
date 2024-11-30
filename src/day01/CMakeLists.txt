add_executable(day01 main.cpp)

add_custom_target( "Day01-Part-1-Sample"
        COMMAND day01 --one < ${CMAKE_CURRENT_SOURCE_DIR}/sample_input.txt
        DEPENDS day01
)

add_custom_target( "Day01-Part-2-Sample"
        COMMAND day01 --two < ${CMAKE_CURRENT_SOURCE_DIR}/sample_input.txt
        DEPENDS day01
)

add_custom_target( "Day01-Part-1"
        COMMAND day01 --one < ${CMAKE_CURRENT_SOURCE_DIR}/input.txt
        DEPENDS day01
)

add_custom_target( "Day01-Part-2"
        COMMAND day01 --two < ${CMAKE_CURRENT_SOURCE_DIR}/input.txt
        DEPENDS day01
)

target_link_libraries(day01 PRIVATE AoC_Lib)