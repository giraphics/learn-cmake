#include "mylib/mylib.h"

namespace mylib {
    int compute(int x) { return x * x + 1; }
    const char* version() { return "1.2.0"; }
}
