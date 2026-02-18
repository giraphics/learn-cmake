# CMake Variables

## Introduction

Variables are central to how CMake works. They control everything from which files to compile, to what compiler flags to use, to where to install the final product. CMake has several kinds of variables, each with different scope, lifetime, and purpose.

Understanding CMake's variable system is essential for writing maintainable build scripts and for configuring projects from the command line.

## Concepts

| Concept | Description |
|---|---|
| **Normal Variables** | Standard variables set with `set()`. Scoped to the current `CMakeLists.txt` and its children. Disappear when CMake re-runs. |
| **Cache Variables** | Persistent variables stored in `CMakeCache.txt`. Survive between CMake runs. Can be set from the command line with `-D`. |
| **Environment Variables** | Read from the system environment using `$ENV{NAME}`. Changes to these in CMake do not affect the calling shell. |
| **Built-in Variables** | Variables automatically set by CMake (e.g., `CMAKE_SOURCE_DIR`, `PROJECT_NAME`). Provide information about the project and system. |
| **message()** | Prints output during the configure step. `STATUS` prefix prints with `--` indentation. |

## File Structure

```
E-variables/
├── CMakeLists.txt
├── build.sh
├── build.bat
├── README.md
└── src/
    └── main.cpp
```

## Code Walkthrough

### Project Declaration with Version

```cmake
cmake_minimum_required(VERSION 3.16)
project(hello_variables VERSION 1.2.3)
```

The `VERSION` keyword in `project()` sets several variables automatically:
- `PROJECT_VERSION` = `1.2.3`
- `PROJECT_VERSION_MAJOR` = `1`
- `PROJECT_VERSION_MINOR` = `2`
- `PROJECT_VERSION_PATCH` = `3`

These are useful for embedding version information into your code (via `configure_file()`, covered in later examples).

### Normal Variables

```cmake
set(MY_VARIABLE "Hello")
message(STATUS "MY_VARIABLE = ${MY_VARIABLE}")
```

The `set()` command creates a variable. `${MY_VARIABLE}` dereferences it (retrieves its value). Normal variables are:
- **Scoped** to the current directory and below (child `CMakeLists.txt` files inherit them).
- **Transient** -- they exist only during the configure step and are not saved between runs.
- **String-based** -- all CMake variables are strings. Even numbers and booleans are stored as strings.

### List Variables

```cmake
set(SOURCE_FILES src/main.cpp)
message(STATUS "SOURCE_FILES = ${SOURCE_FILES}")
```

In CMake, a "list" is simply a string with semicolons as separators. For example:

```cmake
set(MY_LIST "a;b;c")      # Three-element list
set(MY_LIST a b c)          # Same thing -- spaces become semicolons
set(MY_LIST "a" "b" "c")   # Also the same
```

You can manipulate lists with the `list()` command:

```cmake
list(APPEND MY_LIST "d")    # Add an element
list(LENGTH MY_LIST len)     # Get the length
list(GET MY_LIST 0 first)    # Get element at index 0
```

### Cache Variables

```cmake
set(USE_FEATURE ON CACHE BOOL "Enable the special feature")
message(STATUS "USE_FEATURE = ${USE_FEATURE}")
```

Cache variables are special:
- They are **stored in `CMakeCache.txt`** in the build directory and persist between CMake runs.
- They can be **set from the command line** with `-D`: `cmake -DUSE_FEATURE=OFF ..`
- They have a **type** (`BOOL`, `STRING`, `PATH`, `FILEPATH`) used by CMake GUIs.
- They have a **description string** shown in CMake GUIs.
- Once a cache variable is set, `set(...CACHE...)` will **not overwrite it** (unless you add `FORCE`).

Cache variables are the primary mechanism for user-configurable options. They are also how CMake remembers your compiler choice, build type, and other settings between runs.

### Built-in Variables

```cmake
message(STATUS "CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")
message(STATUS "PROJECT_NAME = ${PROJECT_NAME}")
message(STATUS "PROJECT_VERSION = ${PROJECT_VERSION}")
message(STATUS "CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")
```

CMake provides many built-in variables. The most commonly used ones:

| Variable | Description |
|---|---|
| `CMAKE_SOURCE_DIR` | Top-level source directory (where the root `CMakeLists.txt` is). |
| `CMAKE_BINARY_DIR` | Top-level build directory (where you ran `cmake`). |
| `CMAKE_CURRENT_SOURCE_DIR` | Directory of the currently-processed `CMakeLists.txt`. |
| `CMAKE_CURRENT_BINARY_DIR` | Build directory corresponding to the current source directory. |
| `PROJECT_SOURCE_DIR` | Source directory of the most recent `project()` call. |
| `PROJECT_NAME` | Name from the most recent `project()` call. |
| `PROJECT_VERSION` | Version from the most recent `project()` call. |
| `CMAKE_SYSTEM_NAME` | Operating system name (`Linux`, `Windows`, `Darwin`). |
| `CMAKE_CXX_COMPILER` | Path to the C++ compiler being used. |
| `CMAKE_BUILD_TYPE` | Build configuration (`Debug`, `Release`, etc.) for single-config generators. |

