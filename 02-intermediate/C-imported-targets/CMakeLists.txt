cmake_minimum_required(VERSION 3.16)

project(imported_targets)

# --- Modern way: Imported Targets ---
# find_package creates imported targets like Threads::Threads
find_package(Threads REQUIRED)

add_executable(imported_targets src/main.cpp)

# Modern: Use imported target (recommended)
target_link_libraries(imported_targets PRIVATE Threads::Threads)

target_compile_features(imported_targets PRIVATE cxx_std_11)

# Show the difference:
message(STATUS "--- Legacy variables ---")
message(STATUS "CMAKE_THREAD_LIBS_INIT = ${CMAKE_THREAD_LIBS_INIT}")
message(STATUS "")
message(STATUS "--- Modern imported target ---")
message(STATUS "Threads::Threads is an IMPORTED target")
message(STATUS "It carries includes, flags, and link libraries automatically")
