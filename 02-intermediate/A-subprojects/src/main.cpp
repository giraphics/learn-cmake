#include <iostream>
#include "greeting/greeting.h"

int main() {
    std::cout << greeting::hello("CMake") << std::endl;
    std::cout << greeting::goodbye("CMake") << std::endl;
    return 0;
}