### Environment Variables

```cmake
message(STATUS "PATH = $ENV{PATH}")
```

Environment variables are accessed with the `$ENV{NAME}` syntax. Important notes:
- Reading `$ENV{PATH}` gets the PATH from the environment where `cmake` was invoked.
- You can set environment variables with `set(ENV{NAME} value)`, but this only affects the CMake process and child processes during the build -- it does **not** modify the calling shell's environment.
- Environment variable changes are **not cached** between CMake runs.

### Passing Variables from the Command Line

The `-D` flag is the standard way to set cache variables from the command line:

```bash
cmake -DUSE_FEATURE=OFF -DCMAKE_BUILD_TYPE=Release ..
```

Common patterns:
- `-DCMAKE_BUILD_TYPE=Release` -- Set the build type
- `-DCMAKE_INSTALL_PREFIX=/usr/local` -- Set the install location
- `-DBUILD_SHARED_LIBS=ON` -- Build shared instead of static libraries
- `-DCMAKE_CXX_COMPILER=clang++` -- Choose a specific compiler

### Variable Scope Rules

CMake variables follow directory-based scoping:

```
root/CMakeLists.txt         <-- set(VAR "hello")
  ├── libA/CMakeLists.txt   <-- ${VAR} is "hello" (inherited)
  └── libB/CMakeLists.txt   <-- ${VAR} is "hello" (inherited)
```

A child directory inherits copies of all parent variables. Modifications in a child do **not** affect the parent. To send a value back to the parent, use `set(VAR value PARENT_SCOPE)`.

Functions also create a new scope (variables set inside a function do not leak out). Macros do **not** create a new scope.

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

### With custom variables

```bash
mkdir -p build && cd build
cmake -DUSE_FEATURE=OFF ..
cmake --build .
```

## Expected Output

During the configure step (`cmake ..`), you will see the variable values printed:

```
-- MY_VARIABLE = Hello
-- SOURCE_FILES = src/main.cpp
-- USE_FEATURE = ON
-- CMAKE_SOURCE_DIR = /path/to/E-variables
-- CMAKE_BINARY_DIR = /path/to/E-variables/build
-- PROJECT_NAME = hello_variables
-- PROJECT_VERSION = 1.2.3
-- CMAKE_SYSTEM_NAME = Linux
-- PATH = /usr/local/bin:/usr/bin:...
-- Configuring done
-- Generating done
```

Program output:

```
Learning CMake variables!
```

## Key Takeaways

- CMake has three kinds of variables: **normal** (transient, scoped), **cache** (persistent, in `CMakeCache.txt`), and **environment** (from the OS).
- All CMake variables are strings. Lists are semicolon-separated strings.
- Use `${VAR}` to dereference normal/cache variables, `$ENV{VAR}` for environment variables.
- Cache variables are set from the command line with `-D` and persist between CMake runs.
- `project(name VERSION x.y.z)` automatically creates version variables.
- CMake provides many built-in variables like `CMAKE_SOURCE_DIR`, `PROJECT_NAME`, and `CMAKE_SYSTEM_NAME`.
- Variables follow directory-based scoping: children inherit copies, modifications do not propagate upward (unless `PARENT_SCOPE` is used).
- `message(STATUS "...")` is the primary debugging tool for inspecting variable values during configuration.
- After configuring, inspect `CMakeCache.txt` in the build directory to see all cached variables and their values.

## Exercises

1. **Override a cache variable.** Run `cmake -DUSE_FEATURE=OFF ..` and observe the output. Then run `cmake ..` again (without `-D`) and check whether the value reverts or stays. Open `build/CMakeCache.txt` and find the `USE_FEATURE` entry to understand why.

2. **Explore version components.** Add `message()` calls to print `PROJECT_VERSION_MAJOR`, `PROJECT_VERSION_MINOR`, and `PROJECT_VERSION_PATCH`. Change the version in `project()` and verify the values update correctly.

3. **Experiment with scope.** Create a subdirectory `sub/` with its own `CMakeLists.txt`. Set a variable in the parent, modify it in the child (with and without `PARENT_SCOPE`), and print it in both locations. Observe how scoping works. Use `add_subdirectory(sub)` in the parent to include the child.
