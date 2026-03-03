cmake_minimum_required(VERSION 3.16)

project(fetchcontent_example)

include(FetchContent)

# --- Dependency 1: fmt (formatting library) ---
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG        10.2.1
)

# --- Dependency 2: nlohmann/json (JSON library) ---
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
)

# Download and make available (can batch multiple)
FetchContent_MakeAvailable(fmt json)

# --- Executable ---
add_executable(app src/main.cpp)

target_link_libraries(app PRIVATE
    fmt::fmt
    nlohmann_json::nlohmann_json
)

target_compile_features(app PRIVATE cxx_std_17)

# Show where dependencies were downloaded
FetchContent_GetProperties(fmt SOURCE_DIR fmt_SOURCE)
FetchContent_GetProperties(json SOURCE_DIR json_SOURCE)
message(STATUS "fmt source dir: ${fmt_SOURCE}")
message(STATUS "json source dir: ${json_SOURCE}")
