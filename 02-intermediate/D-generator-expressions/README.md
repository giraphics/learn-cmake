# D-generator-expressions: Conditional Logic at Build Time

Generator expressions are a powerful CMake feature that lets you embed conditional logic that is evaluated at build-system generation time (or build time for multi-config generators), rather than at configure time. They use a special `$<...>` syntax and are essential for writing CMakeLists files that work correctly across different compilers, platforms, and build configurations within a single build directory.

## Concepts

- **Generator expressions ($<...>)**: Special expressions evaluated during build system generation. Unlike regular CMake variables (evaluated at configure time), generator expressions can adapt to information only available at build time, such as the active configuration in multi-config generators like Visual Studio.
- **Conditional expressions**: The form `$<$<CONDITION:true_value>:result>` evaluates the condition and returns `result` if true, or an empty string if false. Conditions include `CONFIG:Debug`, `CXX_COMPILER_ID:GNU`, `PLATFORM_ID:Linux`, and many more.
- **Informational expressions**: Expressions like `$<TARGET_FILE:target>` that return information about targets, such as file paths, without any conditional logic.
- **BUILD_INTERFACE vs INSTALL_INTERFACE**: Two special generator expressions that control which include paths (or other properties) are used when building within the project versus when the target is consumed after installation. This is critical for creating relocatable packages.
- **Why not just use if()?**: CMake `if()` statements are evaluated once at configure time. For single-config generators (like Makefiles), this works fine. But multi-config generators (Visual Studio, Xcode) support Debug and Release in the same build directory, so `if(CMAKE_BUILD_TYPE STREQUAL "Debug")` would only capture whatever value was set during configuration. Generator expressions correctly handle all configurations.

## File Structure

```
D-generator-expressions/
├── CMakeLists.txt      # Demonstrates various generator expressions
├── README.md
├── build.sh
├── build.bat
└── src/
    └── main.cpp        # Uses IS_DEBUG definition set by generator expression
```

## Code Walkthrough

### CMakeLists.txt

**1. Configuration-based definitions:**
```cmake
target_compile_definitions(genexpr PRIVATE
    $<$<CONFIG:Debug>:IS_DEBUG>
    $<$<CONFIG:Release>:IS_RELEASE>
)
```
When building in Debug mode, `IS_DEBUG` is defined. When building in Release mode, `IS_RELEASE` is defined. In any other configuration, neither is defined. This works correctly even with multi-config generators where both configurations exist in the same build directory.

**2. Compiler-specific flags:**
```cmake
target_compile_options(genexpr PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra>
)
```
Different compilers use different flag syntax. MSVC uses `/W4` for high warning level, while GCC and Clang use `-Wall -Wextra`. Generator expressions let you specify all variants in one place, and only the matching flags are applied.

**3. Platform-specific definitions:**
```cmake
target_compile_definitions(genexpr PRIVATE
    $<$<PLATFORM_ID:Windows>:ON_WINDOWS>
    $<$<PLATFORM_ID:Linux>:ON_LINUX>
    $<$<PLATFORM_ID:Darwin>:ON_MACOS>
)
```
Similar to compiler detection, but for the target platform. Useful for platform-specific code paths.

**4. BUILD_INTERFACE vs INSTALL_INTERFACE:**
```cmake
target_include_directories(genexpr PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
)
```
`BUILD_INTERFACE` paths are used when the target is built as part of the current project. `INSTALL_INTERFACE` paths are used when the target is consumed from an install tree. This ensures that absolute source paths do not leak into installed package configurations.

### src/main.cpp

```cpp
#include <iostream>

int main() {
    std::cout << "Generator Expressions Example" << std::endl;

#ifdef IS_DEBUG
    std::cout << "Configuration: Debug" << std::endl;
#else
    std::cout << "Configuration: Release" << std::endl;
#endif

    return 0;
}
```

The `IS_DEBUG` macro is only defined when the build configuration is Debug, thanks to the generator expression in CMakeLists.txt.

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

**Building with a specific configuration:**
```bash
# Single-config generator (Makefiles, Ninja):
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# Multi-config generator (Visual Studio, Xcode, Ninja Multi-Config):
cmake -B build
cmake --build build --config Debug
cmake --build build --config Release
```

## Expected Output

When built with Debug configuration:
```
Generator Expressions Example
Configuration: Debug
```

When built with Release configuration (or no explicit type):
```
Generator Expressions Example
Configuration: Release
```

During CMake configuration, you will also see:
```
-- Generator expressions are evaluated at build/generate time, not configure time!
```

## Key Takeaways

1. **Generator expressions solve the multi-config problem.** They are the correct way to handle configuration-dependent logic because they are evaluated at build time, not configure time.
2. **The syntax is `$<CONDITION:VALUE>` or `$<$<CONDITION>:VALUE>`.** The outer `$<...>` wraps the entire expression. The inner `$<CONDITION>` evaluates to 1 or 0, and the outer expression returns VALUE when the condition is 1.
3. **Use generator expressions for compiler and platform portability.** Instead of writing separate logic blocks with `if()` for each compiler, put all variants inline with generator expressions.
4. **BUILD_INTERFACE and INSTALL_INTERFACE are essential for libraries.** If you are writing a library that others will install and use, these expressions ensure your package configurations contain correct paths.
5. **Generator expressions cannot be printed with message().** Since `message()` runs at configure time and generator expressions are evaluated later, you cannot easily debug them with `message()`. Use `file(GENERATE ...)` or inspect the generated build files instead.

## Exercises

1. Add a `$<$<BOOL:${SOME_OPTION}>:FEATURE_ENABLED>` definition that conditionally defines a macro based on a CMake option. Pass `-DSOME_OPTION=ON` or `-DSOME_OPTION=OFF` during configuration.
2. Use `$<TARGET_FILE:genexpr>` in a `add_custom_command(POST_BUILD ...)` to print the output file path after building.
3. Combine multiple conditions using `$<AND:...>`, e.g., `$<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU>>:DEBUG_WITH_GCC>`.
4. Use `file(GENERATE OUTPUT "genexpr_info.txt" CONTENT "Compiler: $<CXX_COMPILER_ID>\nConfig: $<CONFIG>\n")` to write a file at generation time and inspect its contents after building.
5. Experiment with `$<IF:condition,true_value,false_value>` (CMake 3.8+) as an alternative to the nested conditional syntax. Rewrite the compiler flags using this form.
