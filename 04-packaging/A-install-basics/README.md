# A-install-basics: CPack and Install Rules

This example demonstrates how to use CMake's `install()` command and CPack module to create distributable packages from your project. You will learn how to define install rules for executables, libraries, and headers, then use CPack to bundle everything into archives (ZIP, TGZ) or platform-specific installers (DEB, RPM, NSIS).

## Concepts

### The install() Command

The `install()` command tells CMake where files should be placed when the user runs `cmake --install .` (or the legacy `make install`). Without install rules, CMake only knows how to build your project -- it does not know how to deploy it.

There are several forms of `install()`:

- **install(TARGETS ...)** -- installs executables, libraries, or other build artifacts. You specify destination directories using `RUNTIME DESTINATION`, `LIBRARY DESTINATION`, and `ARCHIVE DESTINATION` for executables, shared libraries, and static libraries respectively.
- **install(DIRECTORY ...)** -- copies an entire directory tree (e.g., header files) into the install prefix.
- **install(FILES ...)** -- installs individual files.

### Generator Expressions for Include Paths

When a library is both built locally and installed for consumers, its include paths differ:

```cmake
target_include_directories(mathlib PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
```

- `$<BUILD_INTERFACE:...>` applies when the target is used from the build tree.
- `$<INSTALL_INTERFACE:...>` applies when the target is used after installation. Paths here are relative to `CMAKE_INSTALL_PREFIX`.

### CPack

CPack is CMake's packaging companion. By setting `CPACK_*` variables and calling `include(CPack)`, you enable the `cpack` command to generate distributable packages directly from install rules.

**Important**: `include(CPack)` must appear *after* all `CPACK_*` variable definitions, because CPack reads the variables at include time.

### Available CPack Generators

| Generator | Platform | Package Type |
|-----------|----------|-------------|
| ZIP       | All      | .zip archive |
| TGZ       | All      | .tar.gz archive |
| DEB       | Linux    | Debian .deb package |
| RPM       | Linux    | Red Hat .rpm package |
| NSIS      | Windows  | NSIS installer (.exe) |
| WIX       | Windows  | MSI installer |
| DragNDrop | macOS    | .dmg disk image |
| productbuild | macOS | .pkg installer |

### Key CPACK_* Variables

| Variable | Purpose |
|----------|---------|
| `CPACK_PACKAGE_NAME` | Name of the package |
| `CPACK_PACKAGE_VERSION` | Package version string |
| `CPACK_PACKAGE_DESCRIPTION_SUMMARY` | Short description |
| `CPACK_PACKAGE_VENDOR` | Vendor or author name |
| `CPACK_PACKAGE_CONTACT` | Contact email |
| `CPACK_GENERATOR` | Semicolon-separated list of generators to use |
| `CPACK_DEBIAN_PACKAGE_MAINTAINER` | Required for DEB packages |
| `CPACK_RPM_PACKAGE_LICENSE` | License for RPM packages |

## File Structure

```
A-install-basics/
  CMakeLists.txt        # Build system with install rules and CPack config
  build.sh              # Linux/macOS build and package script
  build.bat             # Windows build and package script
  include/
    mathlib.h           # Public header for the math library
  src/
    mathlib.cpp         # Library implementation
    main.cpp            # Demo executable
```

## Code Walkthrough

### CMakeLists.txt

**Project and library setup:**

```cmake
cmake_minimum_required(VERSION 3.16)
project(mathlib_package VERSION 1.0.0)

add_library(mathlib src/mathlib.cpp)
target_include_directories(mathlib PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
```

The library uses generator expressions so include paths work correctly in both build and install contexts.

**Executable:**

```cmake
add_executable(mathapp src/main.cpp)
target_link_libraries(mathapp PRIVATE mathlib)
```

**Install rules for the executable:**

```cmake
install(TARGETS mathapp
    RUNTIME DESTINATION bin
)
```

`RUNTIME DESTINATION bin` places the executable in `<prefix>/bin`. On Windows, DLLs are also classified as RUNTIME.

**Install rules for the library:**

```cmake
install(TARGETS mathlib
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
```

