# A: Target Properties -- The Foundation of Modern CMake

Modern CMake (3.0+) is built around a single core idea: **targets are the unit of abstraction**. Instead of setting global variables and directory-level commands, you attach properties directly to targets and let CMake propagate them automatically through dependency relationships. This example demonstrates how to define a library and an executable using purely target-based commands, and how properties flow from one target to another via `target_link_libraries`.

## Concepts

- **Targets**: Named build artifacts created with `add_library` or `add_executable`. Every target carries a set of properties (include directories, compile options, definitions, link libraries, etc.).
- **PUBLIC / PRIVATE / INTERFACE keywords**: These control how properties propagate:
  - `PRIVATE` -- applies only to the target itself.
  - `PUBLIC` -- applies to the target AND to any target that links against it.
  - `INTERFACE` -- applies only to consumers that link against the target, not the target itself.
- **Property propagation**: When target B calls `target_link_libraries(B PRIVATE A)`, all of A's `INTERFACE` and `PUBLIC` properties are automatically inherited by B. This is the mechanism that eliminates the need for global `include_directories()` or `add_definitions()`.
- **Generator expressions**: The `$<BUILD_INTERFACE:...>` and `$<INSTALL_INTERFACE:...>` expressions let you specify different values depending on whether the target is being built locally or has been installed.
- **`target_compile_features`**: Sets the required C++ standard as a target property rather than using the global `CMAKE_CXX_STANDARD` variable.
- **`get_target_property`**: Allows you to inspect a target's properties at configure time, which is useful for debugging and verification.

## File Structure

```
A-target-properties/
  CMakeLists.txt
  build.sh
  build.bat
  include/
    greeter.h
  src/
    greeter.cpp
    main.cpp
```

## Code Walkthrough

### CMakeLists.txt

The CMake file creates two targets: a static library (`greeter`) and an executable (`app`).

**Library target setup:**

```cmake
add_library(greeter src/greeter.cpp)

target_include_directories(greeter PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
```

The `PUBLIC` keyword means that both `greeter` itself and any target linking against it will receive the include directory. The generator expressions ensure correct paths for both build-tree and install-tree usage.

```cmake
target_compile_features(greeter PUBLIC cxx_std_17)
```

This sets C++17 as a requirement. Because it is `PUBLIC`, any consumer of `greeter` also gets C++17 enforced.

```cmake
target_compile_options(greeter PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra>
)
```

Compiler warnings are `PRIVATE` -- they apply only when compiling greeter's own sources, not when compiling the executable.

```cmake
target_compile_definitions(greeter PUBLIC
    GREETER_VERSION="${PROJECT_VERSION}"
)
```

The `GREETER_VERSION` macro is `PUBLIC` so both the library and any consumer can reference it.

**Executable target setup:**

```cmake
add_executable(app src/main.cpp)
target_link_libraries(app PRIVATE greeter)
```

This single line gives `app` everything it needs: include paths, C++17, and the `GREETER_VERSION` definition. All of these propagate automatically from `greeter`'s `PUBLIC` properties.

**Property inspection:**

```cmake
get_target_property(GREETER_INCLUDES greeter INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "greeter includes: ${GREETER_INCLUDES}")
```

This prints the interface include directories at configure time, useful for verifying that properties are set correctly.

### Source Files

- **greeter.h** declares a `Greeter` class with `greet()` and `farewell()` methods.
- **greeter.cpp** implements the class, demonstrating that the library compiles against its own include directory.
- **main.cpp** includes `greeter.h` without specifying any path -- the include directory propagates from the library target.

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
./app        # Linux/macOS
Debug\app.exe  # Windows (MSVC)
```

## Expected Output

During CMake configuration:
```
-- greeter includes: /path/to/A-target-properties/include
-- greeter features: cxx_std_17
```

When running the executable:
```
Hello, Modern CMake! (using target properties)
Goodbye, Modern CMake!
```

## Key Takeaways

1. **Targets replace global state.** Instead of `include_directories()`, use `target_include_directories()`. Instead of `add_definitions()`, use `target_compile_definitions()`. Instead of `set(CMAKE_CXX_STANDARD 17)`, use `target_compile_features()`.
2. **PUBLIC/PRIVATE/INTERFACE controls propagation.** Think of PUBLIC as "I need this AND my consumers need this", PRIVATE as "only I need this", and INTERFACE as "only my consumers need this".
3. **`target_link_libraries` is the propagation mechanism.** It does far more than linking -- it transfers all interface properties from the dependency to the consumer.
4. **Generator expressions handle build vs. install differences.** Always use `$<BUILD_INTERFACE:...>` and `$<INSTALL_INTERFACE:...>` for include directories in libraries that might be installed.
5. **Per-target compile options keep builds clean.** Strict warnings on a library do not bleed into its consumers.

## Exercises

1. Add a `PRIVATE` compile definition `GREETER_DEBUG` to the `greeter` target. Verify that `main.cpp` does NOT have access to this definition (try `#ifdef GREETER_DEBUG` in both files).
2. Change the `target_compile_options` from `PRIVATE` to `PUBLIC` and observe how the warning flags now also apply when compiling `main.cpp`.
3. Add a second executable that also links against `greeter`. Confirm it automatically receives all the same properties.
4. Use `get_target_property` to inspect additional properties like `INTERFACE_COMPILE_DEFINITIONS` and `LINK_LIBRARIES`.
5. Replace `target_compile_features(greeter PUBLIC cxx_std_17)` with the older `set(CMAKE_CXX_STANDARD 17)` approach and compare: does the executable still get C++17? Why or why not?
