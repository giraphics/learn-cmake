cmake_minimum_required(VERSION 3.16)

project(generator_expressions)

add_executable(genexpr src/main.cpp)

target_compile_features(genexpr PRIVATE cxx_std_17)

# --- Generator Expression Examples ---

# 1. Configuration-based definitions
target_compile_definitions(genexpr PRIVATE
    $<$<CONFIG:Debug>:IS_DEBUG>
    $<$<CONFIG:Release>:IS_RELEASE>
)

# 2. Compiler-specific flags
target_compile_options(genexpr PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra>
)

# 3. Platform-specific definitions
target_compile_definitions(genexpr PRIVATE
    $<$<PLATFORM_ID:Windows>:ON_WINDOWS>
    $<$<PLATFORM_ID:Linux>:ON_LINUX>
    $<$<PLATFORM_ID:Darwin>:ON_MACOS>
)

# 4. BUILD_INTERFACE vs INSTALL_INTERFACE
target_include_directories(genexpr PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
)

message(STATUS "Generator expressions are evaluated at build/generate time, not configure time!")
