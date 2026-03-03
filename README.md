# Learn CMake — A Comprehensive Cross-Platform Tutorial

A beginner-friendly, progressive tutorial for learning **CMake** — the industry-standard C/C++ build system.

> **Target CMake version:** 3.16+
> **Languages:** C++ (C++11 / C++14 / C++17)
> **Platforms:** Windows, Linux, macOS

---

## Prerequisites

| Tool | Minimum Version | Check Command |
|------|----------------|---------------|
| CMake | 3.16+ | `cmake --version` |
| C++ Compiler | GCC 7+ / Clang 5+ / MSVC 2017+ | `g++ --version` or `cl` |
| Make / Ninja | Any recent | `make --version` or `ninja --version` |

### Platform Setup

<details>
<summary><strong>Windows</strong></summary>

1. Install [Visual Studio 2019+](https://visualstudio.microsoft.com/) with "Desktop development with C++" workload
2. Or install [MinGW-w64](https://www.mingw-w64.org/) and add to PATH
3. Install [CMake](https://cmake.org/download/) and add to PATH
4. Verify: open a terminal and run `cmake --version`

</details>

<details>
<summary><strong>Linux (Ubuntu/Debian)</strong></summary>

```bash
sudo apt update
sudo apt install build-essential cmake
```

</details>

<details>
<summary><strong>macOS</strong></summary>

```bash
xcode-select --install
brew install cmake
```

</details>

---

## Table of Contents

### 01 — Basics
| # | Example | Description |
|---|---------|-------------|
| A | [hello-cmake](01-basics/A-hello-cmake/) | Minimal CMake project — your first build |
| B | [hello-headers](01-basics/B-hello-headers/) | Separating headers and sources |
| C | [static-library](01-basics/C-static-library/) | Creating and linking static libraries |
| D | [shared-library](01-basics/D-shared-library/) | Creating and linking shared (dynamic) libraries |
| E | [variables](01-basics/E-variables/) | CMake variables, cache variables, and scope |
| F | [build-type](01-basics/F-build-type/) | Debug, Release, RelWithDebInfo, MinSizeRel |
| G | [compile-flags](01-basics/G-compile-flags/) | Adding compiler flags and warnings |
| H | [third-party-library](01-basics/H-third-party-library/) | Finding and using external libraries |
| I | [cpp-standard](01-basics/I-cpp-standard/) | Setting the C++ standard (11/14/17/20) |
| J | [installing](01-basics/J-installing/) | Install targets and destination paths |

### 02 — Intermediate
| # | Example | Description |
|---|---------|-------------|
| A | [subprojects](02-intermediate/A-subprojects/) | Multi-project builds with `add_subdirectory()` |
| B | [code-generation](02-intermediate/B-code-generation/) | Generating files with `configure_file()` |
| C | [imported-targets](02-intermediate/C-imported-targets/) | Modern imported targets vs. legacy variables |
| D | [generator-expressions](02-intermediate/D-generator-expressions/) | Compile-time conditional logic |
| E | [build-with-ninja](02-intermediate/E-build-with-ninja/) | Using Ninja and other generators |

### 03 — Testing
| # | Example | Description |
|---|---------|-------------|
| A | [ctest-intro](03-testing/A-ctest-intro/) | Introduction to CTest |
| B | [google-test](03-testing/B-google-test/) | GoogleTest integration with FetchContent |
| C | [catch2](03-testing/C-catch2/) | Catch2 integration |

### 04 — Packaging
| # | Example | Description |
|---|---------|-------------|
| A | [install-basics](04-packaging/A-install-basics/) | CPack basics — creating distributable packages |
| B | [version-config](04-packaging/B-version-config/) | Package version and config files |

### 05 — Modern CMake
| # | Example | Description |
|---|---------|-------------|
| A | [target-properties](05-modern-cmake/A-target-properties/) | Target-based approach — the modern way |
| B | [interface-libraries](05-modern-cmake/B-interface-libraries/) | Header-only (interface) libraries |
| C | [fetchcontent](05-modern-cmake/C-fetchcontent/) | Downloading dependencies with FetchContent |

---

## How to Use This Tutorial

### Building Any Example

Every example includes cross-platform build scripts:

**Unix (Linux/macOS):**
```bash
cd 01-basics/A-hello-cmake
chmod +x build.sh
./build.sh
```

**Windows:**
```cmd
cd 01-basics\A-hello-cmake
build.bat
```

**Manual (any platform):**
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Recommended Learning Path

1. **Start with 01-basics** — Work through A to J in order
2. **Move to 02-intermediate** — Builds on basics concepts
3. **Pick your interest** — Testing, Packaging, or Modern CMake can be done in any order

### Conventions Used

- `CMakeLists.txt` files include comments explaining every command
- Each README is self-contained — you don't need to read prior lessons (but it helps)
- Build scripts create an out-of-source `build/` directory to keep things clean
- All examples use **modern CMake** practices (target-based commands over directory-based)

---

## Quick Reference

```
cmake_minimum_required(VERSION 3.16)    # Minimum CMake version
project(MyProject VERSION 1.0)          # Project name and version
add_executable(app src/main.cpp)        # Create executable target
add_library(lib src/lib.cpp)            # Create library target
target_link_libraries(app PRIVATE lib)  # Link library to executable
target_include_directories(app PRIVATE include)  # Add include paths
```

---

## Documentation (MkDocs book)

This repo can be built as a **static book** with [MkDocs](https://www.mkdocs.org/):

```bash
pip install -r requirements-docs.txt
mkdocs serve   # open http://127.0.0.1:8000
mkdocs build   # output in site/
```

The book uses the Material theme and includes all tutorial pages in order (Basics → Intermediate → Testing, Packaging, Modern CMake). Content is pulled from the existing markdown files via `docs/` stubs, so no duplication.

---

## License

This tutorial is provided for educational purposes. Feel free to use, modify, and share.
