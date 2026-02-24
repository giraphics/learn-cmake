#include "greeting/greeting.h"

namespace greeting {
    std::string hello(const std::string& name) {
        return "Hello, " + name + "!";
    }
    std::string goodbye(const std::string& name) {
        return "Goodbye, " + name + "!";
    }
}
