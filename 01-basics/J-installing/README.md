# J-installing

This example demonstrates how to use CMake's `install()` command to install executables, libraries, and header files to a specified location. It covers install destinations, the `CMAKE_INSTALL_PREFIX` variable, and the `BUILD_INTERFACE` / `INSTALL_INTERFACE` generator expressions that allow a library to work correctly both during development and after installation.

## Concepts

### The install() Command

The `install()` command defines rules for copying built artifacts and other files to an installation directory. Nothing is actually installed during `cmake --build` -- installation happens as a separate step via `cmake --install` or `make install`.

Common forms:

```cmake
# Install targets (executables, libraries)
install(TARGETS target1 target2
    RUNTIME DESTINATION bin          # Executables, DLLs (Windows)
    LIBRARY DESTINATION lib          # Shared libraries (.so on Linux)
    ARCHIVE DESTINATION lib          # Static libraries (.a, .lib)
)

# Install header files
install(DIRECTORY include/
    DESTINATION include
)

# Install individual files
install(FILES config.txt
    DESTINATION etc
)
```

### CMAKE_INSTALL_PREFIX

This variable controls the root directory for installation. All `DESTINATION` paths in `install()` commands are relative to this prefix.

| Platform | Default Prefix         |
|----------|------------------------|
| Linux    | `/usr/local`           |
| macOS    | `/usr/local`           |
| Windows  | `C:/Program Files/...` |

You almost always want to override it:

```bash
cmake -B build -DCMAKE_INSTALL_PREFIX=/path/to/install
```

### GNUInstallDirs

CMake provides the `GNUInstallDirs` module for platform-standard install directories:

```cmake
include(GNUInstallDirs)

install(TARGETS mylib
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}      # bin
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}      # lib or lib64
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}           # include
)
```

This is especially useful on Linux where 64-bit libraries may go in `lib64` instead of `lib`.

### BUILD_INTERFACE and INSTALL_INTERFACE

When a library specifies include directories with `target_include_directories()`, it needs different paths depending on whether the library is being built or has been installed:

```cmake
target_include_directories(hello_library PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
```

- **`$<BUILD_INTERFACE:...>`**: Used when the target is consumed directly from the build tree (e.g., via `add_subdirectory()` or `target_link_libraries()` within the same project). Points to the source directory.
- **`$<INSTALL_INTERFACE:...>`**: Used when the target is consumed after installation (e.g., via `find_package()` from another project). The path is relative to `CMAKE_INSTALL_PREFIX`.

Without these generator expressions, either the build or the installation would have incorrect include paths.

### Target Artifact Types

CMake categorizes build outputs into several types, each with its own install destination:

| Artifact Type | What It Is                              | Typical Destination |
|---------------|-----------------------------------------|---------------------|
| RUNTIME       | Executables, DLLs (Windows)             | `bin`               |
| LIBRARY       | Shared libraries (.so, .dylib)          | `lib`               |
| ARCHIVE       | Static libraries (.a, .lib), import libs| `lib`               |
| INCLUDES      | Header files (via INCLUDES DESTINATION) | `include`           |

On Windows, a shared library produces both a DLL (RUNTIME) and an import library (ARCHIVE), so you need both destinations.

## File Structure

```
J-installing/
  CMakeLists.txt       # Build and install rules
  build.sh             # Linux/macOS build and install script
  build.bat            # Windows build and install script
  include/
    Hello.h            # Public header file
  src/
    Hello.cpp          # Library implementation
    main.cpp           # Executable that uses the library
```

After installation:

```
install/
  bin/
    hello_binary       # Installed executable
  lib/
    libhello_library.a # Installed static library (or .so/.lib)
  include/
    Hello.h            # Installed public header
```

## Code Walkthrough

### CMakeLists.txt

**Library with proper include paths:**

```cmake
add_library(hello_library src/Hello.cpp)
target_include_directories(hello_library PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
```

The `PUBLIC` keyword means both the library itself and any target linking against it can see these include directories. The generator expressions ensure the correct path is used in each context.

