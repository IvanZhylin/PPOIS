cmake_minimum_required(VERSION 3.10)
project(Sets VERSION 1.0 DESCRIPTION "Sets with Tests")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_COVERAGE "Enable code coverage" OFF)
option(BUILD_TESTS "Build tests" ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCES
    src/Set.cpp
)

set(HEADERS
    include/Set.h
)

# Основной исполняемый файл
add_executable(Sets src/main.cpp ${SOURCES} ${HEADERS})
target_include_directories(Sets PRIVATE ${CMAKE_SOURCE_DIR}/include)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Sets PRIVATE DEBUG)
    target_compile_options(Sets PRIVATE -g -O0 -Wall -Wextra -Wpedantic)
else()
    target_compile_options(Sets PRIVATE -O2)
endif()

if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(Sets PRIVATE --coverage -fprofile-arcs -ftest-coverage)
        target_link_options(Sets PRIVATE --coverage)
    endif()
endif()

# ============================================================
# Google Test - используем системную версию
# ============================================================

if(BUILD_TESTS)
    find_package(GTest QUIET)
    
    if(GTest_FOUND)
        message(STATUS "Found system GTest")
        enable_testing()
        
        add_executable(test_sets tests/test.cpp ${SOURCES} ${HEADERS})
        
        target_include_directories(test_sets PRIVATE 
            ${CMAKE_SOURCE_DIR}/include
        )
        
        target_link_libraries(test_sets 
            GTest::GTest
            GTest::Main
            pthread
        )
        
        if(ENABLE_COVERAGE)
            if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
                target_compile_options(test_sets PRIVATE --coverage -fprofile-arcs -ftest-coverage)
                target_link_options(test_sets PRIVATE --coverage)
                
                add_custom_target(coverage
                    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/coverage
                    COMMAND lcov --directory . --zerocounters
                    COMMAND ${CMAKE_BINARY_DIR}/bin/test_sets
                    COMMAND lcov --directory . --capture --output-file ${CMAKE_BINARY_DIR}/coverage/coverage.info
                    COMMAND lcov --remove ${CMAKE_BINARY_DIR}/coverage/coverage.info '/usr/*' '*/tests/*' --output-file ${CMAKE_BINARY_DIR}/coverage/coverage_filtered.info
                    COMMAND genhtml ${CMAKE_BINARY_DIR}/coverage/coverage_filtered.info --output-directory ${CMAKE_BINARY_DIR}/coverage/html
                    COMMAND echo "Coverage report: ${CMAKE_BINARY_DIR}/coverage/html/index.html"
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                    DEPENDS test_sets
                    COMMENT "Generating code coverage report"
                )
            endif()
        endif()
        
        include(GoogleTest)
        gtest_discover_tests(test_sets)
        
    else()
        message(WARNING "GTest not found. Tests will not be built. Install with: sudo pacman -S gtest")
    endif()
endif()

# ============================================================
# Doxygen
# ============================================================

option(BUILD_DOCUMENTATION "Create HTML documentation" ON)

find_package(Doxygen OPTIONAL_COMPONENTS dot)

if(DOXYGEN_FOUND AND BUILD_DOCUMENTATION)
    set(DOXYGEN_PROJECT_NAME "Sets Library")
    set(DOXYGEN_PROJECT_BRIEF "C++ библиотека для работы с математическими множествами")
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_LATEX NO)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_SOURCE_BROWSER YES)
    
    doxygen_add_docs(
        doc
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
        COMMENT "Генерация документации"
    )
endif()
