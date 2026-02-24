# C-imported-targets: Modern Imported Targets vs Legacy Variables

Before modern CMake, `find_package()` would set loose variables like `SOME_LIB_LIBRARIES` and `SOME_LIB_INCLUDE_DIRS` that you had to manually pass to the right commands. Modern CMake instead creates imported targets (identifiable by the `::` in their name) that bundle everything together: include directories, compiler flags, link libraries, and dependencies. This example demonstrates the modern approach using the Threads library.

## Concepts

- **Imported targets**: Special CMake targets created by `find_package()` that represent external libraries. They are not built by your project but carry all the properties needed to use the library (include paths, link flags, compile definitions, etc.).
- **The :: naming convention**: Imported targets use double colons (e.g., `Threads::Threads`, `Boost::filesystem`, `OpenSSL::SSL`). This convention has a practical benefit: if you misspell the target name, CMake will produce an error at configure time rather than silently treating it as a linker flag.
- **Legacy variables vs modern targets**: The old approach required manually wiring `${LIB_INCLUDE_DIRS}` and `${LIB_LIBRARIES}` into the correct commands. Modern imported targets handle all of this through `target_link_libraries()` alone.
- **find_package()**: This command searches for a package's CMake configuration or Find module. When it succeeds, it typically creates one or more imported targets and/or sets legacy variables.
- **Transitive properties**: Imported targets propagate their requirements automatically. If `Threads::Threads` requires `-pthread` as both a compile and link flag, linking against it applies both automatically.

## File Structure

```
C-imported-targets/
├── CMakeLists.txt      # Uses find_package(Threads) and the imported target
├── README.md
├── build.sh
├── build.bat
└── src/
    └── main.cpp        # Multi-threaded example using std::thread
```

## Code Walkthrough

### CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.16)
project(imported_targets)

find_package(Threads REQUIRED)

add_executable(imported_targets src/main.cpp)
target_link_libraries(imported_targets PRIVATE Threads::Threads)
target_compile_features(imported_targets PRIVATE cxx_std_11)
```

The key line is `target_link_libraries(imported_targets PRIVATE Threads::Threads)`. This single call:
- Links the threading library (e.g., `-lpthread` on Linux).
- Adds any required compile flags (e.g., `-pthread` on GCC).
- Propagates any other necessary properties.

Compare this to the legacy approach, which would require:
```cmake
# Legacy (avoid this):
find_package(Threads REQUIRED)
add_executable(imported_targets src/main.cpp)
target_link_libraries(imported_targets PRIVATE ${CMAKE_THREAD_LIBS_INIT})
# And you might still need to manually add compile flags...
```

The status messages in the CMakeLists.txt print both the legacy variable and a note about the imported target, so you can see both approaches during configuration.

### src/main.cpp

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

std::mutex cout_mutex;

void worker(int id) {
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "Worker " << id << " running on thread "
              << std::this_thread::get_id() << std::endl;
}

int main() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(worker, i);
    }
    for (auto& t : threads) {
        t.join();
    }
    std::cout << "All workers finished!" << std::endl;
    return 0;
}
```

The program spawns four worker threads, each printing its thread ID. A mutex protects `std::cout` from interleaved output. This requires linking against the platform's threading library, which `Threads::Threads` handles portably.

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
```

## Expected Output

During CMake configuration:
```
-- --- Legacy variables ---
-- CMAKE_THREAD_LIBS_INIT = -pthread
--
-- --- Modern imported target ---
-- Threads::Threads is an IMPORTED target
-- It carries includes, flags, and link libraries automatically
```

When running the program:
```
Imported Targets Example
Using modern CMake imported targets
Worker 0 running on thread 140234567890432
Worker 1 running on thread 140234567890688
Worker 2 running on thread 140234567890944
Worker 3 running on thread 140234567891200
All workers finished!
```

The thread IDs and worker order may vary between runs since threads execute concurrently.

## Key Takeaways

1. **Always prefer imported targets** (the `Namespace::Target` form) over legacy variables. They are safer, more complete, and less error-prone.
2. **The :: syntax provides a safety net.** If you write `target_link_libraries(myapp Threads::Thraeds)` (misspelled), CMake will error immediately. With legacy variables, a misspelled `${THREADS_LIBRARIS}` would silently expand to an empty string.
3. **find_package() does double duty.** It both locates the library and creates the imported targets. Check the documentation for each package to see which imported targets it creates.
4. **One line replaces many.** A single `target_link_libraries()` call with an imported target replaces separate calls for include directories, link libraries, and compile flags.
5. **Imported targets are read-only.** You cannot modify their properties with `set_target_properties()`. They represent external dependencies as they are, not as you want them to be.

## Exercises

1. Add `find_package(OpenSSL)` (if available on your system) and link against `OpenSSL::SSL` and `OpenSSL::Crypto`. Observe how multiple imported targets from the same package work together.
2. Print the properties of the `Threads::Threads` imported target using `get_target_property()`. Try reading `INTERFACE_LINK_LIBRARIES`, `INTERFACE_COMPILE_OPTIONS`, and `IMPORTED_LOCATION`.
3. Intentionally misspell the imported target name (e.g., `Threads::Thread`) and observe the error message CMake produces. Compare this to what happens if you misspell a legacy variable name.
4. Use `if(TARGET Threads::Threads)` to conditionally compile code only when the threading library is available. This pattern is useful for optional dependencies.
5. Increase the thread count to 16 or 32 and experiment with different synchronization primitives (e.g., `std::atomic` instead of `std::mutex`) to see how threading behavior changes.
