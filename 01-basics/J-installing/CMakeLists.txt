cmake_minimum_required(VERSION 3.16)

project(hello_install VERSION 1.0)

# --- Library ---
add_library(hello_library src/Hello.cpp)
target_include_directories(hello_library PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# --- Executable ---
add_executable(hello_binary src/main.cpp)
target_link_libraries(hello_binary PRIVATE hello_library)

# --- Install Rules ---
# Install the executable
install(TARGETS hello_binary
    RUNTIME DESTINATION bin
)

# Install the library
install(TARGETS hello_library
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Install headers
install(DIRECTORY include/
    DESTINATION include
)

message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
