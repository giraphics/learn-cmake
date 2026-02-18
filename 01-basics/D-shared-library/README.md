# Shared Library

## Introduction

A **shared library** (also called a dynamic library) is a library that is loaded at runtime rather than being copied into the executable at build time. The executable contains only references to the library's symbols; the actual code lives in a separate file that must be present when the program runs.

Shared libraries have different extensions depending on the platform:
- **Linux:** `.so` (shared object)
- **macOS:** `.dylib` (dynamic library)
- **Windows:** `.dll` (dynamic-link library) paired with a `.lib` import library

This example is nearly identical to Example C (Static Library) but uses `SHARED` instead of `STATIC`, and the implications are significant.

## Concepts

| Concept | Description |
|---|---|
| **Shared Library** | A library loaded at runtime. Multiple programs can share a single copy in memory, reducing disk and memory usage. |
| **SHARED keyword** | Passed to `add_library()` to create a shared/dynamic library instead of a static one. |
| **Runtime linking** | The operating system's dynamic linker locates and loads shared libraries when a program starts (or on demand with `dlopen`/`LoadLibrary`). |
| **RPATH** | (Linux/macOS) A path embedded in the executable that tells the dynamic linker where to find shared libraries at runtime. CMake manages this automatically during development. |
| **DLL** | On Windows, shared libraries are split into a `.dll` (the actual code) and an import `.lib` (a small stub used at link time). Both are needed. |

## File Structure

```
D-shared-library/
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

### CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.16)
project(hello_shared)
```

Standard preamble.

```cmake
add_library(hello_library SHARED src/Hello.cpp)
```

The only difference from the static library example is the `SHARED` keyword. This produces:
- Linux: `libhello_library.so`
- macOS: `libhello_library.dylib`
- Windows: `hello_library.dll` + `hello_library.lib` (import library)

```cmake
target_include_directories(hello_library PUBLIC ${PROJECT_SOURCE_DIR}/include)
```

Same as the static example. PUBLIC ensures that consumers of the library inherit the include path.

```cmake
add_executable(hello_binary src/main.cpp)
target_link_libraries(hello_binary PRIVATE hello_library)
```

Identical to the static example. CMake handles the differences between static and shared linking transparently. Your `CMakeLists.txt` for the consumer does not change -- only the library's `add_library()` call differs.

### Static vs Shared -- What Changes Under the Hood

| Aspect | Static Library | Shared Library |
|---|---|---|
| File extension | `.a` / `.lib` | `.so` / `.dylib` / `.dll` |
| When linked | Build time (code copied) | Runtime (loaded by OS) |
| Executable size | Larger (contains library code) | Smaller (only references) |
| Library needed at runtime? | No | Yes |
| Memory sharing | Each process has its own copy | Multiple processes can share one copy |
| Update library without recompiling? | No | Yes (if ABI unchanged) |

## How Shared Linking Works

```
                    Build Time
                    ==========

  Hello.cpp ──compile──> Hello.o ──link──> libhello_library.so
                                                  │
  main.cpp  ──compile──> main.o                   │
                           │                      │
                           └──── link (refs) ─────┘
                                    │
                                    v
                              hello_binary
                        (contains references only)


                    Runtime
                    =======

  hello_binary ──starts──> OS dynamic linker
                                │
                     locates & loads
                                │
                                v
                      libhello_library.so
                     (must be findable!)
```

## RPATH -- How the Executable Finds the Library

On Linux and macOS, the operating system's dynamic linker needs to know where to find shared libraries. It searches several locations:

1. **RPATH** -- A path embedded in the executable itself.
2. **LD_LIBRARY_PATH** (Linux) / **DYLD_LIBRARY_PATH** (macOS) -- Environment variables.
3. **System directories** -- `/lib`, `/usr/lib`, etc.

CMake handles RPATH automatically:
- **During development:** CMake sets the RPATH to point at the build directory, so you can run the executable directly after building.
- **After installation:** CMake adjusts the RPATH (or removes it) based on your install configuration.

On **Windows**, the DLL must be in the same directory as the executable, in the system PATH, or in the current working directory. CMake on Windows with MSVC places the DLL in the same output directory as the executable by default.

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

**Windows note:** If you get a "DLL not found" error, ensure the `.dll` file is in the same directory as the `.exe`. With MSVC generators, CMake typically handles this. With other generators, you may need to set `CMAKE_RUNTIME_OUTPUT_DIRECTORY`:

```cmake
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
```

## Expected Output

Build output:

```
[ 25%] Building CXX object CMakeFiles/hello_library.dir/src/Hello.cpp.o
[ 50%] Linking CXX shared library libhello_library.so
[ 50%] Built target hello_library
[ 75%] Building CXX object CMakeFiles/hello_binary.dir/src/main.cpp.o
[100%] Linking CXX executable hello_binary
[100%] Built target hello_binary
```

Program output:

```
Hello from a shared library!
```

## Key Takeaways

- `add_library(name SHARED sources...)` creates a shared/dynamic library.
- Shared libraries are loaded at runtime -- the executable only stores references to symbols.
- The same `target_link_libraries()` syntax works for both static and shared libraries.
- Multiple programs can share one copy of a shared library in memory.
- Shared libraries can be updated without recompiling the executable (as long as the ABI is compatible).
- RPATH (Linux/macOS) tells the executable where to find shared libraries at runtime. CMake manages this automatically.
- On Windows, DLLs must be locatable at runtime (same directory, system PATH, etc.).
- Omitting the STATIC/SHARED keyword in `add_library()` lets the user choose via the `BUILD_SHARED_LIBS` variable: `cmake -DBUILD_SHARED_LIBS=ON ..`.

## Exercises

1. **Compare file sizes.** Build both Example C (static) and Example D (shared). Compare the size of the `hello_binary` executable in each case. The shared version should be smaller because it does not contain the library code.

2. **Break the runtime link.** After building, rename or move the `.so`/`.dylib`/`.dll` file and try running the executable. Observe the "library not found" error. This demonstrates the runtime dependency that shared libraries introduce.

3. **Use BUILD_SHARED_LIBS.** Modify the `add_library()` call to omit the type keyword: `add_library(hello_library src/Hello.cpp)`. Then build with `cmake -DBUILD_SHARED_LIBS=ON ..` and `cmake -DBUILD_SHARED_LIBS=OFF ..` to toggle between shared and static from the command line.
