cmake_minimum_required(VERSION 3.16)

project(hello_shared)

# Create a shared library
add_library(hello_library SHARED src/Hello.cpp)

# Set include directories
target_include_directories(hello_library PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Create an executable
add_executable(hello_binary src/main.cpp)

# Link the shared library to the executable
target_link_libraries(hello_binary PRIVATE hello_library)
