cmake_minimum_required(VERSION 3.16)

project(build_ninja)

add_executable(build_ninja src/main.cpp)

target_compile_features(build_ninja PRIVATE cxx_std_11)

# Display the generator being used
message(STATUS "CMake Generator: ${CMAKE_GENERATOR}")
message(STATUS "Build tool: ${CMAKE_MAKE_PROGRAM}")
