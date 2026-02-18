#include <iostream>

// This example demonstrates the pattern of finding third-party libraries.
// We use a simple "find or inform" approach that works without installing anything.

int main() {
    std::cout << "Third-party library example" << std::endl;
    std::cout << "This demonstrates the find_package() pattern." << std::endl;

#ifdef HAS_THREADS
    std::cout << "Threading support is available!" << std::endl;
#else
    std::cout << "Threading support was not found." << std::endl;
#endif

    return 0;
}
