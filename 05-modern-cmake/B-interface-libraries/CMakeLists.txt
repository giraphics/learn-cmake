cmake_minimum_required(VERSION 3.16)

project(interface_libraries)

# --- Header-Only (Interface) Library ---
# INTERFACE = no compiled sources, only headers and properties
add_library(mathlib INTERFACE)

# INTERFACE include directories — passed to consumers
target_include_directories(mathlib INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# INTERFACE compile features — consumers must support C++14
target_compile_features(mathlib INTERFACE cxx_std_14)

# INTERFACE compile definitions — passed to consumers
target_compile_definitions(mathlib INTERFACE
    _USE_MATH_DEFINES  # For M_PI on MSVC
)

# --- Executable ---
add_executable(app src/main.cpp)

# Linking an INTERFACE library propagates all INTERFACE properties
target_link_libraries(app PRIVATE mathlib)
