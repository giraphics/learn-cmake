# I-cpp-standard

This example demonstrates how to specify the C++ language standard in CMake. It shows the preferred per-target approach using `target_compile_features()`, explains the difference between `CMAKE_CXX_STANDARD` and `target_compile_features()`, and covers related properties like `CXX_EXTENSIONS` and `CXX_STANDARD_REQUIRED`.

## Concepts

### Methods for Setting the C++ Standard

There are three common approaches, listed from most preferred to least:

| Method                       | Scope      | Modern CMake? | Notes                             |
|------------------------------|------------|:-------------:|-----------------------------------|
| `target_compile_features()`  | Per-target | Yes           | Preferred. Expresses requirements.|
| `set_target_properties(CXX_STANDARD ...)` | Per-target | Yes | Good, but less expressive.    |
| `set(CMAKE_CXX_STANDARD ...)` | Global   | Partially     | Affects all targets. Use sparingly.|

### target_compile_features()

```cmake
target_compile_features(myapp PRIVATE cxx_std_17)
```

This tells CMake: "this target requires at least C++17." CMake will:
- Add the appropriate compiler flag (e.g., `-std=c++17` or `/std:c++17`).
- Fail at configure time if the compiler does not support C++17.

Available meta-features: `cxx_std_11`, `cxx_std_14`, `cxx_std_17`, `cxx_std_20`, `cxx_std_23`.

You can also request specific language features instead of a full standard:

```cmake
target_compile_features(myapp PRIVATE cxx_auto_type cxx_range_for)
```

CMake will automatically select the minimum standard that provides all requested features.

### CXX_STANDARD Target Property

```cmake
set_target_properties(myapp PROPERTIES CXX_STANDARD 17)
```

This directly sets the standard version. Unlike `target_compile_features()`, it does not fail if the compiler cannot satisfy the request unless `CXX_STANDARD_REQUIRED` is also set.

### CXX_STANDARD_REQUIRED

```cmake
set_target_properties(myapp PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
```

When `CXX_STANDARD_REQUIRED` is `ON`, CMake will produce an error if the compiler does not support the requested standard. When `OFF` (the default), CMake silently falls back to the newest standard the compiler supports. Always set this to `ON` when using `CXX_STANDARD` directly.

Note: `target_compile_features(... cxx_std_17)` implicitly sets `CXX_STANDARD_REQUIRED` to `ON`, which is one reason it is preferred.

### CXX_EXTENSIONS

```cmake
set_target_properties(myapp PROPERTIES CXX_EXTENSIONS OFF)
```

Controls whether compiler-specific extensions are allowed:

- **ON** (default): Uses `-std=gnu++17` on GCC/Clang, which enables GNU extensions.
- **OFF**: Uses `-std=c++17`, which enforces strict ISO C++ compliance.

Setting `CXX_EXTENSIONS OFF` is recommended for portable code. GNU extensions (like `typeof`, variable-length arrays, and statement expressions) are not available on all compilers.

### Global vs Per-Target

```cmake
# Global (affects ALL targets in this directory and below)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Per-target (affects only this target) -- PREFERRED
target_compile_features(myapp PRIVATE cxx_std_17)
set_target_properties(myapp PROPERTIES CXX_EXTENSIONS OFF)
```

The global approach is convenient for simple projects. The per-target approach is better for projects with multiple targets that might need different standards, or for libraries that should not impose a standard on their consumers.

## File Structure

```
I-cpp-standard/
  CMakeLists.txt       # Sets C++17 standard using target_compile_features
  build.sh             # Linux/macOS build script
  build.bat            # Windows build script
  src/
    main.cpp           # Uses C++11, C++14, and C++17 features
```

## Code Walkthrough

### CMakeLists.txt

```cmake
target_compile_features(cpp_standard PRIVATE cxx_std_17)
```

This is the single most important line. It tells CMake that the `cpp_standard` target needs C++17 or later. CMake will select the right compiler flag and fail early if the compiler is too old.

```cmake
set_target_properties(cpp_standard PROPERTIES
    CXX_EXTENSIONS OFF
)
```

This ensures we get strict `-std=c++17` instead of `-std=gnu++17`. Our code is written in portable ISO C++, so we want the compiler to enforce that.

### src/main.cpp

The source file deliberately uses features from multiple C++ standards:

- **C++11**: `std::vector` initializer lists, range-based for loops, `auto`.
- **C++17**: `std::optional`, `std::nullopt`, if-statements with initializers.

If you change the required standard to `cxx_std_14` or `cxx_std_11`, the build will fail because `<optional>` is a C++17 header.

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
./build/cpp_standard
```

### Overriding the Standard from the Command Line

```bash
# You can still override with CMAKE_CXX_STANDARD, but target_compile_features
# acts as a minimum, so the higher standard wins.
cmake -B build -DCMAKE_CXX_STANDARD=20
```

## Expected Output

### CMake Configure Output

```
-- C++ standard:
-- C++ compiler: GNU 11.4.0
```

Note: `CMAKE_CXX_STANDARD` may appear empty because we used `target_compile_features()` rather than setting it directly. The standard is still correctly applied at the target level.

### Program Output

```
Numbers: 1 2 3 4 5
Found: 3
99 not found in the vector
```

## Key Takeaways

1. **Use `target_compile_features(target PRIVATE cxx_std_XX)` to set the C++ standard.** It is per-target, self-documenting, and implies `CXX_STANDARD_REQUIRED ON`.
2. **Set `CXX_EXTENSIONS OFF` for portable code.** This avoids accidental reliance on compiler-specific extensions.
3. **If using `CXX_STANDARD` directly, always pair it with `CXX_STANDARD_REQUIRED ON`.** Otherwise CMake silently falls back to an older standard if the compiler does not support your request.
4. **For libraries, use `PUBLIC` instead of `PRIVATE`** with `target_compile_features()` if consumers of the library also need C++17 (e.g., because your public headers use C++17 types).
5. **The standard specified is a minimum.** If a consumer requires C++20 and links to your C++17 library, everything compiles as C++20.
6. **Avoid setting `CMAKE_CXX_STANDARD` globally** in projects that contain multiple targets or are consumed as subdirectories. Per-target settings prevent conflicts.

## Exercises

1. Change `cxx_std_17` to `cxx_std_14` and observe the compilation error. Which specific feature causes the failure?
2. Remove the `CXX_EXTENSIONS OFF` property and inspect the compiler command line (using `--verbose` or `compile_commands.json`). Notice the difference between `-std=c++17` and `-std=gnu++17`.
3. Replace `target_compile_features()` with `set_target_properties(cpp_standard PROPERTIES CXX_STANDARD 17)` but omit `CXX_STANDARD_REQUIRED`. Then try setting `CXX_STANDARD` to 99 and observe what happens.
4. Use fine-grained compile features instead of a meta-feature: replace `cxx_std_17` with specific features like `cxx_auto_type` and `cxx_range_for`. Check what standard CMake selects.
5. Add a second target that only needs C++11, and verify that each target uses its own standard by inspecting the build commands.
