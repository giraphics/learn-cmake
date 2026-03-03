#include <iostream>
#include <vector>
#include "mathlib/math_utils.h"

int main() {
    std::cout << "Interface (Header-Only) Library Example" << std::endl;
    std::cout << std::endl;

    // Compile-time computation
    std::cout << "5! = " << mathlib::Factorial<5>::value << std::endl;

    // Runtime utilities
    std::cout << "Circle area (r=3): " << mathlib::circle_area(3.0) << std::endl;

    std::vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0};
    std::cout << "Average: " << mathlib::average(data) << std::endl;

    std::cout << "clamp(15, 0, 10): " << mathlib::clamp(15, 0, 10) << std::endl;

    return 0;
}
