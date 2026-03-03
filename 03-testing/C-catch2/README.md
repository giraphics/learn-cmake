# C - Catch2 Integration

This example demonstrates integrating Catch2 v3, a modern C++ testing framework known for
its expressive syntax and zero-configuration setup. Catch2 uses natural expression-based
assertions (`REQUIRE(x == y)` instead of `EXPECT_EQ(x, y)`) and provides a unique
`SECTION` mechanism for sharing setup code between related test cases without fixtures.

## Concepts

- **Catch2 v3 vs v2** -- Catch2 v3 is a proper library (not header-only like v2). It
  requires linking against `Catch2::Catch2WithMain` or `Catch2::Catch2`. The v3 API uses
  separate headers like `<catch2/catch_test_macros.hpp>` instead of the single-header
  approach of v2.

- **`TEST_CASE` and tags** -- `TEST_CASE("description", "[tag1][tag2]")` defines a test.
  Tags in square brackets allow filtering: run `./calculator_tests [add]` to execute only
  tests tagged with `[add]`. Tags replace the test suite concept from GoogleTest.

- **`SECTION`** -- Sections allow branching within a test case. Each `SECTION` block runs
  the entire test case from the top, entering only that specific section. This means code
  before the sections acts as shared setup, and each section is isolated from the others.

