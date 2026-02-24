#include <iostream>

int main() {
    std::cout << "Built with a specific generator!" << std::endl;
    std::cout << "Try building with different generators:" << std::endl;
    std::cout << "  cmake -G Ninja .." << std::endl;
    std::cout << "  cmake -G \"Unix Makefiles\" .." << std::endl;
    std::cout << "  cmake -G \"Visual Studio 17 2022\" .." << std::endl;
    return 0;
}
