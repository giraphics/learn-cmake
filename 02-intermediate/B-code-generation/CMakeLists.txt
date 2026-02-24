cmake_minimum_required(VERSION 3.16)

project(code_generation VERSION 2.1.3)

# Generate version.h from template
configure_file(
    templates/version.h.in
    ${CMAKE_BINARY_DIR}/generated/version.h
    @ONLY
)

add_executable(code_generation src/main.cpp)

# Add the generated header to the include path
target_include_directories(code_generation PRIVATE
    ${CMAKE_BINARY_DIR}/generated
)
