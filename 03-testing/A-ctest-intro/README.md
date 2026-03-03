# A - CTest Introduction

This example introduces CTest, CMake's built-in testing framework. CTest provides a
standardized way to define, run, and manage tests in CMake projects without requiring
any external testing library. You write test executables that return zero on success
and non-zero on failure, then register them with CMake using `add_test()`.

## Concepts

- **`enable_testing()`** -- Activates CMake's testing infrastructure for the current
  directory and below. This must be called before any `add_test()` commands. It generates
  the `CTestTestfile.cmake` that CTest reads at runtime.

- **`add_test()`** -- Registers a test with CTest. The `NAME` argument gives the test a
  human-readable name, and `COMMAND` specifies the executable (and optional arguments) to
  run. A test passes when the command returns exit code 0, and fails on any non-zero exit
  code.

- **`set_tests_properties()`** -- Assigns metadata to tests. Common properties include
  `TIMEOUT` (maximum seconds a test may run), `LABELS` (semicolon-separated tags for
  filtering), `WILL_FAIL` (expect the test to return non-zero), and
  `PASS_REGULAR_EXPRESSION` (test passes only if stdout matches a regex).

- **CTest command-line usage** -- `ctest` is run from the build directory.
  `--output-on-failure` prints stdout/stderr only for failing tests. `-V` (verbose) prints
  all output from every test. `-L <regex>` runs only tests whose labels match the pattern.
  `-N` lists tests without running them.

- **Test labeling** -- Labels let you organize tests into categories (e.g., `unit`,
  `integration`, `math`). Run a subset with `ctest -L unit` or exclude labels with
  `ctest -LE integration`.

## File Structure

```
A-ctest-intro/
├── CMakeLists.txt           # Build config with enable_testing() and add_test()
├── README.md
├── build.sh                 # Linux/macOS build script
├── build.bat                # Windows build script
└── src/
    ├── calculator.h          # Header-only calculator functions
    ├── test_calculator.cpp   # Tests for add, subtract, multiply
    └── test_divide.cpp       # Tests for divide (floating-point + zero)
```

## Code Walkthrough

### calculator.h

The header defines four inline arithmetic functions. Using `inline` in a header avoids
multiple-definition linker errors when the header is included by multiple translation
units. The `divide` function returns 0 on division by zero as simple error handling.

### test_calculator.cpp

This file defines a minimal `ASSERT_EQ` macro that compares an actual value to an
expected value. If they differ, the macro prints a failure message to stderr and returns
`EXIT_FAILURE`. CTest interprets any non-zero exit code as a test failure. The `main`
function runs four assertions and returns `EXIT_SUCCESS` if all pass.

### test_divide.cpp

This file tests floating-point division with a tolerance check (`std::abs(result - expected) > 0.001`) and verifies that dividing by zero returns 0. Floating-point comparisons
require an epsilon because of representation imprecision.

### CMakeLists.txt

```cmake
enable_testing()
```

This single line activates CTest. Without it, `add_test()` calls are silently ignored.

```cmake
add_executable(test_calculator src/test_calculator.cpp)
target_include_directories(test_calculator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
```

Each test is a standalone executable. `target_include_directories` ensures the compiler
can find `calculator.h`.

```cmake
add_test(NAME CalculatorTests COMMAND test_calculator)
```

Registers the executable as a named test. CTest will run `test_calculator` and check the
exit code.

```cmake
set_tests_properties(CalculatorTests PROPERTIES
    TIMEOUT 10
    LABELS "unit;math"
)
```

Sets a 10-second timeout (kills the test if it hangs) and assigns two labels for
filtering.

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

**Manual steps:**

```bash
mkdir build && cd build
cmake ..
cmake --build .
ctest --output-on-failure
```

## Expected Output

```
Test project /path/to/build
    Start 1: CalculatorTests
1/2 Test #1: CalculatorTests ..................   Passed    0.01 sec
    Start 2: DivideTests
2/2 Test #2: DivideTests ......................   Passed    0.01 sec

100% tests passed, 0 tests failed out of 2

Total Test time (real) =   0.02 sec
```

With `-V` (verbose), you also see the stdout from each test:

```
PASS: 2 + 3 = 5
PASS: -1 + 1 = 0
PASS: 5 - 3 = 2
PASS: 4 * 3 = 12
All tests passed!
```

**Useful CTest flags:**

| Flag | Description |
|------|-------------|
| `--output-on-failure` | Show output only for failing tests |
| `-V` | Verbose -- show all test output |
| `-N` | List tests without running them |
| `-L <regex>` | Run tests matching a label |
| `-R <regex>` | Run tests matching a name |
| `-j <n>` | Run tests in parallel |
| `-C <config>` | Specify build configuration (Windows multi-config) |

## Key Takeaways

1. `enable_testing()` must appear before any `add_test()` calls.
2. A CTest test is any executable that returns 0 for pass and non-zero for fail.
3. No external framework is needed -- plain C++ executables work with CTest.
4. `set_tests_properties()` lets you set timeouts, labels, expected failures, and regex
   checks on output.
5. Labels provide a lightweight way to categorize and selectively run tests.
6. On Windows multi-config generators (Visual Studio), pass `-C Debug` or `-C Release` to
   `ctest` so it knows which build configuration to use.

## Exercises

1. Add a test for `multiply(0, 0)` and `multiply(-3, -3)` in a new `test_multiply.cpp`.
   Register it with CTest and assign it the label `unit`.

2. Create a test that is expected to fail by using `set_tests_properties(... PROPERTIES
   WILL_FAIL TRUE)`. For example, write a test that deliberately returns `EXIT_FAILURE`.

3. Add a `PASS_REGULAR_EXPRESSION` property to `CalculatorTests` so it only passes if the
   output contains "All tests passed!".

4. Run only the tests labeled `math` using `ctest -L math`. Then try `-R Divide` to run
   only the divide test by name.

5. Use `ctest -j 2` to run both tests in parallel and observe the interleaved output with
   `-V`.
