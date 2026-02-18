cmake_minimum_required(VERSION 3.16)

project(third_party)

add_executable(third_party src/main.cpp)

# --- Example 1: Finding Threads (available on all platforms) ---
find_package(Threads REQUIRED)
target_link_libraries(third_party PRIVATE Threads::Threads)
target_compile_definitions(third_party PRIVATE HAS_THREADS)

message(STATUS "Threads found: ${Threads_FOUND}")
message(STATUS "Thread library: ${CMAKE_THREAD_LIBS_INIT}")

# --- Example 2: Finding an optional package ---
find_package(ZLIB QUIET)
if(ZLIB_FOUND)
    message(STATUS "ZLIB found: ${ZLIB_VERSION_STRING}")
    message(STATUS "ZLIB includes: ${ZLIB_INCLUDE_DIRS}")
    target_compile_definitions(third_party PRIVATE HAS_ZLIB)
else()
    message(STATUS "ZLIB not found (optional -- skipping)")
endif()
