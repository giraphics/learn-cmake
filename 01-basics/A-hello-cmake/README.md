# Hello CMake

## Introduction

CMake is a cross-platform build system generator. Rather than building your code directly, CMake generates native build files for your platform -- Makefiles on Linux/macOS, Visual Studio project files on Windows, Ninja build files, and more. This means you write one set of build instructions (in `CMakeLists.txt` files) and CMake translates them into whatever your system needs.

This first example is the simplest possible CMake project: a single C++ source file compiled into an executable. It introduces the three fundamental CMake commands that every project uses.

## Concepts

| Concept | Description |
|---|---|
| **CMakeLists.txt** | The file CMake reads for build instructions. Every CMake project must have at least one in the root directory. The filename is case-sensitive and must be exactly `CMakeLists.txt`. |
| **cmake_minimum_required()** | Declares the minimum version of CMake needed to process this file. This must be the first command in any `CMakeLists.txt`. It sets policies and ensures compatibility. |
| **project()** | Declares the project name (and optionally version, languages, etc.). This sets several useful variables like `PROJECT_NAME` and `PROJECT_SOURCE_DIR`. |
| **add_executable()** | Creates a build target that produces an executable binary. You give it a target name and a list of source files. |

## File Structure

```
A-hello-cmake/
├── CMakeLists.txt      # Build instructions
├── build.sh            # Linux/macOS build script
├── build.bat           # Windows build script
├── README.md           # This file
└── src/
    └── main.cpp        # Source code
```

## Code Walkthrough

Let us walk through the `CMakeLists.txt` line by line:

```cmake
# Set the minimum version of CMake required
cmake_minimum_required(VERSION 3.16)
```

This tells CMake that version 3.16 or higher is required. If someone tries to build this project with an older version, CMake will stop and print an error. Version 3.16 was chosen because it is widely available on modern systems and supports all the features used in this tutorial series.

```cmake
# Set the project name
project(hello_cmake)
```

This declares a project named `hello_cmake`. Behind the scenes, CMake sets several variables:
- `PROJECT_NAME` becomes `hello_cmake`
- `PROJECT_SOURCE_DIR` points to the directory containing this `CMakeLists.txt`
- `PROJECT_BINARY_DIR` points to the build directory

```cmake
# Add an executable target
add_executable(hello_cmake src/main.cpp)
```

This is where the actual build target is defined. It tells CMake: "Build an executable called `hello_cmake` from the source file `src/main.cpp`." On Linux this produces a file called `hello_cmake`, on Windows it produces `hello_cmake.exe`.

## Building

CMake uses an "out-of-source" build approach. You create a separate `build/` directory so that generated files do not clutter your source tree.

### Linux / macOS

```bash
# From the A-hello-cmake directory:
mkdir -p build && cd build
cmake ..
cmake --build .
./hello_cmake
```

Or simply run the provided script:

```bash
chmod +x build.sh
./build.sh
```

### Windows

```batch
mkdir build
cd build
cmake ..
cmake --build .
Debug\hello_cmake.exe
```

Or run the provided script:

```batch
build.bat
```

### Step-by-step explanation

1. **`mkdir build && cd build`** -- Create and enter a build directory.
2. **`cmake ..`** -- Run CMake, pointing it at the parent directory where `CMakeLists.txt` lives. This is the "configure" step that generates native build files.
3. **`cmake --build .`** -- Build the project using whatever native build system CMake generated. This is a portable way to invoke `make`, `msbuild`, `ninja`, etc.
4. **`./hello_cmake`** -- Run the resulting executable.

## Expected Output

During the configure step you will see CMake identifying your compiler:

```
-- The C compiler identification is GNU 11.4.0
-- The CXX compiler identification is GNU 11.4.0
...
-- Build files have been written to: .../A-hello-cmake/build
```

During the build step:

```
[ 50%] Building CXX object CMakeFiles/hello_cmake.dir/src/main.cpp.o
[100%] Linking CXX executable hello_cmake
[100%] Built target hello_cmake
```

When running the executable:

```
Hello, CMake!
```

## Key Takeaways

- Every CMake project needs a `CMakeLists.txt` file in the root directory.
- `cmake_minimum_required(VERSION x.y)` must be the first command.
- `project()` declares the project and sets built-in variables.
- `add_executable(target_name source1 source2 ...)` defines what to build.
- Always use out-of-source builds (a separate `build/` directory).
- `cmake --build .` is the portable way to invoke the underlying build tool.
- CMake is a build system **generator**, not a build system itself.

## Exercises

1. **Add a second source file.** Create `src/greeting.cpp` with a function `void greet()` that prints a message, and call it from `main()`. Update `CMakeLists.txt` to include both source files in `add_executable()`.

2. **Explore project() options.** Modify the `project()` command to include a version number: `project(hello_cmake VERSION 1.0.0)`. Add a `message(STATUS "Version: ${PROJECT_VERSION}")` line and observe the output during the configure step.

3. **Try a different generator.** If you have Ninja installed, run `cmake -G Ninja ..` instead of `cmake ..` and compare the build output. List the available generators on your system with `cmake --help`.