- **`REQUIRE` vs `CHECK`** -- `REQUIRE` is fatal: it aborts the test case on failure
  (similar to GoogleTest's `ASSERT`). `CHECK` is non-fatal: it records the failure and
  continues (similar to GoogleTest's `EXPECT`). Use `REQUIRE` for preconditions and
  `CHECK` for independent assertions.

- **`catch_discover_tests()`** -- Analogous to GoogleTest's `gtest_discover_tests()`.
  The `Catch` CMake module (from Catch2's `extras/` directory) provides this function.
  It runs the test binary with `--list-tests` and registers each test case with CTest.

- **`Catch2::Catch2WithMain`** -- Provides a default `main()` function. Link to
  `Catch2::Catch2` instead if you need a custom `main()` with custom configuration.

- **Catch2 vs GoogleTest comparison** -- Catch2 uses natural C++ expressions in assertions
  and decomposes them to show both sides on failure. GoogleTest uses explicit macros like
  `EXPECT_EQ`. Catch2 has `SECTION` for lightweight setup sharing; GoogleTest uses
  `TEST_F` fixtures. Both integrate with CTest via auto-discovery.

## File Structure

```
C-catch2/
├── CMakeLists.txt            # Fetches Catch2 v3, creates test target
├── README.md
├── build.sh                  # Linux/macOS build script
├── build.bat                 # Windows build script
└── src/
    ├── calculator.h           # Calculator class with static methods
    └── test_calculator.cpp    # Catch2 tests with TEST_CASE and SECTION
```

## Code Walkthrough

### test_calculator.cpp

```cpp
TEST_CASE("Addition works correctly", "[calculator][add]") {
    REQUIRE(Calculator::add(2, 3) == 5);
    REQUIRE(Calculator::add(-1, 1) == 0);
    REQUIRE(Calculator::add(0, 0) == 0);
}
```

The string is a human-readable test description. Tags like `[calculator]` and `[add]` let
you filter tests at the command line. `REQUIRE` uses a natural expression (`==`) and
decomposes it on failure to show both the left-hand and right-hand values.

```cpp
TEST_CASE("Division by zero throws", "[calculator][divide]") {
    REQUIRE_THROWS_AS(Calculator::divide(1, 0), std::invalid_argument);
}
```

`REQUIRE_THROWS_AS` checks that the expression throws the specified exception type.

```cpp
TEST_CASE("Calculator scenarios", "[calculator]") {
    SECTION("Adding positive numbers") {
        REQUIRE(Calculator::add(1, 2) == 3);
    }
    SECTION("Adding negative numbers") {
        REQUIRE(Calculator::add(-1, -2) == -3);
    }
    SECTION("Mixed operations") {
        int sum = Calculator::add(10, 5);
        int product = Calculator::multiply(sum, 2);
        REQUIRE(product == 30);
    }
}
```

The test case runs three times, once for each section. Any code before the sections
(not shown here, but possible) would run as shared setup each time. This replaces the
need for test fixtures in many cases.

### CMakeLists.txt

```cmake
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.5.2
)
FetchContent_MakeAvailable(Catch2)
```

Fetches and builds Catch2. Like GoogleTest, this is cached after the first configure.

```cmake
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
```

Catch2's `extras/` directory contains the `Catch.cmake` module that provides
`catch_discover_tests()`. We must add it to the module path before `include(Catch)`.
Note: `catch2_SOURCE_DIR` is lowercase -- this is the variable that `FetchContent`
creates from the lowercase name in `FetchContent_Declare`.

```cmake
target_link_libraries(calculator_tests PRIVATE
    Catch2::Catch2WithMain
)
catch_discover_tests(calculator_tests)
```

`Catch2WithMain` provides both the Catch2 library and a `main()`. Each `TEST_CASE`
becomes an individual CTest entry.

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

The first build downloads and compiles Catch2. Subsequent builds are fast.

## Expected Output

```
Test project /path/to/build
    Start 1: Addition works correctly
1/6 Test #1: Addition works correctly ..........   Passed    0.01 sec
    Start 2: Subtraction works correctly
2/6 Test #2: Subtraction works correctly .......   Passed    0.01 sec
    Start 3: Multiplication works correctly
3/6 Test #3: Multiplication works correctly ....   Passed    0.01 sec
    Start 4: Division works correctly
4/6 Test #4: Division works correctly ..........   Passed    0.01 sec
    Start 5: Division by zero throws
5/6 Test #5: Division by zero throws ...........   Passed    0.01 sec
    Start 6: Calculator scenarios
6/6 Test #6: Calculator scenarios ..............   Passed    0.01 sec

100% tests passed, 0 tests failed out of 6

Total Test time (real) =   0.06 sec
```

**Running with tag filters (directly, not through CTest):**

```bash
./calculator_tests [add]              # Run only tests tagged [add]
./calculator_tests [calculator]       # Run all calculator tests
./calculator_tests ~[divide]          # Exclude tests tagged [divide]
```

**Catch2 assertion reference:**

| Macro | Description |
|-------|-------------|
| `REQUIRE(expr)` | Fatal assertion (aborts test on failure) |
| `CHECK(expr)` | Non-fatal assertion (continues on failure) |
| `REQUIRE_THROWS(expr)` | Expression must throw any exception |
| `REQUIRE_THROWS_AS(expr, type)` | Expression must throw specific type |
| `REQUIRE_NOTHROW(expr)` | Expression must not throw |
| `REQUIRE_THAT(expr, matcher)` | Use Catch2 matchers for complex checks |

## Key Takeaways

1. Catch2 uses natural C++ expressions in assertions (`REQUIRE(a == b)`) and
   automatically decomposes them to show both sides on failure.

2. Tags (`[tag]`) provide flexible test filtering without the rigid suite structure of
   GoogleTest. A test can have multiple tags.

3. `SECTION` blocks share setup code and run independently within a single `TEST_CASE`,
   often eliminating the need for test fixtures.

4. `catch_discover_tests()` requires adding Catch2's `extras/` directory to
   `CMAKE_MODULE_PATH` before calling `include(Catch)`.

5. `Catch2::Catch2WithMain` provides a default `main()`. Use `Catch2::Catch2` if you
   need custom initialization.

6. Catch2 v3 is a compiled library (not header-only). It uses modular headers like
   `<catch2/catch_test_macros.hpp>` rather than a single include.

## Exercises

1. Add `CHECK` assertions alongside `REQUIRE` to see the difference: make one `CHECK`
   fail and observe that subsequent assertions still run.

2. Add nested `SECTION` blocks to test a multi-step workflow. For example, create a section
   that sets up a value, then has sub-sections that test different operations on it.

3. Use Catch2's `GENERATE` macro to create a data-driven test:
   ```cpp
   auto x = GENERATE(1, 2, 3, 4, 5);
   REQUIRE(Calculator::multiply(x, 0) == 0);
   ```

4. Run the test binary directly with `--list-tests` to see all discovered test names,
   then use `-c "section name"` to run a specific section.

5. Add a BDD-style test using `SCENARIO`, `GIVEN`, `WHEN`, `THEN` macros (include
   `<catch2/catch_test_macros.hpp>` -- the BDD macros are built in):
   ```cpp
   SCENARIO("Calculator can add numbers", "[calculator]") {
       GIVEN("Two positive integers") {
           int a = 3, b = 4;
           WHEN("They are added") {
               int result = Calculator::add(a, b);
               THEN("The result is their sum") {
                   REQUIRE(result == 7);
               }
           }
       }
   }
   ```
