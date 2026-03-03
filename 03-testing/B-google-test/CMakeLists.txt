cmake_minimum_required(VERSION 3.16)

project(google_test_example)

# Enable testing
enable_testing()

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Include GoogleTest module for gtest_discover_tests
include(GoogleTest)

# Create the test executable
add_executable(calculator_tests src/test_calculator.cpp)

target_include_directories(calculator_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(calculator_tests PRIVATE
    GTest::gtest_main
)

# Discover tests automatically
gtest_discover_tests(calculator_tests)
