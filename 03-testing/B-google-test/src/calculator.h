#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <stdexcept>

class Calculator {
public:
    static int add(int a, int b) { return a + b; }
    static int subtract(int a, int b) { return a - b; }
    static int multiply(int a, int b) { return a * b; }
    static double divide(int a, int b) {
        if (b == 0) throw std::invalid_argument("Division by zero");
        return static_cast<double>(a) / b;
    }
};

#endif
