#ifndef GREETING_H
#define GREETING_H

#include <string>

namespace greeting {
    std::string hello(const std::string& name);
    std::string goodbye(const std::string& name);
}

#endif
