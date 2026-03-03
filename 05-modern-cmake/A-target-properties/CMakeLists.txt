cmake_minimum_required(VERSION 3.16)

project(target_properties VERSION 1.0)

# --- The Modern Way: Everything is target-based ---

# Create a library target
add_library(greeter src/greeter.cpp)

# PUBLIC: consumers of this library also get these include dirs
target_include_directories(greeter PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Set C++ standard as a target property
target_compile_features(greeter PUBLIC cxx_std_17)

# Add compile options per-target (not globally!)
target_compile_options(greeter PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra>
)

# Add compile definitions per-target
target_compile_definitions(greeter PUBLIC
    GREETER_VERSION="${PROJECT_VERSION}"
)

# --- Executable ---
add_executable(app src/main.cpp)

# When we link greeter, we automatically get:
#   - Include directories (PUBLIC)
#   - Compile features/C++ standard (PUBLIC)
#   - Compile definitions (PUBLIC)
# This is the power of target-based CMake!
target_link_libraries(app PRIVATE greeter)

# --- Display target properties ---
get_target_property(GREETER_INCLUDES greeter INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(GREETER_FEATURES greeter INTERFACE_COMPILE_FEATURES)
message(STATUS "greeter includes: ${GREETER_INCLUDES}")
message(STATUS "greeter features: ${GREETER_FEATURES}")