- `LIBRARY DESTINATION lib` -- for shared libraries (.so, .dylib).
- `ARCHIVE DESTINATION lib` -- for static libraries (.a, .lib).

**Install headers:**

```cmake
install(DIRECTORY include/
    DESTINATION include
)
```

The trailing `/` on `include/` is important: it copies the *contents* of the directory rather than the directory itself. Without it, you would get `<prefix>/include/include/mathlib.h` instead of `<prefix>/include/mathlib.h`.

**CPack configuration:**

```cmake
set(CPACK_PACKAGE_NAME "MathLib")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR "ZIP;TGZ")
include(CPack)
```

The `CPACK_GENERATOR` variable selects which package formats to produce. The `include(CPack)` line must come last.

### Source Files

- `include/mathlib.h` -- declares three functions in the `mathlib` namespace: `add`, `subtract`, and `pi`.
- `src/mathlib.cpp` -- implements those functions.
- `src/main.cpp` -- a demo program that calls each function and prints results.

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

### Manual Steps

```bash
# Configure
cmake -B build

# Build
cmake --build build

# Install to a local directory (optional)
cmake --install build --prefix ./install

# Create packages
cd build
cpack
```

To select a specific generator:

```bash
cpack -G ZIP
cpack -G TGZ
cpack -G DEB    # Linux only, requires dpkg
cpack -G NSIS   # Windows only, requires NSIS installed
```

## Expected Output

### Build and run:

```
MathLib Package Example
2 + 3 = 5
5 - 1 = 4
Pi = 3.14159
```

### Package creation:

```
--- Creating packages ---
CPack: Create package using ZIP
CPack: Install projects
CPack: - Install project: mathlib_package
CPack: Create package
CPack: - package: ./MathLib-1.0.0-win64.zip generated.
CPack: Create package using TGZ
CPack: Install projects
CPack: - Install project: mathlib_package
CPack: Create package
CPack: - package: ./MathLib-1.0.0-win64.tar.gz generated.
```

### Package contents (ZIP example):

```
MathLib-1.0.0-win64/
  bin/
    mathapp.exe
  lib/
    mathlib.lib
  include/
    mathlib.h
```

## Key Takeaways

1. **install() is a prerequisite for CPack** -- CPack uses the install rules to determine what goes into the package. If you have no `install()` commands, your packages will be empty.

2. **Use generator expressions** for include directories so paths are correct in both build and install contexts.

3. **Set CPACK_* variables before include(CPack)** -- CPack reads variables at include time. Setting them afterward has no effect.

4. **The trailing slash matters** in `install(DIRECTORY include/ ...)` -- with it, the contents are copied; without it, the directory itself is copied, creating an extra nesting level.

5. **RUNTIME, LIBRARY, ARCHIVE** refer to different artifact types. On Windows, both executables and DLLs use RUNTIME. Static libraries use ARCHIVE. Shared libraries on Linux/macOS use LIBRARY.

6. **CPack generators are platform-specific** -- DEB and RPM require Linux tools, NSIS requires the NSIS installer on Windows, DragNDrop is macOS-only. ZIP and TGZ work everywhere.

## Exercises

1. **Add NSIS support**: Install NSIS on Windows and add `NSIS` to the `CPACK_GENERATOR` list. Run `cpack -G NSIS` and inspect the generated installer.

2. **Install a license file**: Create a `LICENSE` file and use `install(FILES LICENSE DESTINATION share/doc/MathLib)` to include it in the package. Set `CPACK_RESOURCE_FILE_LICENSE` to point to it.

3. **Component-based packaging**: Use `install(TARGETS mathapp COMPONENT runtime ...)` and `install(TARGETS mathlib COMPONENT development ...)` to separate runtime and development components. Use `cpack -G ZIP -D CPACK_COMPONENTS_ALL=runtime` to package only the runtime.

4. **Version the shared library**: Change `add_library(mathlib ...)` to build a shared library with `add_library(mathlib SHARED ...)` and set `VERSION` and `SOVERSION` properties. Observe how the install rules handle the symlinks.

5. **Custom install prefix**: Build with `cmake -DCMAKE_INSTALL_PREFIX=/opt/mathlib ..` and run `cmake --install .` to see how the prefix affects the final layout.
