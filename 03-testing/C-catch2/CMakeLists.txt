cmake_minimum_required(VERSION 3.16)

project(catch2_example)

enable_testing()

# Fetch Catch2
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.5.2
)

FetchContent_MakeAvailable(Catch2)

# Append Catch2's CMake module path for catch_discover_tests
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)

# Create the test executable
add_executable(calculator_tests src/test_calculator.cpp)

target_include_directories(calculator_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(calculator_tests PRIVATE
    Catch2::Catch2WithMain
)

# Discover tests automatically
catch_discover_tests(calculator_tests)
