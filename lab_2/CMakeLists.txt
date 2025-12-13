cmake_minimum_required(VERSION 3.14)
project(ComputerSimulation VERSION 1.0)
option(ENABLE_COVERAGE "Enable code coverage" ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

if(ENABLE_COVERAGE)
    add_compile_options(--coverage -fprofile-arcs -ftest-coverage)
    add_link_options(--coverage)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

# ═══════════════════════════════════════════════════════════════════
# ИСХОДНИКИ ПРОЕКТА
# ═══════════════════════════════════════════════════════════════════

file(GLOB_RECURSE PROJECT_SOURCES 
    "src/cpu/*.cpp"
    "src/memory/*.cpp"
    "src/storage/*.cpp"
    "src/graphics/*.cpp"
    "src/motherboard/*.cpp"
    "src/power/*.cpp"
    "src/cooling/*.cpp"
    "src/network/*.cpp"
    "src/peripherals/*.cpp"
    "src/audio/*.cpp"
    "src/case/*.cpp"
    "src/os/*.cpp"
    "src/Computer.cpp"
)

# ═══════════════════════════════════════════════════════════════════
# ОСНОВНАЯ ПРОГРАММА
# ═══════════════════════════════════════════════════════════════════

add_executable(computer_simulation 
    src/main.cpp
    ${PROJECT_SOURCES}
)

set_target_properties(computer_simulation PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# ═══════════════════════════════════════════════════════════════════
# ТЕСТЫ
# ═══════════════════════════════════════════════════════════════════

option(BUILD_TESTS "Build tests" ON)

if(BUILD_TESTS)
    enable_testing()
    
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )
    
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    
    # ТОЛЬКО test_main.cpp!
    # Он сам включает test_*.cpp через #include
    add_executable(run_tests 
        tests/test_main.cpp      # ← ТОЛЬКО ЭТОТ ФАЙЛ
        ${PROJECT_SOURCES}       # ← Исходники проекта (src/)
    )
    
    target_link_libraries(run_tests
        GTest::gtest
        GTest::gtest_main
        pthread
    )
    
    target_include_directories(run_tests PRIVATE 
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/tests    # ← Чтобы найти test_*.cpp
    )
    
    set_target_properties(run_tests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
    )
    
    include(GoogleTest)
    gtest_discover_tests(run_tests)
    
    message(STATUS "✓ Tests enabled")
endif()

message(STATUS "════════════════════════════════════════════════════")
message(STATUS "  Project: ${PROJECT_NAME}")
message(STATUS "  Main: bin/computer_simulation")
if(BUILD_TESTS)
    message(STATUS "  Tests: tests/run_tests")
endif()
message(STATUS "════════════════════════════════════════════════════")

