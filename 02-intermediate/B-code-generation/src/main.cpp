#include <iostream>
#include "version.h"

int main() {
    std::cout << PROJECT_NAME << " v" << PROJECT_VERSION << std::endl;
    std::cout << "Version: " << PROJECT_VERSION_MAJOR << "."
              << PROJECT_VERSION_MINOR << "."
              << PROJECT_VERSION_PATCH << std::endl;
    std::cout << "Compiler: " << COMPILER_ID << std::endl;
    std::cout << "Build type: " << BUILD_TYPE << std::endl;
    return 0;
}
