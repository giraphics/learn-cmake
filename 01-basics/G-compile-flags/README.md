# G-compile-flags

This example demonstrates how to set compiler warning flags, optimization options, and preprocessor definitions in CMake. It covers the modern per-target approach using `target_compile_options()` and `target_compile_definitions()`, as well as why you should avoid the older global `CMAKE_CXX_FLAGS` approach.

## Concepts

### Methods for Setting Compile Flags

There are several ways to pass flags to the compiler in CMake, listed from most preferred to least preferred:

| Method                         | Scope       | Modern CMake? | Recommendation       |
|--------------------------------|-------------|:-------------:|----------------------|
| `target_compile_options()`     | Per-target  | Yes           | Preferred            |
| `target_compile_definitions()` | Per-target  | Yes           | Preferred            |
| `target_compile_features()`    | Per-target  | Yes           | For C++ standard     |
| `add_compile_options()`        | Directory   | Partially     | Use sparingly        |
| `set(CMAKE_CXX_FLAGS ...)`    | Global      | No            | Avoid in new code    |
| `add_definitions()`           | Directory   | No            | Legacy, avoid        |

### Visibility Specifiers (PRIVATE / PUBLIC / INTERFACE)

When adding flags to a target, the visibility specifier controls who gets the flags:

- **PRIVATE**: Only the target itself uses these flags when compiling.
- **PUBLIC**: Both the target and anything that links to it get these flags.
- **INTERFACE**: Only consumers that link to the target get these flags, not the target itself.

For executables, `PRIVATE` is almost always correct. For libraries, choose carefully based on whether the flag is needed by consumers.

### Generator Expressions for Compiler Detection

CMake generator expressions allow compile flags to be conditionally applied:

- `$<CXX_COMPILER_ID:MSVC>` -- True when compiling with MSVC.
- `$<CXX_COMPILER_ID:GNU>` -- True when compiling with GCC.
- `$<CXX_COMPILER_ID:Clang>` -- True when compiling with Clang.
- `$<CONFIG:Debug>` -- True when building in Debug configuration.

### Common Warning Flags

**GCC / Clang:**
- `-Wall` -- Enable most common warnings.
- `-Wextra` -- Enable extra warnings beyond `-Wall`.
- `-Wpedantic` -- Warn about non-standard C++ usage.
- `-Werror` -- Treat warnings as errors (use in CI, not as default).

**MSVC:**
- `/W0` through `/W4` -- Warning levels (0 = off, 4 = most verbose).
- `/WX` -- Treat warnings as errors.
- `/Wall` -- Enable all warnings (usually too noisy).

### Compile Definitions

`target_compile_definitions()` adds `-D` flags to the compiler command line. These define preprocessor macros that can be tested with `#ifdef` in source code.

- `APP_VERSION="1.0.0"` becomes `-DAPP_VERSION="1.0.0"` on the command line.
- `$<$<CONFIG:Debug>:DEBUG_MODE>` defines `DEBUG_MODE` only in Debug builds.

## File Structure

```
G-compile-flags/
  CMakeLists.txt       # Demonstrates compile options and definitions
  build.sh             # Linux/macOS build script
  build.bat            # Windows build script
  src/
    main.cpp           # Contains an intentionally unused variable
```

## Code Walkthrough

### CMakeLists.txt

```cmake
target_compile_options(compile_flags PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)
```

This uses generator expressions to apply the right flags for each compiler. The `PRIVATE` keyword means these flags only affect the `compile_flags` target itself. If this were a library and we used `PUBLIC`, any target linking against it would also inherit these flags.

```cmake
target_compile_definitions(compile_flags PRIVATE
    APP_VERSION="1.0.0"
    $<$<CONFIG:Debug>:DEBUG_MODE>
)
```

This adds preprocessor definitions. `APP_VERSION` is always defined, while `DEBUG_MODE` is only defined in Debug builds. In the source code, you could use `#ifdef DEBUG_MODE` to conditionally compile debug-only features.

### src/main.cpp

The unused variable `x` is intentional. When you build this example, the compiler will produce a warning about it (e.g., `warning: unused variable 'x'`). This demonstrates that the warning flags are working correctly.

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
```

### Inspecting Actual Flags

To see exactly what flags CMake passes to the compiler:

```bash
# Method 1: Verbose build
cmake --build build --verbose

# Method 2: Use VERBOSE environment variable (Makefiles)
cd build && make VERBOSE=1

# Method 3: Check compile_commands.json (with Ninja or Makefiles)
cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

## Expected Output

### Build Output (GCC/Clang)

You should see a warning during compilation:

```
warning: unused variable 'x' [-Wunused-variable]
    int x = 42;  // Intentionally unused to demonstrate warnings
        ^
```

### Build Output (MSVC)

```
warning C4189: 'x': local variable is initialized but not referenced
```

### Program Output

```
Compile flags example
```

### CMake Configure Output

```
-- Compiler: GNU
-- Compiler version: 11.4.0
```

(Your compiler ID and version will vary.)

## Key Takeaways

1. **Use `target_compile_options()` instead of `CMAKE_CXX_FLAGS`.** Per-target flags are cleaner, composable, and do not leak across unrelated targets.
2. **Use generator expressions for compiler-specific flags.** This keeps your CMakeLists.txt portable across compilers without `if/else` blocks.
3. **Use `PRIVATE` for executable compile options.** Only use `PUBLIC` or `INTERFACE` on libraries when consumers genuinely need those flags.
4. **Use `target_compile_definitions()` for preprocessor macros.** This is cleaner than manually adding `-D` flags to compile options.
5. **Enable warnings early in a project.** Starting with `-Wall -Wextra -Wpedantic` (or `/W4` on MSVC) catches many bugs at compile time.
6. **Avoid `-Werror` as a default.** It can break builds when upgrading compilers. Use it in CI pipelines instead.
7. **Use `CMAKE_EXPORT_COMPILE_COMMANDS=ON`** to generate a `compile_commands.json` file, which is useful for IDEs and static analysis tools.

## Exercises

1. Fix the unused variable warning by either using the variable or casting it to `void` with `(void)x;`. Verify the warning disappears.
2. Add `-Werror` (or `/WX` for MSVC) to the compile options and observe how the unused variable now causes a build failure.
3. Add a `DEBUG_MODE` check in `main.cpp` that prints extra information, then build in both Debug and Release to see the difference.
4. Use `CMAKE_EXPORT_COMPILE_COMMANDS=ON` and inspect the generated `compile_commands.json` file to see the exact flags passed to the compiler.
5. Create a second target (another executable or a library) and verify that the `PRIVATE` flags on `compile_flags` do not affect it. Then try changing `PRIVATE` to `PUBLIC` on a library target and observe the difference.
