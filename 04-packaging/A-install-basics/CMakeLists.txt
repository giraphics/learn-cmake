cmake_minimum_required(VERSION 3.16)

project(mathlib_package VERSION 1.0.0)

# --- Library ---
add_library(mathlib src/mathlib.cpp)
target_include_directories(mathlib PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# --- Executable ---
add_executable(mathapp src/main.cpp)
target_link_libraries(mathapp PRIVATE mathlib)

# --- Install Rules ---
install(TARGETS mathapp
    RUNTIME DESTINATION bin
)

install(TARGETS mathlib
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/
    DESTINATION include
)

# --- CPack Configuration ---
set(CPACK_PACKAGE_NAME "MathLib")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A simple math library example")
set(CPACK_PACKAGE_VENDOR "Learn CMake Tutorial")
set(CPACK_PACKAGE_CONTACT "learner@example.com")

# Generator-specific settings
set(CPACK_GENERATOR "ZIP;TGZ")

# For DEB packages (Linux)
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Learn CMake")

# For RPM packages (Linux)
set(CPACK_RPM_PACKAGE_LICENSE "MIT")

# Include the CPack module (must be AFTER setting variables)
include(CPack)
