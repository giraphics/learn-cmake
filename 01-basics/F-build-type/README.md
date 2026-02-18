# F-build-type

This example demonstrates how CMake handles different build types (configurations) and how each one affects compiler flags, optimizations, and debug information. Understanding build types is essential for producing both debuggable development builds and optimized release builds.

## Concepts

CMake supports four standard build types, each applying a different set of compiler flags:

| Build Type      | Optimization | Debug Info | NDEBUG Defined | Typical Use Case              |
|-----------------|:------------:|:----------:|:--------------:|-------------------------------|
| Debug           | None (`-O0`) | Yes (`-g`) | No             | Day-to-day development        |
| Release         | Full (`-O3`) | No         | Yes            | Shipping to end users         |
| RelWithDebInfo  | Some (`-O2`) | Yes (`-g`) | Yes            | Profiling, crash analysis     |
| MinSizeRel      | Size (`-Os`) | No         | Yes            | Embedded, size-constrained    |

### Compiler Flags Per Build Type (GCC/Clang)

- **Debug**: `-O0 -g` -- No optimization, full debug symbols.
- **Release**: `-O3 -DNDEBUG` -- Maximum optimization, assertions disabled.
- **RelWithDebInfo**: `-O2 -g -DNDEBUG` -- Moderate optimization with debug symbols.
- **MinSizeRel**: `-Os -DNDEBUG` -- Optimize for binary size.

### MSVC Equivalents

- **Debug**: `/Od /Zi /MDd` -- No optimization, debug runtime.
- **Release**: `/O2 /DNDEBUG /MD` -- Full optimization, release runtime.
- **RelWithDebInfo**: `/O2 /Zi /DNDEBUG /MD` -- Optimization with PDB debug info.
- **MinSizeRel**: `/O1 /DNDEBUG /MD` -- Minimize size.

### Single-Config vs Multi-Config Generators

CMake generators fall into two categories:

- **Single-configuration generators** (Makefiles, Ninja): The build type is set at configure time via `-DCMAKE_BUILD_TYPE=Release`. Only one configuration exists per build directory.
- **Multi-configuration generators** (Visual Studio, Xcode, Ninja Multi-Config): All configurations coexist in the same build directory. The build type is chosen at build time via `cmake --build . --config Release`. The `CMAKE_BUILD_TYPE` variable is ignored.

When writing portable CMake code you should handle both cases. This example sets a default `CMAKE_BUILD_TYPE` but also checks `CMAKE_CONFIGURATION_TYPES` to avoid overriding multi-config generators.

## File Structure

```
F-build-type/
  CMakeLists.txt       # Build configuration with default build type logic
  build.sh             # Linux/macOS build script (accepts build type argument)
  build.bat            # Windows build script (accepts build type argument)
  src/
    main.cpp           # Prints whether NDEBUG is defined
```

## Code Walkthrough

### CMakeLists.txt

```cmake
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()
```

Key points:

1. **Guard condition**: We only set a default when neither `CMAKE_BUILD_TYPE` (single-config) nor `CMAKE_CONFIGURATION_TYPES` (multi-config) is already set.
2. **CACHE STRING ... FORCE**: Stores the value in the CMake cache so it persists across re-configurations. `FORCE` ensures the default is written even if the cache variable already exists as empty.
3. **set_property(CACHE ... PROPERTY STRINGS ...)**: Provides a dropdown list in cmake-gui and ccmake, making it easier for users to select a valid build type.

### src/main.cpp

The program checks whether the `NDEBUG` preprocessor macro is defined. This macro is automatically defined by CMake for Release, RelWithDebInfo, and MinSizeRel builds, and is notably used by the standard library to disable `assert()` calls.

## Building

### Linux / macOS

```bash
# Debug build (default)
./build.sh

# Release build
./build.sh Release

# RelWithDebInfo build
./build.sh RelWithDebInfo

# MinSizeRel build
./build.sh MinSizeRel
```

### Windows

```batch
REM Debug build (default)
build.bat

REM Release build
build.bat Release
```

### Manual CMake Commands

```bash
# Single-config generator (Makefiles / Ninja)
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Multi-config generator (Visual Studio / Xcode)
cmake -B build
cmake --build build --config Release
```

## Expected Output

### Debug Build

```
Build Type Example
NDEBUG is NOT defined (Debug mode)
```

### Release Build

```
Build Type Example
NDEBUG is defined (Release mode)
```

### CMake Configure Output (no build type specified)

```
-- Setting build type to 'RelWithDebInfo' as none was specified.
-- Build type: RelWithDebInfo
```

## Key Takeaways

1. **Always set a default build type.** If no build type is specified with a single-config generator, CMake uses an empty string, which means no optimization flags and no debug flags -- the worst of both worlds.
2. **Check both `CMAKE_BUILD_TYPE` and `CMAKE_CONFIGURATION_TYPES`** when setting defaults to avoid interfering with multi-config generators.
3. **Use `CACHE STRING ... FORCE`** to persist the default in the CMake cache and make it visible in GUI tools.
4. **`NDEBUG` is defined automatically** for Release, RelWithDebInfo, and MinSizeRel builds. This disables `assert()` and can be used for conditional compilation.
5. **For multi-config generators**, use `--config <type>` at build time, not `-DCMAKE_BUILD_TYPE` at configure time.
6. **Use generator expressions** like `$<CONFIG:Debug>` in CMakeLists.txt when you need per-configuration behavior that works with both single-config and multi-config generators.

## Exercises

1. Build the project with all four build types and compare the binary sizes. Which produces the smallest binary? Which produces the largest?
2. Add a `message()` that prints the actual compiler flags being used: `CMAKE_CXX_FLAGS_DEBUG`, `CMAKE_CXX_FLAGS_RELEASE`, etc. What flags does your compiler use for each build type?
3. Add an `assert(false)` statement to `main.cpp` and observe how it behaves in Debug vs Release mode.
4. Modify the default build type from `RelWithDebInfo` to `Debug`. When might you prefer one default over the other?
5. Try building with a multi-config generator (e.g., `cmake -G "Ninja Multi-Config" -B build`) and observe how `CMAKE_BUILD_TYPE` behaves differently.
