#include <iostream>
#include <vector>
#include <algorithm>
#include <optional>

// C++17 feature: std::optional
std::optional<int> find_value(const std::vector<int>& v, int target) {
    auto it = std::find(v.begin(), v.end(), target);
    if (it != v.end()) {
        return *it;
    }
    return std::nullopt;
}

int main() {
    // C++11 feature: initializer list
    std::vector<int> numbers = {1, 2, 3, 4, 5};

    // C++11 feature: range-based for loop
    std::cout << "Numbers: ";
    for (const auto& n : numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    // C++14 feature: auto return type deduction (in find_value above uses C++17)
    // C++17 feature: structured bindings with optional
    if (auto result = find_value(numbers, 3); result.has_value()) {
        std::cout << "Found: " << result.value() << std::endl;
    }

    // C++17 feature: if with initializer
    if (auto val = find_value(numbers, 99); !val) {
        std::cout << "99 not found in the vector" << std::endl;
    }

    return 0;
}
