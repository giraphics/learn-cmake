# Static Library

## Introduction

A **static library** is an archive of compiled object files that gets linked directly into your executable at build time. The library's code is literally copied into the final binary. On Linux/macOS, static libraries have the `.a` extension (standing for "archive"). On Windows, they use `.lib`.

This example demonstrates how to create a static library with CMake using `add_library()`, and how to link it to an executable with `target_link_libraries()`. Understanding libraries is essential -- virtually every C++ project of any size is organized into libraries.

## Concepts

| Concept | Description |
|---|---|
| **Static Library** | A collection of compiled object files bundled into a single archive file. The linker copies the needed code into the executable at build time. |
| **add_library()** | Creates a library target. The `STATIC` keyword specifies a static library. |
| **target_link_libraries()** | Links a library to a target. This handles both the linker flags and, crucially, transitive build requirements like include directories. |
| **PUBLIC include directories** | When a library marks its include directories as PUBLIC, any target that links to the library automatically inherits those include paths. |
| **Transitive properties** | Build properties (include dirs, compile flags, etc.) that automatically propagate from a library to its consumers through `target_link_libraries()`. |

## File Structure

```
C-static-library/
├── CMakeLists.txt
├── build.sh
├── build.bat
├── README.md
├── include/
│   └── Hello.h          # Library's public header
└── src/
    ├── Hello.cpp         # Library implementation
    └── main.cpp          # Application that uses the library
```

## Code Walkthrough

### CMakeLists.txt -- Line by Line

```cmake
cmake_minimum_required(VERSION 3.16)
project(hello_library)
```

Standard preamble.

```cmake
add_library(hello_library STATIC src/Hello.cpp)
```

This creates a **static library** target named `hello_library` from `src/Hello.cpp`. When built, this produces:
- Linux/macOS: `libhello_library.a`
- Windows: `hello_library.lib`

The `STATIC` keyword is explicit. If you omit it, CMake uses the value of `BUILD_SHARED_LIBS` (defaults to OFF, meaning static).

```cmake
target_include_directories(hello_library PUBLIC ${PROJECT_SOURCE_DIR}/include)
```

This line does two things because of the `PUBLIC` keyword:

1. **For the library itself:** The `include/` directory is added to the compiler's header search path when compiling `Hello.cpp`. This lets `Hello.cpp` use `#include "Hello.h"`.

2. **For consumers of the library:** Any target that links to `hello_library` via `target_link_libraries()` will automatically get the `include/` directory added to its search path. This is why `main.cpp` can also use `#include "Hello.h"` without any extra configuration.

This is the critical difference from Example B, where we used `PRIVATE`. A library's public headers need to be available to its consumers, so `PUBLIC` is the correct choice here.

```cmake
add_executable(hello_binary src/main.cpp)
```

Creates the executable target. Notice that `main.cpp` includes `Hello.h` but we do not call `target_include_directories()` for `hello_binary` -- the PUBLIC property on the library handles this automatically.

```cmake
target_link_libraries(hello_binary PRIVATE hello_library)
```

This links `hello_library` to `hello_binary`. It does two things:
1. Tells the linker to include the library's code in the executable.
2. Propagates all PUBLIC (and INTERFACE) properties from `hello_library` to `hello_binary` -- in this case, the include directory.

We use `PRIVATE` here because `hello_binary` is an executable. The fact that it uses `hello_library` is an internal implementation detail; nothing links to `hello_binary`.

### Why PUBLIC on the Library, PRIVATE on the Executable?

Think of it this way:

- **The library** exposes `Hello.h` as its API. Anyone using the library needs this header. Therefore the include directory is `PUBLIC`.
- **The executable** consumes the library internally. Nothing else depends on the executable. Therefore the link relationship is `PRIVATE`.

This pattern -- PUBLIC includes on the library, PRIVATE linking on the consumer -- is the most common pattern in CMake projects.

## How Static Linking Works

```
                    Build Time
                    ==========

  Hello.cpp ──compile──> Hello.o ──archive──> libhello_library.a
                                                     │
  main.cpp  ──compile──> main.o                      │
                           │                         │
                           └──────── link ───────────┘
                                      │
                                      v
                                hello_binary
                           (self-contained executable)
```

At link time, the linker extracts the needed object files from the `.a` archive and copies them into the final executable. The resulting binary is **self-contained** -- it does not need the `.a` file at runtime.

## Building

### Linux / macOS

```bash
chmod +x build.sh
./build.sh
```

### Windows

```batch
build.bat
```

## Expected Output

Build output:

```
[ 25%] Building CXX object CMakeFiles/hello_library.dir/src/Hello.cpp.o
[ 50%] Linking CXX static library libhello_library.a
[ 50%] Built target hello_library
[ 75%] Building CXX object CMakeFiles/hello_binary.dir/src/main.cpp.o
[100%] Linking CXX executable hello_binary
[100%] Built target hello_binary
```

Notice that CMake builds the library first, then the executable. It automatically determines the correct build order from the dependency graph.

Program output:

```
Hello from a static library!
```

## Key Takeaways

- `add_library(name STATIC sources...)` creates a static library target.
- Static libraries are archives of compiled code (`.a` on Linux/macOS, `.lib` on Windows).
- Static linking copies library code into the executable -- the resulting binary is self-contained.
- Use `PUBLIC` on `target_include_directories()` for library headers that consumers need.
- `target_link_libraries()` propagates PUBLIC and INTERFACE properties transitively.
- CMake automatically determines the correct build order from the dependency graph.
- The `PRIVATE`/`PUBLIC`/`INTERFACE` keywords on `target_link_libraries()` control whether the dependency is further propagated (matters when libraries depend on other libraries).

## Exercises

1. **Inspect the static library.** After building, find the `.a` or `.lib` file in the build directory. On Linux/macOS, run `ar -t libhello_library.a` to list its contents. On Windows, use `lib /list hello_library.lib` from a Developer Command Prompt.

2. **Add a second library.** Create a `Goodbye` class in its own library (`goodbye_library`). Link both `hello_library` and `goodbye_library` to the executable. Verify that both libraries' include directories are propagated correctly.

3. **Test the PRIVATE vs PUBLIC difference.** Change the library's `target_include_directories()` from `PUBLIC` to `PRIVATE`. Try to build and observe the error in `main.cpp`. This demonstrates that PRIVATE include paths do not propagate to consumers.
