# B: Interface Libraries -- Header-Only Dependencies in CMake

Many popular C++ libraries are header-only: there is nothing to compile into a `.a` or `.so` file, yet consumers still need include paths, compile definitions, and feature requirements. CMake models these with **INTERFACE libraries** -- targets that carry properties but produce no build artifacts of their own. This example creates a header-only math utility library and shows how all of its requirements propagate cleanly to consumers.

## Concepts

- **INTERFACE library**: Created with `add_library(name INTERFACE)`. It has no source files and produces no compiled output. Its sole purpose is to bundle a set of properties (include directories, compile features, definitions, linked libraries) that are forwarded to any target that links against it.
- **INTERFACE keyword on target commands**: When a library is declared as INTERFACE, all `target_*` commands on it must use the `INTERFACE` keyword. You cannot use `PUBLIC` or `PRIVATE` because the library has no "self" to apply properties to -- everything exists only for consumers.
- **Comparison with STATIC and SHARED libraries**:
  - `STATIC` / `SHARED` -- compiled from source files, can use PUBLIC, PRIVATE, and INTERFACE.
  - `INTERFACE` -- no source files, can only use INTERFACE. Properties propagate identically through `target_link_libraries`.
- **Real-world INTERFACE libraries**: Eigen (linear algebra), nlohmann/json (JSON parsing), Catch2 (single-header testing), and many Boost components are header-only. When these libraries provide CMake config files, they define INTERFACE imported targets.
- **`_USE_MATH_DEFINES`**: On MSVC, the constant `M_PI` is not defined by default in `<cmath>`. Defining `_USE_MATH_DEFINES` before including `<cmath>` enables it. By placing this in the INTERFACE library's compile definitions, every consumer automatically gets it.

## File Structure

```
B-interface-libraries/
  CMakeLists.txt
  build.sh
  build.bat
  include/
    mathlib/
      math_utils.h
  src/
    main.cpp
```

## Code Walkthrough

### CMakeLists.txt

**Creating the INTERFACE library:**

```cmake
add_library(mathlib INTERFACE)
```

No source files are listed. This target exists purely to carry properties.

**Setting INTERFACE properties:**

```cmake
target_include_directories(mathlib INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(mathlib INTERFACE cxx_std_14)

target_compile_definitions(mathlib INTERFACE
    _USE_MATH_DEFINES
)
```

Every command uses the `INTERFACE` keyword. These properties do not affect `mathlib` itself (it has no compilation step) but will be applied to any target that links against it.

**Consuming the INTERFACE library:**

```cmake
add_executable(app src/main.cpp)
target_link_libraries(app PRIVATE mathlib)
```

After this single `target_link_libraries` call, `app` receives:
- The include directory containing `mathlib/math_utils.h`.
- The C++14 standard requirement.
- The `_USE_MATH_DEFINES` preprocessor definition.

No manual `include_directories()` or `add_definitions()` needed.

### Header File (math_utils.h)

The header provides a mix of compile-time and runtime utilities inside the `mathlib` namespace:

- **`Factorial<N>`** -- a template metaprogramming example that computes factorials at compile time.
- **`circle_area()`** -- uses `M_PI` (enabled via the `_USE_MATH_DEFINES` definition).
- **`average()`** -- demonstrates use of `<numeric>` and `<algorithm>`.
- **`clamp()`** -- a generic clamping function template.

All functions are either templates or marked `inline`, which is essential for header-only libraries to avoid multiple-definition linker errors.

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

```
Interface (Header-Only) Library Example

5! = 120
Circle area (r=3): 28.2743
Average: 3
clamp(15, 0, 10): 10
```

## Key Takeaways

1. **INTERFACE libraries model header-only dependencies.** They carry no source files and produce no compiled output, but they can specify include directories, compile features, definitions, and even link dependencies.
2. **Only the INTERFACE keyword is valid.** Since there is no compiled target to apply properties to, PUBLIC and PRIVATE are not allowed on INTERFACE library targets.
3. **Property propagation works identically to compiled libraries.** Consumers get everything they need from a single `target_link_libraries` call, just as with STATIC or SHARED libraries.
4. **INTERFACE libraries are ideal for packaging requirements.** Even if you do not have header files, you can create an INTERFACE library to bundle a set of compile flags, definitions, or linked system libraries under a single target name.
5. **Use `inline` or templates in header-only code.** Without `inline`, function definitions in headers included by multiple translation units will cause linker errors.

## Exercises

1. Add a second executable that also links against `mathlib`. Verify that it compiles and runs correctly without any additional configuration.
2. Remove the `_USE_MATH_DEFINES` definition from the INTERFACE library and try to build on MSVC. Observe the error, then add it back.
3. Add an INTERFACE link library to `mathlib` (e.g., `target_link_libraries(mathlib INTERFACE some_other_lib)`) and see how it chains through to consumers.
4. Convert `mathlib` from an INTERFACE library to a STATIC library by adding a `src/math_utils.cpp` file with a non-template function. Compare the CMakeLists.txt changes required.
5. Create a second INTERFACE library (e.g., `stringlib`) and have `mathlib` depend on it via `target_link_libraries(mathlib INTERFACE stringlib)`. Verify that the transitive dependency reaches the final executable.
