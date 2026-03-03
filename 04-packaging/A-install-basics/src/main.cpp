#include <iostream>
#include "mathlib.h"

int main() {
    std::cout << "MathLib Package Example" << std::endl;
    std::cout << "2 + 3 = " << mathlib::add(2, 3) << std::endl;
    std::cout << "5 - 1 = " << mathlib::subtract(5, 1) << std::endl;
    std::cout << "Pi = " << mathlib::pi() << std::endl;
    return 0;
}