**Executable linked to the library:**

```cmake
add_executable(hello_binary src/main.cpp)
target_link_libraries(hello_binary PRIVATE hello_library)
```

Because `hello_library` declared its include directories as `PUBLIC`, the `hello_binary` target automatically gets the correct include path without any additional `target_include_directories()` call.

**Install rules:**

```cmake
install(TARGETS hello_binary RUNTIME DESTINATION bin)
install(TARGETS hello_library LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
```

Three separate install rules handle each component. Note the trailing slash in `include/` -- this copies the contents of the `include` directory (i.e., `Hello.h`) directly into the destination, rather than creating an `include/include/` nesting.

### include/Hello.h and src/Hello.cpp

A simple class with a header in `include/` and implementation in `src/`. This layout separates public headers (which get installed) from private source files (which do not).

### src/main.cpp

A minimal program that creates a `Hello` object and calls its `print()` method. After installation, this binary can run independently of the source tree.

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
# Configure with a local install prefix
cmake -B build -DCMAKE_INSTALL_PREFIX=$(pwd)/install

# Build
cmake --build build

# Install
cmake --install build

# Run the installed binary
./install/bin/hello_binary
```

### Installing a Specific Configuration (Multi-Config)

```bash
cmake --install build --config Release
```

## Expected Output

### Build and Install Output

```
-- Install prefix: /path/to/J-installing/install
-- Configuring done
-- Generating done
-- Build files have been written to: /path/to/J-installing/build

...

-- Installing: /path/to/J-installing/install/bin/hello_binary
-- Installing: /path/to/J-installing/install/lib/libhello_library.a
-- Installing: /path/to/J-installing/install/include/Hello.h
```

### Installed Files Listing

```
install/bin/hello_binary
install/include/Hello.h
install/lib/libhello_library.a
```

### Program Output

```
Hello from an installed library!
```

## Key Takeaways

1. **Use `install()` to define what gets installed and where.** Separate `install()` calls for targets, headers, and other files.
2. **Always set `CMAKE_INSTALL_PREFIX`** to a project-local directory during development to avoid polluting system directories.
3. **Use `BUILD_INTERFACE` and `INSTALL_INTERFACE` generator expressions** to provide correct include paths in both build and install contexts.
4. **Specify all relevant artifact types** in `install(TARGETS ...)`: `RUNTIME` for executables/DLLs, `LIBRARY` for shared libraries, `ARCHIVE` for static/import libraries.
5. **Use a trailing slash** in `install(DIRECTORY include/ ...)` to copy contents rather than the directory itself.
6. **Consider using `GNUInstallDirs`** for platform-appropriate install directories, especially on Linux where `lib` vs `lib64` matters.
7. **Installation is a separate step.** Running `cmake --build` alone does not install anything. Use `cmake --install` (CMake 3.15+) or `make install`.
8. **For complete package support**, you would also need `install(EXPORT ...)` and a `*Config.cmake` file, which enables other projects to find your library with `find_package()`. This is covered in more advanced examples.

## Exercises

1. Run the build script and examine the `install/` directory. Verify that the binary, library, and header are all present in the correct locations.
2. Add `include(GNUInstallDirs)` and replace the hardcoded `bin`, `lib`, and `include` destinations with `${CMAKE_INSTALL_BINDIR}`, `${CMAKE_INSTALL_LIBDIR}`, and `${CMAKE_INSTALL_INCLUDEDIR}`.
3. Change `hello_library` from a static library to a shared library using `add_library(hello_library SHARED ...)`. Rebuild and observe how the install layout changes (on Linux, the `.so` file goes to `lib`; on Windows, the `.dll` goes to `bin` and the `.lib` import library goes to `lib`).
4. Add a second header file to `include/` and verify it gets installed automatically because of the `install(DIRECTORY ...)` rule.
5. Try installing to the system default prefix (without setting `CMAKE_INSTALL_PREFIX`) and observe where CMake tries to place files. Note: this may require administrator/root privileges.
