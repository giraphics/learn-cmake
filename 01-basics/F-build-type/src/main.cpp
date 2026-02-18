#include <iostream>

int main() {
    std::cout << "Build Type Example" << std::endl;

#ifdef NDEBUG
    std::cout << "NDEBUG is defined (Release mode)" << std::endl;
#else
    std::cout << "NDEBUG is NOT defined (Debug mode)" << std::endl;
#endif

    return 0;
}
