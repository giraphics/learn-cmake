cmake_minimum_required(VERSION 3.16)

project(compile_flags)

add_executable(compile_flags src/main.cpp)

# Method 1: Per-target compile options (PREFERRED - Modern CMake)
target_compile_options(compile_flags PRIVATE
    # MSVC flags
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    # GCC/Clang flags
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

# Method 2: Adding compile definitions
target_compile_definitions(compile_flags PRIVATE
    APP_VERSION="1.0.0"
    $<$<CONFIG:Debug>:DEBUG_MODE>
)

message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Compiler version: ${CMAKE_CXX_COMPILER_VERSION}")
