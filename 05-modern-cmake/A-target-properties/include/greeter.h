#ifndef GREETER_H
#define GREETER_H

#include <string>

class Greeter {
public:
    explicit Greeter(const std::string& name);
    std::string greet() const;
    std::string farewell() const;
private:
    std::string name_;
};

#endif
