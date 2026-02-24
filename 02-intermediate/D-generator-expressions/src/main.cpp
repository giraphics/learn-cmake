#include <iostream>

int main() {
    std::cout << "Generator Expressions Example" << std::endl;

#ifdef IS_DEBUG
    std::cout << "Configuration: Debug" << std::endl;
#else
    std::cout << "Configuration: Release" << std::endl;
#endif

    return 0;
}
