cmake_minimum_required(VERSION 3.16)

project(cpp_standard)

add_executable(cpp_standard src/main.cpp)

# Method 1 (PREFERRED): Per-target C++ standard
target_compile_features(cpp_standard PRIVATE cxx_std_17)

# Disable compiler extensions (e.g., -std=c++17 instead of -std=gnu++17)
set_target_properties(cpp_standard PROPERTIES
    CXX_EXTENSIONS OFF
)

# Print the C++ standard being used
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
