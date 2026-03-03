#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <cmath>
#include <numeric>
#include <vector>
#include <algorithm>

namespace mathlib {

// Compile-time factorial
template<int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

template<>
struct Factorial<0> {
    static constexpr int value = 1;
};

// Runtime utilities
inline double circle_area(double radius) {
    return M_PI * radius * radius;
}

inline double average(const std::vector<double>& values) {
    if (values.empty()) return 0.0;
    double sum = std::accumulate(values.begin(), values.end(), 0.0);
    return sum / values.size();
}

template<typename T>
T clamp(T value, T min_val, T max_val) {
    return std::max(min_val, std::min(value, max_val));
}

} // namespace mathlib

#endif
