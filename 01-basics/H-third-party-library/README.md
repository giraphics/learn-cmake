# H-third-party-library

This example demonstrates how to find and link third-party libraries in CMake using `find_package()`. It shows both required and optional package discovery, and introduces the modern imported targets pattern (e.g., `Threads::Threads`) that is the standard in modern CMake.

## Concepts

### find_package() Overview

`find_package()` is CMake's primary mechanism for locating external libraries. It searches for package configuration files or find modules and, when successful, creates variables and imported targets that you can use to link against the library.

```cmake
find_package(<PackageName> [version] [REQUIRED] [QUIET] [COMPONENTS comp1 comp2 ...])
```

Key arguments:

- **REQUIRED**: CMake will stop with an error if the package is not found.
- **QUIET**: Suppresses informational messages. Useful for optional packages.
- **COMPONENTS**: Request specific parts of a package (e.g., `find_package(Boost REQUIRED COMPONENTS filesystem regex)`).
- **version**: Minimum version requirement (e.g., `find_package(ZLIB 1.2)`).

### Two Modes of find_package()

CMake uses two search mechanisms in order:

1. **Config mode**: Looks for `<PackageName>Config.cmake` or `<lowercase-name>-config.cmake` files. These are typically installed by the library itself (via `install(EXPORT ...)`). This is the preferred mode.

2. **Module mode**: Looks for `Find<PackageName>.cmake` files in `CMAKE_MODULE_PATH` and CMake's built-in module directory. These are written by CMake maintainers or by you for libraries that do not provide their own config files.

### Imported Targets vs Legacy Variables

Older CMake code uses variables set by find modules:

```cmake
# Legacy approach (avoid)
find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})
target_link_libraries(myapp ${ZLIB_LIBRARIES})
```

Modern CMake provides imported targets that bundle includes, libraries, and compile definitions:

```cmake
# Modern approach (preferred)
find_package(ZLIB REQUIRED)
target_link_libraries(myapp PRIVATE ZLIB::ZLIB)
```

Imported targets (identified by the `::` in their name) are self-contained. When you link against `ZLIB::ZLIB`, CMake automatically adds the correct include paths, link flags, and dependencies.

### Common Packages and Their Imported Targets

| Package       | Imported Target          | Notes                              |
|---------------|--------------------------|------------------------------------|
| Threads       | `Threads::Threads`       | POSIX threads / Win32 threads      |
| ZLIB          | `ZLIB::ZLIB`             | Compression library                |
| OpenSSL       | `OpenSSL::SSL`, `OpenSSL::Crypto` | TLS/crypto library          |
| Boost         | `Boost::filesystem`, etc.| Per-component targets              |
| Python3       | `Python3::Python`        | Python interpreter and libraries   |
| OpenGL        | `OpenGL::GL`             | Graphics library                   |
| Vulkan        | `Vulkan::Vulkan`         | Graphics API                       |
| GTest         | `GTest::gtest`           | Google Test framework              |

### CMAKE_PREFIX_PATH

If a library is installed in a non-standard location, you can tell CMake where to look:

```bash
cmake -B build -DCMAKE_PREFIX_PATH=/path/to/library/install
```

You can specify multiple paths separated by semicolons. This is commonly used when working with package managers like vcpkg or Conan.

## File Structure

```
H-third-party-library/
  CMakeLists.txt       # find_package() for Threads (required) and ZLIB (optional)
  build.sh             # Linux/macOS build script
  build.bat            # Windows build script
  src/
    main.cpp           # Uses HAS_THREADS / HAS_ZLIB preprocessor checks
```

## Code Walkthrough

### CMakeLists.txt

**Required package -- Threads:**

```cmake
find_package(Threads REQUIRED)
target_link_libraries(third_party PRIVATE Threads::Threads)
target_compile_definitions(third_party PRIVATE HAS_THREADS)
```

`Threads` is available on virtually all platforms, making it a safe example. The `REQUIRED` keyword causes CMake to fail if threads are somehow unavailable. `Threads::Threads` is an imported target that handles platform differences automatically -- on Linux it adds `-lpthread`, on Windows it links the Win32 threading API, and on macOS it uses the appropriate framework.

**Optional package -- ZLIB:**

```cmake
find_package(ZLIB QUIET)
if(ZLIB_FOUND)
    message(STATUS "ZLIB found: ${ZLIB_VERSION_STRING}")
    target_compile_definitions(third_party PRIVATE HAS_ZLIB)
else()
    message(STATUS "ZLIB not found (optional -- skipping)")
endif()
```

`QUIET` prevents CMake from printing warnings when ZLIB is not found. The `ZLIB_FOUND` variable is set by the find module and can be tested to conditionally include library support. This pattern is common for optional features.

### src/main.cpp

The source file uses `#ifdef` checks to report which libraries were found at build time. This pattern is useful for optional functionality -- the program compiles and runs regardless of whether ZLIB is available.

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

### Manual CMake Commands

```bash
cmake -B build
cmake --build build
./build/third_party
```

### With a Custom Library Path

```bash
cmake -B build -DCMAKE_PREFIX_PATH=/opt/mylibs
cmake --build build
```

## Expected Output

### CMake Configure Output

```
-- Threads found: TRUE
-- Thread library: -lpthread
-- ZLIB not found (optional -- skipping)
```

(On Windows, the thread library field may be empty since threading is built into the OS.)

If ZLIB is installed:

```
-- Threads found: TRUE
-- Thread library: -lpthread
-- ZLIB found: 1.2.11
-- ZLIB includes: /usr/include
```

### Program Output

```
Third-party library example
This demonstrates the find_package() pattern.
Threading support is available!
```

## Key Takeaways

1. **Use `find_package()` to locate external libraries.** It is the standard CMake mechanism and integrates with package managers.
2. **Prefer imported targets (`Library::Library`) over legacy variables.** Imported targets carry all necessary include paths, link flags, and transitive dependencies.
3. **Use `REQUIRED` for essential dependencies and `QUIET` for optional ones.** This provides clear error messages for required packages and clean output for optional ones.
4. **Use `CMAKE_PREFIX_PATH` for non-standard install locations.** This is how you integrate libraries from vcpkg, Conan, or custom builds.
5. **Check `<PackageName>_FOUND`** to conditionally enable features based on available libraries.
6. **Config mode is preferred over Module mode.** Libraries that install `*Config.cmake` files provide the most reliable integration.
7. **The `::` in target names indicates an imported target.** If you misspell it, CMake will give an error at configure time (unlike plain library names, which would fail at link time).

## Exercises

1. Install ZLIB on your system (e.g., `sudo apt install zlib1g-dev` on Ubuntu, or use vcpkg on Windows) and rebuild to see the ZLIB detection succeed.
2. Add `find_package(OpenSSL QUIET)` and print whether it was found. If available, print the OpenSSL version.
3. Deliberately misspell an imported target name (e.g., `Threads::Thread` instead of `Threads::Threads`) and observe the error message CMake produces. Compare this to misspelling a plain library name.
4. Use `cmake --find-package -DNAME=ZLIB -DCOMPILER_ID=GNU -DLANGUAGE=C -DMODE=EXIST` to query package availability from the command line.
5. Explore the find module for Threads by reading CMake's source: run `cmake --help-module FindThreads` or look in your CMake installation's Modules directory.
