#include <iostream>
#include "mylib/mylib.h"

int main() {
    std::cout << "MyLib version: " << mylib::version() << std::endl;
    std::cout << "compute(5) = " << mylib::compute(5) << std::endl;
    return 0;
}
