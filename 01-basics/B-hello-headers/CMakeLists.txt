cmake_minimum_required(VERSION 3.16)

project(hello_headers)

# Create a sources variable with all .cpp files
set(SOURCES
    src/main.cpp
    src/Hello.cpp
)

# Add an executable with the above sources
add_executable(hello_headers ${SOURCES})

# Set the include directories for this target
target_include_directories(hello_headers PRIVATE ${PROJECT_SOURCE_DIR}/include)
