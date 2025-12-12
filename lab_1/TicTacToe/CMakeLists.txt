cmake_minimum_required(VERSION 3.10)
project(TicTacToe VERSION 1.0 DESCRIPTION "TicTacToe Game with Tests")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_COVERAGE "Enable code coverage" OFF)
option(BUILD_TESTS "Build tests" ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCES
    src/TicTacToe.cpp
)

set(HEADERS
    include/TicTacToe.h
)

# Основной исполняемый файл
add_executable(TicTacToe src/main.cpp ${SOURCES} ${HEADERS})
target_include_directories(TicTacToe PRIVATE ${CMAKE_SOURCE_DIR}/include)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(TicTacToe PRIVATE DEBUG)
    target_compile_options(TicTacToe PRIVATE -g -O0 -Wall -Wextra -Wpedantic)
else()
    target_compile_options(TicTacToe PRIVATE -O2)
endif()

if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(TicTacToe PRIVATE --coverage -fprofile-arcs -ftest-coverage)
        target_link_options(TicTacToe PRIVATE --coverage)
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
        
        add_executable(test_tictactoe tests/test_tictactoe.cpp ${SOURCES} ${HEADERS})
        
        target_include_directories(test_tictactoe PRIVATE 
            ${CMAKE_SOURCE_DIR}/include
        )
        
        target_link_libraries(test_tictactoe 
            GTest::GTest
            GTest::Main
            pthread
        )
        
        if(ENABLE_COVERAGE)
            if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
                target_compile_options(test_tictactoe PRIVATE --coverage -fprofile-arcs -ftest-coverage)
                target_link_options(test_tictactoe PRIVATE --coverage)
                
                # Цель для lcov (HTML отчет)
                add_custom_target(coverage
                    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/coverage
                    COMMAND lcov --directory . --zerocounters
                    COMMAND ${CMAKE_BINARY_DIR}/bin/test_tictactoe
                    COMMAND lcov --directory . --capture --output-file ${CMAKE_BINARY_DIR}/coverage/coverage.info
                    COMMAND lcov --remove ${CMAKE_BINARY_DIR}/coverage/coverage.info '/usr/*' '*/tests/*' --output-file ${CMAKE_BINARY_DIR}/coverage/coverage_filtered.info
                    COMMAND genhtml ${CMAKE_BINARY_DIR}/coverage/coverage_filtered.info --output-directory ${CMAKE_BINARY_DIR}/coverage/html
                    COMMAND echo "Coverage report: ${CMAKE_BINARY_DIR}/coverage/html/index.html"
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                    DEPENDS test_tictactoe
                    COMMENT "Generating code coverage report with lcov"
                )
                
                # Цель для gcovr (вывод в терминал)
                add_custom_target(test-coverage
                    COMMAND ${CMAKE_COMMAND} -E echo "Running tests..."
                    COMMAND ${CMAKE_BINARY_DIR}/bin/test_tictactoe
                    COMMAND ${CMAKE_COMMAND} -E echo ""
                    COMMAND ${CMAKE_COMMAND} -E echo "========================================="
                    COMMAND ${CMAKE_COMMAND} -E echo "  Code Coverage Summary"
                    COMMAND ${CMAKE_COMMAND} -E echo "========================================="
                    COMMAND gcovr -r ${CMAKE_SOURCE_DIR}
                            --filter '${CMAKE_SOURCE_DIR}/src/'
                            --filter '${CMAKE_SOURCE_DIR}/include/'
                            --exclude '${CMAKE_SOURCE_DIR}/tests/'
                            --print-summary
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                    DEPENDS test_tictactoe
                    COMMENT "Running tests and showing coverage in terminal"
                )
                
                # Цель для детального отчета в терминал
                add_custom_target(test-coverage-detailed
                    COMMAND ${CMAKE_BINARY_DIR}/bin/test_tictactoe
                    COMMAND ${CMAKE_COMMAND} -E echo ""
                    COMMAND ${CMAKE_COMMAND} -E echo "========================================="
                    COMMAND ${CMAKE_COMMAND} -E echo "  Detailed Coverage Report"
                    COMMAND ${CMAKE_COMMAND} -E echo "========================================="
                    COMMAND gcovr -r ${CMAKE_SOURCE_DIR}
                            --filter '${CMAKE_SOURCE_DIR}/src/'
                            --filter '${CMAKE_SOURCE_DIR}/include/'
                            --exclude '${CMAKE_SOURCE_DIR}/tests/'
                            -s
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                    DEPENDS test_tictactoe
                    COMMENT "Detailed coverage report in terminal"
                )
            endif()
        endif()
        
        include(GoogleTest)
        gtest_discover_tests(test_tictactoe)
        
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
    set(DOXYGEN_PROJECT_NAME "TicTacToe Game")
    set(DOXYGEN_PROJECT_BRIEF "C++ реализация игры в крестики-нолики")
    set(DOXYGEN_PROJECT_NUMBER ${PROJECT_VERSION})
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
    
    # Генерация
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_LATEX NO)
    set(DOXYGEN_GENERATE_MAN NO)
    
    # Извлечение
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_EXTRACT_PRIVATE YES)
    set(DOXYGEN_EXTRACT_STATIC YES)
    
    # Настройки отображения
    set(DOXYGEN_SOURCE_BROWSER YES)
    set(DOXYGEN_INLINE_SOURCES NO)
    set(DOXYGEN_STRIP_CODE_COMMENTS NO)
    
    # Графики (если dot установлен)
    set(DOXYGEN_HAVE_DOT ${DOXYGEN_dot_FOUND})
    set(DOXYGEN_UML_LOOK YES)
    set(DOXYGEN_CLASS_GRAPH YES)
    set(DOXYGEN_COLLABORATION_GRAPH YES)
    set(DOXYGEN_CALL_GRAPH YES)
    
    # Настройки файлов
    set(DOXYGEN_FILE_PATTERNS "*.h *.cpp")
    set(DOXYGEN_RECURSIVE YES)
    
    # Интерфейс
    set(DOXYGEN_HTML_COLORSTYLE "LIGHT")
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    
    # Кодировка
    set(DOXYGEN_DOXYFILE_ENCODING "UTF-8")
    set(DOXYGEN_INPUT_ENCODING "UTF-8")
    
    doxygen_add_docs(
        doc
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
        COMMENT "Генерация документации с помощью Doxygen"
    )
    
    message(STATUS "Documentation target 'doc' created. Run: cmake --build . --target doc")
else()
    if(BUILD_DOCUMENTATION AND NOT DOXYGEN_FOUND)
        message(WARNING "Doxygen not found. Documentation will not be built.")
    endif()
endif()

# ============================================================
# Информация о проекте
# ============================================================

message(STATUS "")
message(STATUS "=== ${PROJECT_NAME} Configuration ===")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Build Tests: ${BUILD_TESTS}")
message(STATUS "Enable Coverage: ${ENABLE_COVERAGE}")
message(STATUS "Build Documentation: ${BUILD_DOCUMENTATION}")
if(GTest_FOUND)
    message(STATUS "GTest: Found")
else()
    message(STATUS "GTest: Not Found")
endif()
if(DOXYGEN_FOUND)
    message(STATUS "Doxygen: Found")
else()
    message(STATUS "Doxygen: Not Found")
endif()
message(STATUS "======================================")
message(STATUS "")
