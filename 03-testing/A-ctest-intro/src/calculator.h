#ifndef CALCULATOR_H
#define CALCULATOR_H

inline int add(int a, int b) { return a + b; }
inline int subtract(int a, int b) { return a - b; }
inline int multiply(int a, int b) { return a * b; }

inline double divide(int a, int b) {
    if (b == 0) return 0;  // Simple error handling
    return static_cast<double>(a) / b;
}

#endif
