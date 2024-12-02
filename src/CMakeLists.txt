foreach (day_num RANGE 01 25)
    set(day_str ${day_num})
    if (${day_num} LESS 10)
        set(day_str "0${day_num}")
    endif ()

    if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/day${day_str})
        file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/day${day_str})
    endif ()

    if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/day${day_str}/main.cpp)
    else ()
        file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp.template/ DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/day${day_str}/main.cpp)
    endif ()

    add_executable(day${day_str} ${CMAKE_CURRENT_SOURCE_DIR}/day${day_str}/main.cpp)

    add_custom_target("Day${day_str}-Part-1-Sample"
            COMMAND day${day_str} --one < ${PROJECT_SOURCE_DIR}/external/AoC-Inputs/2024/day${day_str}-sample
            DEPENDS day${day_str}
    )

    add_custom_target("Day${day_str}-Part-2-Sample"
            COMMAND day${day_str} --two < ${PROJECT_SOURCE_DIR}/external/AoC-Inputs/2024/day${day_str}-sample
            DEPENDS day${day_str}
    )

    add_custom_target("Day${day_str}-Part-1"
            COMMAND day${day_str} --one < ${PROJECT_SOURCE_DIR}/external/AoC-Inputs/2024/day${day_str}
            DEPENDS day${day_str}
    )

    add_custom_target("Day${day_str}-Part-2"
            COMMAND day${day_str} --two < ${PROJECT_SOURCE_DIR}/external/AoC-Inputs/2024/day${day_str}
            DEPENDS day${day_str}
    )

    target_link_libraries(day${day_str} PRIVATE AoC_Lib)
endforeach ()