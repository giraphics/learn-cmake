# B - Google Test Integration

This example demonstrates how to integrate Google Test (GTest), the most widely used C++
testing framework, into a CMake project. Google Test is fetched automatically at configure
time using `FetchContent`, so no manual installation is required. Tests are discovered
and registered with CTest automatically via `gtest_discover_tests()`.

## Concepts

- **FetchContent for GoogleTest** -- `FetchContent_Declare` + `FetchContent_MakeAvailable`
  downloads and builds GoogleTest as part of your project. The `GIT_TAG` pins to a specific
  release (v1.14.0) for reproducible builds. CMake caches the download, so subsequent
  configures are fast.

- **`GTest::gtest_main`** -- GoogleTest provides CMake imported targets.
  `GTest::gtest_main` links both the GTest library and a default `main()` function, so
  your test files do not need to define `main()`. If you need a custom `main()`, link
  `GTest::gtest` instead and provide your own.

- **`gtest_discover_tests()`** -- Part of CMake's built-in `GoogleTest` module. It runs
  the test executable with `--gtest_list_tests` at build time to discover all `TEST` and
  `TEST_F` entries, then registers each one individually with CTest. This means each
  `TEST()` appears as a separate CTest test.

- **`gtest_force_shared_crt`** -- On Windows with MSVC, GoogleTest defaults to static
  runtime linkage (`/MT`). If your project uses the dynamic runtime (`/MD`, the CMake
  default), this mismatch causes linker errors. Setting `gtest_force_shared_crt ON`
  forces GoogleTest to use `/MD`, matching your project.

- **EXPECT vs ASSERT macros** -- `EXPECT_*` macros record a failure but continue running
  the test, letting you see all failures at once. `ASSERT_*` macros abort the current test
  immediately on failure. Use `EXPECT` by default; use `ASSERT` only when continuing after
  failure would crash or produce meaningless results.

- **TEST macro** -- `TEST(TestSuiteName, TestName)` defines a test. The suite name groups
  related tests. GoogleTest creates a fresh instance for each test, so tests are isolated.

## File Structure

```
B-google-test/
├── CMakeLists.txt            # Fetches GTest, creates test target
├── README.md
├── build.sh                  # Linux/macOS build script
├── build.bat                 # Windows build script
└── src/
    ├── calculator.h           # Calculator class with static methods
    └── test_calculator.cpp    # GTest tests for all Calculator methods
```

## Code Walkthrough

### calculator.h

The `Calculator` class uses static methods for arithmetic. The `divide` method throws
`std::invalid_argument` on division by zero rather than returning a sentinel value. This
is idiomatic C++ and lets us demonstrate GTest's exception-checking macros.

### test_calculator.cpp

```cpp
TEST(CalculatorTest, Add) {
    EXPECT_EQ(Calculator::add(2, 3), 5);
    EXPECT_EQ(Calculator::add(-1, 1), 0);
    EXPECT_EQ(Calculator::add(0, 0), 0);
}
```

Each `TEST` block is an independent test case. `EXPECT_EQ` checks equality and prints
both values on failure. The test suite is `CalculatorTest`; individual tests are `Add`,
`Subtract`, etc.

```cpp
TEST(CalculatorTest, Divide) {
    EXPECT_DOUBLE_EQ(Calculator::divide(10, 2), 5.0);
}
```

`EXPECT_DOUBLE_EQ` compares floating-point values with a tolerance of 4 ULPs (Units in
the Last Place), handling floating-point imprecision automatically.

```cpp
TEST(CalculatorTest, DivideByZero) {
    EXPECT_THROW(Calculator::divide(1, 0), std::invalid_argument);
}
```

`EXPECT_THROW` verifies that the expression throws the expected exception type. The test
fails if no exception is thrown or if a different exception type is thrown.

### CMakeLists.txt

```cmake
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
```

This block fetches GoogleTest from GitHub. The `gtest_force_shared_crt` setting is
critical for Windows MSVC builds.

```cmake
include(GoogleTest)
gtest_discover_tests(calculator_tests)
```

The `GoogleTest` module ships with CMake 3.9+. `gtest_discover_tests` interrogates the
compiled test binary to find all tests and registers each one with CTest individually.

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

Note: The first build takes longer because CMake downloads and compiles GoogleTest.
Subsequent builds reuse the cached content.

## Expected Output

```
Test project /path/to/build
    Start 1: CalculatorTest.Add
1/5 Test #1: CalculatorTest.Add ...............   Passed    0.01 sec
    Start 2: CalculatorTest.Subtract
2/5 Test #2: CalculatorTest.Subtract ..........   Passed    0.01 sec
    Start 3: CalculatorTest.Multiply
3/5 Test #3: CalculatorTest.Multiply ..........   Passed    0.01 sec
    Start 4: CalculatorTest.Divide
4/5 Test #4: CalculatorTest.Divide ............   Passed    0.01 sec
    Start 5: CalculatorTest.DivideByZero
5/5 Test #5: CalculatorTest.DivideByZero ......   Passed    0.01 sec

100% tests passed, 0 tests failed out of 5

Total Test time (real) =   0.05 sec
```

Notice that each `TEST()` macro becomes its own CTest test, thanks to
`gtest_discover_tests()`.

**Common GTest assertions:**

| Macro | Description |
|-------|-------------|
| `EXPECT_EQ(a, b)` | `a == b` (non-fatal) |
| `ASSERT_EQ(a, b)` | `a == b` (fatal -- stops test) |
| `EXPECT_NE(a, b)` | `a != b` |
| `EXPECT_LT(a, b)` | `a < b` |
| `EXPECT_TRUE(cond)` | condition is true |
| `EXPECT_DOUBLE_EQ(a, b)` | floating-point near-equality |
| `EXPECT_THROW(expr, type)` | expression throws expected type |
| `EXPECT_NO_THROW(expr)` | expression does not throw |

## Key Takeaways

1. `FetchContent` eliminates the need to install GoogleTest system-wide. The dependency is
   fetched, built, and linked automatically.

2. `GTest::gtest_main` provides a default `main()`, so test files only need `TEST()` macros.

3. `gtest_discover_tests()` automatically registers each `TEST()` as a separate CTest
   entry, giving you fine-grained test results.

4. On Windows MSVC, always set `gtest_force_shared_crt ON` before
   `FetchContent_MakeAvailable(googletest)` to avoid runtime library mismatches.

5. Prefer `EXPECT_*` over `ASSERT_*` so that a single test can report multiple failures.
   Use `ASSERT_*` only when a failure makes subsequent checks invalid.

6. Pin `GIT_TAG` to a specific release for reproducible builds. Avoid using branch names
   like `main` which change over time.

## Exercises

1. Add a `TEST_F` (test fixture) that creates a `Calculator` instance in `SetUp()` and
   reuses it across tests. Even though the methods are static, this demonstrates the
   fixture pattern.

2. Add a parameterized test using `TEST_P` and `INSTANTIATE_TEST_SUITE_P` that runs the
   `add` function with a table of input/output pairs.

3. Write a test that intentionally fails (e.g., `EXPECT_EQ(Calculator::add(1, 1), 3)`) and
   observe the detailed failure message GTest produces.

4. Run a single test suite using `ctest -R CalculatorTest.Add` and observe the filtered
   output.

5. Run the test executable directly (not through CTest) with `--gtest_filter=*Divide*` to
   run only tests whose names contain "Divide".
