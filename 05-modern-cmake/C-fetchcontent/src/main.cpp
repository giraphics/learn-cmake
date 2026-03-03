#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <fmt/core.h>

int main() {
    // Using nlohmann/json
    nlohmann::json person;
    person["name"] = "CMake Learner";
    person["level"] = "intermediate";
    person["topics"] = {"FetchContent", "dependencies", "modern CMake"};

    std::cout << "=== FetchContent Example ===" << std::endl;
    std::cout << std::endl;

    // Using fmt
    fmt::print("Person: {}\n", person.dump(2));
    fmt::print("Name: {}\n", person["name"].get<std::string>());
    fmt::print("Level: {}\n", person["level"].get<std::string>());

    std::cout << std::endl;
    std::cout << "Topics:" << std::endl;
    for (const auto& topic : person["topics"]) {
        fmt::print("  - {}\n", topic.get<std::string>());
    }

    return 0;
}
