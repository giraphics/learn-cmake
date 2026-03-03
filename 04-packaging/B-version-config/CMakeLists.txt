cmake_minimum_required(VERSION 3.16)

project(MyLib VERSION 1.2.0)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# --- Library ---
add_library(mylib src/mylib.cpp)
add_library(MyLib::mylib ALIAS mylib)

target_include_directories(mylib PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(mylib PUBLIC cxx_std_11)

# --- Executable (for demo) ---
add_executable(myapp src/main.cpp)
target_link_libraries(myapp PRIVATE mylib)

# --- Install Targets ---
install(TARGETS mylib
    EXPORT MyLibTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(TARGETS myapp
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# --- Export Targets ---
install(EXPORT MyLibTargets
    FILE MyLibTargets.cmake
    NAMESPACE MyLib::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MyLib
)

# --- Package Config File ---
configure_package_config_file(
    cmake/MyLibConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/MyLibConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MyLib
)

# --- Version File ---
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/MyLibConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# --- Install Config Files ---
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/MyLibConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/MyLibConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MyLib
)
