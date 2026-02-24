# E-build-with-ninja: CMake Generators and Build Tools

CMake is not a build system itself -- it is a build system generator. When you run `cmake`, it produces build files for a specific backend tool: Makefiles, Ninja files, Visual Studio solutions, Xcode projects, and more. The `-G` flag selects which generator to use. This example explores different generators, with a focus on Ninja, a fast and lightweight build tool that has become the preferred choice for many projects.

## Concepts

- **CMake generators**: The backend that CMake targets when producing build files. Each generator writes files in a format understood by a specific build tool. The generator is selected once at configuration time and cannot be changed without re-running CMake in a clean directory.
- **Single-config vs multi-config generators**: Single-config generators (Makefiles, Ninja) produce build files for one configuration (Debug or Release). You set `CMAKE_BUILD_TYPE` at configure time. Multi-config generators (Visual Studio, Xcode, Ninja Multi-Config) support all configurations in a single build directory, and you choose the configuration at build time with `--config`.
- **Ninja**: A small, fast build tool focused on speed. It was designed to replace Make in the CMake/build-tool workflow. Ninja typically provides faster incremental builds because it has minimal overhead and was designed for parallel execution from the start.
- **The -G flag**: Passed to `cmake` to select the generator. Example: `cmake -G Ninja ..` or `cmake -G "Unix Makefiles" ..`. Use `cmake --help` to see all available generators on your system.
- **CMAKE_GENERATOR and CMAKE_MAKE_PROGRAM**: CMake variables that reflect which generator and build tool are being used. Useful for diagnostic output or conditional logic.

## File Structure

```
E-build-with-ninja/
├── CMakeLists.txt      # Prints the active generator and build tool
├── README.md
├── build.sh            # Auto-detects Ninja, falls back to Make
├── build.bat           # Auto-detects Ninja, falls back to NMake
└── src/
    └── main.cpp        # Simple program that suggests trying generators
```

## Code Walkthrough

### CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.16)
project(build_ninja)

add_executable(build_ninja src/main.cpp)
target_compile_features(build_ninja PRIVATE cxx_std_11)

message(STATUS "CMake Generator: ${CMAKE_GENERATOR}")
message(STATUS "Build tool: ${CMAKE_MAKE_PROGRAM}")
```

The CMakeLists.txt is intentionally minimal. The interesting part is the `message()` calls that display which generator and build tool CMake selected. The generator name comes from the `-G` argument (or the platform default), and the build tool is the actual program that will execute the build (e.g., `ninja`, `make`, `nmake`).

### build.sh

```bash
if command -v ninja &> /dev/null; then
    GENERATOR="Ninja"
else
    GENERATOR="Unix Makefiles"
fi

cmake .. -G "$GENERATOR"
cmake --build .
```

The script checks whether `ninja` is available on the system PATH. If found, it uses the Ninja generator; otherwise, it falls back to Unix Makefiles. This pattern is useful for build scripts that should work on any developer's machine.

### build.bat

The Windows batch script follows the same logic, checking for `ninja` availability and falling back to `NMake Makefiles` if not found.

### Common Generators

| Generator | Platform | Type | Build Tool |
|-----------|----------|------|------------|
| `Ninja` | Cross-platform | Single-config | ninja |
| `Ninja Multi-Config` | Cross-platform | Multi-config | ninja |
| `Unix Makefiles` | Linux/macOS | Single-config | make |
| `NMake Makefiles` | Windows | Single-config | nmake |
| `Visual Studio 17 2022` | Windows | Multi-config | msbuild |
| `Visual Studio 16 2019` | Windows | Multi-config | msbuild |
| `Xcode` | macOS | Multi-config | xcodebuild |
| `MinGW Makefiles` | Windows (MinGW) | Single-config | mingw32-make |

## Building

**With Ninja (if installed):**
```bash
mkdir build && cd build
cmake .. -G Ninja
cmake --build .
```

**With Unix Makefiles:**
```bash
mkdir build && cd build
cmake .. -G "Unix Makefiles"
cmake --build .
```

**With Visual Studio (Windows):**
```batch
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

**Using the provided scripts:**
```bash
# Linux/macOS:
chmod +x build.sh
./build.sh

# Windows:
build.bat
```

**Listing available generators:**
```bash
cmake --help
# Scroll to the "Generators" section at the bottom
```

## Expected Output

During CMake configuration (with Ninja):
```
-- CMake Generator: Ninja
-- Build tool: /usr/bin/ninja
```

During CMake configuration (with Makefiles):
```
-- CMake Generator: Unix Makefiles
-- Build tool: /usr/bin/make
```

When running the program:
```
Built with a specific generator!
Try building with different generators:
  cmake -G Ninja ..
  cmake -G "Unix Makefiles" ..
  cmake -G "Visual Studio 17 2022" ..
```

## Key Takeaways

1. **CMake generates build files; it does not build.** The actual compilation is done by the selected backend tool (Ninja, Make, MSBuild, etc.). The `cmake --build .` command is a portable wrapper that invokes the correct tool.
2. **Ninja is generally faster than Make** for incremental builds because it was designed with minimal overhead, better dependency tracking, and parallel execution as a default. For large projects, the difference can be significant.
3. **Multi-config generators do not use CMAKE_BUILD_TYPE.** With Visual Studio or Xcode, you select the configuration at build time (`cmake --build . --config Release`), not at configure time. Setting `CMAKE_BUILD_TYPE` has no effect with these generators.
4. **The generator is locked in at configure time.** Once you run `cmake -G Ninja ..`, the build directory is bound to Ninja. To switch generators, delete the build directory (or use a different one) and re-run cmake.
5. **cmake --build is your friend.** Regardless of the generator, `cmake --build .` will invoke the correct build tool. This makes build scripts portable across generators.

## Exercises

1. Install Ninja on your system (if not already installed) and compare build times between `Ninja` and `Unix Makefiles` on a larger project. Use `time cmake --build .` to measure.
2. Try the `Ninja Multi-Config` generator. Build both Debug and Release from the same build directory using `cmake --build . --config Debug` and `cmake --build . --config Release`.
3. Use `cmake --help` to list all generators available on your system. Try at least two different generators and observe the generated build files (e.g., `build.ninja` vs `Makefile`).
4. Set the `CMAKE_DEFAULT_BUILD_TYPE` or use `CMAKE_BUILD_TYPE` with a single-config generator. Then try the same with a multi-config generator and observe the difference.
5. Explore the `CMAKE_GENERATOR` environment variable, which lets you set a default generator without passing `-G` every time. Set it to `Ninja` and run `cmake ..` without `-G` to verify it picks up the default.
