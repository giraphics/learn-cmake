#include "greeter.h"

Greeter::Greeter(const std::string& name) : name_(name) {}

std::string Greeter::greet() const {
    return "Hello, " + name_ + "! (using target properties)";
}

std::string Greeter::farewell() const {
    return "Goodbye, " + name_ + "!";
}
