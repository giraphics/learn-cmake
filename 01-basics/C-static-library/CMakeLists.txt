cmake_minimum_required(VERSION 3.16)

project(hello_library)

# Create a static library
add_library(hello_library STATIC src/Hello.cpp)

# Set include directories for the library
target_include_directories(hello_library PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Create an executable
add_executable(hello_binary src/main.cpp)

# Link the static library to the executable
target_link_libraries(hello_binary PRIVATE hello_library)
