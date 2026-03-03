#include "calculator.h"
#include <iostream>
#include <cstdlib>

// Simple test macro
#define ASSERT_EQ(actual, expected, msg) \
    if ((actual) != (expected)) { \
        std::cerr << "FAIL: " << msg << " (expected " << expected << ", got " << actual << ")" << std::endl; \
        return EXIT_FAILURE; \
    } else { \
        std::cout << "PASS: " << msg << std::endl; \
    }

int main() {
    ASSERT_EQ(add(2, 3), 5, "2 + 3 = 5");
    ASSERT_EQ(add(-1, 1), 0, "-1 + 1 = 0");
    ASSERT_EQ(subtract(5, 3), 2, "5 - 3 = 2");
    ASSERT_EQ(multiply(4, 3), 12, "4 * 3 = 12");

    std::cout << "All tests passed!" << std::endl;
    return EXIT_SUCCESS;
}
