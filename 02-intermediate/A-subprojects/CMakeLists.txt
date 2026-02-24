cmake_minimum_required(VERSION 3.16)

project(subprojects)

# Add the sub-library
add_subdirectory(libs/greeting)

# Create executable
add_executable(subprojects src/main.cpp)

# Link the sub-library
target_link_libraries(subprojects PRIVATE greeting)
