# Hello Headers

## Introduction

In real-world C++ projects, code is never crammed into a single file. Declarations go into header files (`.h` / `.hpp`) and implementations go into source files (`.cpp`). This separation is fundamental to C++ development -- it enables faster compilation, cleaner APIs, and better code organization.

This example demonstrates how to tell CMake where to find your header files using `target_include_directories()`, and how to group source files into a variable using `set()`.

## Concepts

| Concept | Description |
|---|---|
| **Header/Source Separation** | Headers declare interfaces (classes, function signatures). Source files contain implementations. This is the standard C++ project layout. |
| **target_include_directories()** | Tells CMake where to search for header files when compiling a specific target. Without this, `#include "Hello.h"` would fail because the compiler would not know to look in the `include/` directory. |
| **PRIVATE / PUBLIC / INTERFACE** | Controls who inherits the include path. PRIVATE means only this target uses it. PUBLIC means this target and anything that links to it. INTERFACE means only things that link to it (not the target itself). |
| **set()** | Creates a CMake variable. Commonly used to group source files into a named list for cleaner `add_executable()` calls. |
| **PROJECT_SOURCE_DIR** | A built-in variable set by `project()`. It always points to the directory containing the most recent `project()` command's `CMakeLists.txt`. |

## File Structure

```
B-hello-headers/
├── CMakeLists.txt
├── build.sh
├── build.bat
├── README.md
├── include/
│   └── Hello.h          # Class declaration
└── src/
    ├── Hello.cpp         # Class implementation
    └── main.cpp          # Entry point
```

This layout -- `include/` for headers, `src/` for source files -- is one of the most common conventions in C++ projects. Some projects use `inc/` or place headers alongside sources; what matters is consistency.

## Code Walkthrough

### The Header File (`include/Hello.h`)

```cpp
#ifndef HELLO_H
#define HELLO_H

class Hello {
public:
    void print();
};

#endif
```

The `#ifndef` / `#define` / `#endif` pattern is called an **include guard**. It prevents the header from being processed more than once if multiple files include it. Modern C++ also supports `#pragma once` as a shorter alternative.

### The CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.16)
project(hello_headers)
```

Standard preamble -- minimum version and project name.

```cmake
set(SOURCES
    src/main.cpp
    src/Hello.cpp
)
```

The `set()` command creates a variable called `SOURCES` containing two file paths. This is cleaner than listing all files directly in `add_executable()`, especially as projects grow. Note that each file is on its own line for readability -- CMake treats whitespace-separated values as list elements.

```cmake
add_executable(hello_headers ${SOURCES})
```

The `${SOURCES}` syntax dereferences the variable, expanding to `src/main.cpp src/Hello.cpp`. This is equivalent to writing `add_executable(hello_headers src/main.cpp src/Hello.cpp)`.

```cmake
target_include_directories(hello_headers PRIVATE ${PROJECT_SOURCE_DIR}/include)
```

This is the key line. It tells the compiler: "When building `hello_headers`, add the `include/` directory to the header search path." This is why `#include "Hello.h"` works in both `main.cpp` and `Hello.cpp` -- without this line, the compiler would report that it cannot find `Hello.h`.

The `PRIVATE` keyword means this include path is only used when compiling `hello_headers` itself. Since `hello_headers` is an executable (nothing links to it), PRIVATE is the correct choice here.

### Understanding PRIVATE, PUBLIC, and INTERFACE

These keywords control **transitive usage requirements** -- how build settings propagate between targets:

| Keyword | This target uses it? | Targets linking to this use it? |
|---|---|---|
| **PRIVATE** | Yes | No |
| **PUBLIC** | Yes | Yes |
| **INTERFACE** | No | Yes |

For an executable target, PRIVATE is almost always correct because nothing links to an executable. The distinction becomes important with libraries (covered in examples C and D).

## Building

### Linux / macOS

```bash
chmod +x build.sh
./build.sh
```

Or manually:

```bash
mkdir -p build && cd build
cmake ..
cmake --build .
./hello_headers
```

### Windows

```batch
build.bat
```

Or manually:

```batch
mkdir build
cd build
cmake ..
cmake --build .
Debug\hello_headers.exe
```

## Expected Output

Build output:

```
[ 33%] Building CXX object CMakeFiles/hello_headers.dir/src/main.cpp.o
[ 66%] Building CXX object CMakeFiles/hello_headers.dir/src/Hello.cpp.o
[100%] Linking CXX executable hello_headers
[100%] Built target hello_headers
```

Program output:

```
Hello from a header/source split!
```

## Key Takeaways

- Use `include/` for headers and `src/` for source files -- this is the most common C++ project layout.
- `target_include_directories()` tells the compiler where to find headers for a specific target.
- Use `PRIVATE` for executable targets. Use `PUBLIC` for library targets whose headers are needed by consumers.
- `set()` creates variables to group related values (like source file lists).
- `${VARIABLE_NAME}` dereferences a variable. `${PROJECT_SOURCE_DIR}` always points to the project root.
- Include guards (`#ifndef` / `#define` / `#endif`) or `#pragma once` prevent double-inclusion of headers.
- Always use `target_include_directories()` (modern CMake) instead of the older `include_directories()` which affects all targets globally.

## Exercises

1. **Add another class.** Create `Goodbye.h` and `Goodbye.cpp` with a `Goodbye` class that has a `print()` method. Update `CMakeLists.txt` to include the new source file, and call both classes from `main()`.

2. **Experiment with include paths.** Remove the `target_include_directories()` line and try to build. Observe the compiler error. Then try using `#include "../include/Hello.h"` as a relative path instead -- note why this is fragile and why `target_include_directories()` is preferred.

3. **Use GLOB (and understand the tradeoff).** Replace the manual `set(SOURCES ...)` with `file(GLOB SOURCES src/*.cpp)`. This automatically finds all `.cpp` files. Build and verify it works. Then read the CMake documentation on why `GLOB` is discouraged for source files (hint: CMake does not know to re-run when new files are added).
