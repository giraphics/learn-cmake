cmake_minimum_required(VERSION 3.16)

project(ctest_intro)

# Enable testing support
enable_testing()

# --- Test executables ---
add_executable(test_calculator src/test_calculator.cpp)
target_include_directories(test_calculator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(test_divide src/test_divide.cpp)
target_include_directories(test_divide PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# --- Register tests with CTest ---
add_test(NAME CalculatorTests COMMAND test_calculator)
add_test(NAME DivideTests COMMAND test_divide)

# Optional: Set test properties
set_tests_properties(CalculatorTests PROPERTIES
    TIMEOUT 10
    LABELS "unit;math"
)

set_tests_properties(DivideTests PROPERTIES
    TIMEOUT 10
    LABELS "unit;math"
)
