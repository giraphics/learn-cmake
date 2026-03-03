#include <iostream>
#include "greeter.h"

int main() {
    Greeter g("Modern CMake");
    std::cout << g.greet() << std::endl;
    std::cout << g.farewell() << std::endl;
    return 0;
}
