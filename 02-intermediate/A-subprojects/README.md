# A-subprojects: Multi-Directory Projects with add_subdirectory()

As projects grow, keeping all source code in a single directory becomes unmanageable. CMake's `add_subdirectory()` command lets you organize code into separate libraries and components, each with its own `CMakeLists.txt`. This example demonstrates how to structure a project with an internal sub-library and how include paths propagate automatically through target properties.

## Concepts

- **add_subdirectory()**: Tells CMake to descend into a subdirectory and process its `CMakeLists.txt`. The subdirectory gets its own scope for variables but shares the same build system.
- **PUBLIC include directories**: When a library declares its include directories as `PUBLIC`, any target that links against it automatically inherits those include paths. This is the key mechanism that makes multi-directory projects work seamlessly.
- **Project organization**: A common pattern is to place internal libraries under a `libs/` directory, each with their own `CMakeLists.txt`, source files, and a public include directory.
- **Target-based dependencies**: By using `target_link_libraries()`, CMake handles not only linking but also propagation of compile flags, include directories, and other properties.

## File Structure

```
A-subprojects/
├── CMakeLists.txt              # Root CMakeLists — ties everything together
├── README.md
├── build.sh
├── build.bat
├── src/
│   └── main.cpp                # Application entry point
└── libs/
    └── greeting/
        ├── CMakeLists.txt      # Library-level CMakeLists
        ├── src/
        │   └── greeting.cpp    # Library implementation
        └── include/
            └── greeting/
                └── greeting.h  # Public header (namespaced under greeting/)
```

The `include/greeting/greeting.h` path follows a common convention: the public header is nested inside a directory matching the library name. This prevents header name collisions across libraries and makes includes self-documenting (e.g., `#include "greeting/greeting.h"`).

## Code Walkthrough

### libs/greeting/CMakeLists.txt

```cmake
add_library(greeting
    src/greeting.cpp
)

target_include_directories(greeting PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
```

This file defines the `greeting` library target. The critical detail is the `PUBLIC` keyword on `target_include_directories()`:

- **PUBLIC** means the include directory is used both when compiling the library itself and by any target that links against it.
- **PRIVATE** would mean only the library itself uses the path.
- **INTERFACE** would mean only consumers use the path, not the library itself.

`${CMAKE_CURRENT_SOURCE_DIR}` resolves to the directory containing this CMakeLists.txt (i.e., `libs/greeting/`), so the full include path becomes `libs/greeting/include`.

### Root CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.16)
project(subprojects)

add_subdirectory(libs/greeting)

add_executable(subprojects src/main.cpp)
target_link_libraries(subprojects PRIVATE greeting)
```

The root file does three things:
1. Pulls in the greeting library via `add_subdirectory()`.
2. Creates the main executable.
3. Links the executable to the greeting library with `PRIVATE` scope (the main executable uses greeting, but nothing else links against the executable).

Because greeting declared its includes as `PUBLIC`, the `subprojects` executable automatically knows where to find `greeting/greeting.h` without any extra `target_include_directories()` call.

### src/main.cpp

```cpp
#include <iostream>
#include "greeting/greeting.h"

int main() {
    std::cout << greeting::hello("CMake") << std::endl;
    std::cout << greeting::goodbye("CMake") << std::endl;
    return 0;
}
```

The main file includes the greeting header using the namespaced path `greeting/greeting.h`. This works because the PUBLIC include directory from the greeting library has been propagated.

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

**Manual build:**
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Expected Output

```
Hello, CMake!
Goodbye, CMake!
```

## Key Takeaways

1. **add_subdirectory()** is the primary way to include internal sub-libraries in a CMake project. Each subdirectory has its own `CMakeLists.txt` that defines targets.
2. **PUBLIC vs PRIVATE vs INTERFACE** on `target_include_directories()` controls how include paths propagate through the dependency chain. Use `PUBLIC` when consumers of your library need access to the same headers.
3. **Namespace your include paths** by placing headers under `include/<library_name>/`. This prevents collisions and makes dependencies clear in source code.
4. **target_link_libraries()** does more than linking. It propagates all PUBLIC and INTERFACE properties from the dependency, including include directories, compile definitions, and compile options.
5. **${CMAKE_CURRENT_SOURCE_DIR}** always refers to the directory of the CMakeLists.txt file currently being processed, which is essential when writing library-level CMakeLists files.

## Exercises

1. Add a second library under `libs/` (e.g., `libs/math`) with its own `CMakeLists.txt` and have the main executable link against both libraries.
2. Make the `greeting` library depend on your new `math` library using `target_link_libraries(greeting PUBLIC math)`. Observe how the include paths chain through to the executable.
3. Change the `PUBLIC` keyword on the greeting library's include directories to `PRIVATE`. Try to build and observe the error. Then fix it by adding the include path directly to the executable.
4. Add a compile definition to the greeting library using `target_compile_definitions(greeting PUBLIC GREETING_VERSION=2)` and use it in both the library and the main executable to verify propagation.
5. Create a nested subdirectory structure (e.g., `libs/core/utils/`) and explore how `add_subdirectory()` works with deeper paths.
