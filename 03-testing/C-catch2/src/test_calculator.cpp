#include "calculator.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Addition works correctly", "[calculator][add]") {
    REQUIRE(Calculator::add(2, 3) == 5);
    REQUIRE(Calculator::add(-1, 1) == 0);
    REQUIRE(Calculator::add(0, 0) == 0);
}

TEST_CASE("Subtraction works correctly", "[calculator][subtract]") {
    REQUIRE(Calculator::subtract(5, 3) == 2);
    REQUIRE(Calculator::subtract(3, 5) == -2);
}

TEST_CASE("Multiplication works correctly", "[calculator][multiply]") {
    REQUIRE(Calculator::multiply(4, 3) == 12);
    REQUIRE(Calculator::multiply(-2, 3) == -6);
    REQUIRE(Calculator::multiply(0, 100) == 0);
}

TEST_CASE("Division works correctly", "[calculator][divide]") {
    REQUIRE(Calculator::divide(10, 2) == 5.0);
    REQUIRE(Calculator::divide(7, 2) == 3.5);
}

TEST_CASE("Division by zero throws", "[calculator][divide]") {
    REQUIRE_THROWS_AS(Calculator::divide(1, 0), std::invalid_argument);
}

TEST_CASE("Calculator scenarios", "[calculator]") {
    SECTION("Adding positive numbers") {
        REQUIRE(Calculator::add(1, 2) == 3);
    }
    SECTION("Adding negative numbers") {
        REQUIRE(Calculator::add(-1, -2) == -3);
    }
    SECTION("Mixed operations") {
        int sum = Calculator::add(10, 5);
        int product = Calculator::multiply(sum, 2);
        REQUIRE(product == 30);
    }
}
