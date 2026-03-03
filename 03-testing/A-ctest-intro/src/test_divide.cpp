#include "calculator.h"
#include <iostream>
#include <cstdlib>
#include <cmath>

int main() {
    double result = divide(10, 3);
    if (std::abs(result - 3.333333) > 0.001) {
        std::cerr << "FAIL: 10/3 should be ~3.333" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "PASS: 10/3 = " << result << std::endl;

    result = divide(10, 0);
    if (result != 0) {
        std::cerr << "FAIL: divide by zero should return 0" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "PASS: divide by zero handled" << std::endl;

    std::cout << "All divide tests passed!" << std::endl;
    return EXIT_SUCCESS;
}
