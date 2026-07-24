cmake_minimum_required(VERSION 3.14)
project(vitalcore VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Compiler warnings
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

# --- Library ---
add_library(vitalcore STATIC
    src/vitals.c
    src/thresholds.c
    src/anomaly.c
    src/risk.c
    src/alert.c
    src/vitalcore.c
)

target_include_directories(vitalcore
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(vitalcore PRIVATE m)

# Shared library option
option(BUILD_SHARED_LIBS "Build shared library" ON)
if(BUILD_SHARED_LIBS)
    add_library(vitalcore_shared SHARED
        src/vitals.c
        src/thresholds.c
        src/anomaly.c
        src/risk.c
        src/alert.c
        src/vitalcore.c
    )
    target_include_directories(vitalcore_shared
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )
    target_link_libraries(vitalcore_shared PRIVATE m)
    set_target_properties(vitalcore_shared PROPERTIES
        OUTPUT_NAME vitalcore
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
    )
endif()

# --- Tests ---
enable_testing()
option(BUILD_TESTS "Build tests" ON)

if(BUILD_TESTS)
    # Test framework (minimal)
    add_library(test_framework STATIC tests/test_framework.c)
    target_include_directories(test_framework PRIVATE include tests)

    # Individual test files
    set(TEST_SOURCES
        tests/test_vitals.c
        tests/test_thresholds.c
        tests/test_anomaly.c
        tests/test_risk.c
        tests/test_alert.c
        tests/test_integration.c
    )

    foreach(test_src ${TEST_SOURCES})
        get_filename_component(test_name ${test_src} NAME_WE)
        add_executable(${test_name} ${test_src} tests/test_framework.c)
        target_include_directories(${test_name} PRIVATE include)
        target_link_libraries(${test_name} PRIVATE vitalcore m)
        add_test(NAME ${test_name} COMMAND ${test_name})
    endforeach()
endif()

# --- Example ---
option(BUILD_EXAMPLES "Build examples" ON)
if(BUILD_EXAMPLES)
    add_executable(basic_monitor examples/basic_monitor.c)
    target_include_directories(basic_monitor PRIVATE include)
    target_link_libraries(basic_monitor PRIVATE vitalcore m)
endif()

# --- Install ---
install(TARGETS vitalcore
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)
install(DIRECTORY include/vitalcore DESTINATION include)

# Print summary
message(STATUS "VitalCore v${PROJECT_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Shared lib: ${BUILD_SHARED_LIBS}")
message(STATUS "  Tests: ${BUILD_TESTS}")
message(STATUS "  Examples: ${BUILD_EXAMPLES}")
