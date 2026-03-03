# C: FetchContent -- Downloading Dependencies at Configure Time

Managing third-party dependencies is one of the hardest problems in C++. The `FetchContent` module, introduced in CMake 3.11 and improved in 3.14+, provides a built-in way to download, configure, and integrate external projects directly into your build. This example fetches two popular libraries -- **fmt** (formatting) and **nlohmann/json** (JSON) -- from GitHub and uses them as if they were local targets.

## Concepts

- **`FetchContent` module**: A CMake module that downloads external content (Git repos, archives, URLs) at configure time and makes their targets available to your project.
- **`FetchContent_Declare`**: Declares a dependency with its download method and version. This does not download anything yet -- it only registers the dependency.
- **`FetchContent_MakeAvailable`**: Downloads the declared dependencies (if not already present) and calls `add_subdirectory` on them, making their targets available. Multiple dependencies can be passed in a single call.
- **`GIT_REPOSITORY` / `GIT_TAG`**: Specifies a Git repository URL and the exact tag, branch, or commit hash to check out. Always pin to a specific tag or commit for reproducible builds.
- **`FETCHCONTENT_BASE_DIR`**: The cache variable controlling where downloaded content is stored. Defaults to `${CMAKE_BINARY_DIR}/_deps`. You can set this to a shared directory to avoid re-downloading across multiple build directories.
- **FetchContent vs. ExternalProject**:
  - `FetchContent` runs at **configure time** -- targets are immediately available for `target_link_libraries`.
  - `ExternalProject_Add` runs at **build time** -- the external project is built as a separate step, and its targets are not directly available. This makes it harder to integrate but useful for projects that cannot be added via `add_subdirectory`.
- **`FETCHCONTENT_FULLY_DISCONNECTED`**: When set to `ON`, FetchContent will not attempt any downloads. This is useful for offline builds or CI environments where dependencies have been pre-fetched.

## File Structure

```
C-fetchcontent/
  CMakeLists.txt
  build.sh
  build.bat
  src/
    main.cpp
```

Note the simplicity: no `include/` directory is needed because all headers come from the fetched dependencies.

## Code Walkthrough

### CMakeLists.txt

**Including the module:**

```cmake
include(FetchContent)
```

This loads the FetchContent functions into scope.

**Declaring dependencies:**

```cmake
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG        10.2.1
)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
)
```

Each call registers a dependency name with its source. The `GIT_TAG` pins the exact version. Using a tag like `10.2.1` rather than a branch like `master` ensures reproducible builds.

**Making dependencies available:**

```cmake
FetchContent_MakeAvailable(fmt json)
```

This single call handles both dependencies: it downloads them (on first configure), adds their subdirectories, and makes all of their CMake targets available. Subsequent configures reuse the cached downloads.

**Linking against fetched targets:**

```cmake
add_executable(app src/main.cpp)

target_link_libraries(app PRIVATE
    fmt::fmt
    nlohmann_json::nlohmann_json
)
```

The fetched libraries provide namespaced targets (`fmt::fmt`, `nlohmann_json::nlohmann_json`). These carry all necessary include directories, compile features, and link dependencies -- the same target-based approach from examples A and B.

**Inspecting download locations:**

```cmake
FetchContent_GetProperties(fmt SOURCE_DIR fmt_SOURCE)
message(STATUS "fmt source dir: ${fmt_SOURCE}")
```

This prints where each dependency was downloaded, useful for debugging.

### Source File (main.cpp)

The application uses both libraries:
- **nlohmann/json** to create and serialize a JSON object.
- **fmt** to format and print strings using Python-style format syntax.

The `#include` paths (`<nlohmann/json.hpp>` and `<fmt/core.h>`) work automatically because the fetched targets set up the correct include directories.

## Building

**Linux / macOS:**
```bash
chmod +x build.sh
./build.sh
```

**Windows:**
```batch
build.bat
```

**Important**: The first build requires an internet connection and will take longer as it downloads the dependencies. Subsequent builds reuse the cached sources in the `build/_deps/` directory.

**Manual build:**
```bash
mkdir build && cd build
cmake ..
cmake --build .
./app        # Linux/macOS
Debug\app.exe  # Windows (MSVC)
```

## Expected Output

```
=== FetchContent Example ===

Person: {
  "level": "intermediate",
  "name": "CMake Learner",
  "topics": [
    "FetchContent",
    "dependencies",
    "modern CMake"
  ]
}
Name: CMake Learner
Level: intermediate

Topics:
  - FetchContent
  - dependencies
  - modern CMake
```

## Key Takeaways

1. **FetchContent integrates dependencies at configure time.** Unlike ExternalProject, fetched targets are immediately available for `target_link_libraries`, making integration seamless.
2. **Always pin versions with `GIT_TAG`.** Use a specific release tag or commit hash, never a branch name. This ensures reproducible builds and prevents unexpected breakage.
3. **`FetchContent_MakeAvailable` is the modern approach.** It replaced the older pattern of `FetchContent_Populate` + manual `add_subdirectory`. Use it unless you need fine-grained control over the population step.
4. **Namespaced targets are the standard.** Well-maintained libraries provide targets like `fmt::fmt` rather than plain `fmt`. If a target does not exist, the library may need additional configuration.
5. **Consider caching for CI.** Set `FETCHCONTENT_BASE_DIR` to a persistent location or use `FETCHCONTENT_FULLY_DISCONNECTED=ON` with pre-fetched sources to speed up CI builds and enable offline builds.
6. **FetchContent adds to your build.** Unlike a pre-installed system library, fetched sources are compiled as part of your project. This increases build time but guarantees version consistency.

## Exercises

1. Add a third dependency (e.g., [spdlog](https://github.com/gabime/spdlog)) using FetchContent and use it in `main.cpp`.
2. Set `FETCHCONTENT_FULLY_DISCONNECTED` to `ON` after the first build and verify that the project still configures without network access.
3. Change `GIT_TAG` to a branch name (e.g., `master`) and observe how CMake re-checks the repository on every configure. Understand why this is undesirable for reproducible builds.
4. Use `FetchContent_Declare` with a `URL` and `URL_HASH` instead of `GIT_REPOSITORY` to fetch a release archive. Compare the configure-time performance.
5. Set `FETCHCONTENT_BASE_DIR` to a directory outside the build tree (e.g., `~/.cmake-deps`). Create a second build directory and observe that it reuses the already-downloaded sources.
