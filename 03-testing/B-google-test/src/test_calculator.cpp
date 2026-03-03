#include "calculator.h"
#include <gtest/gtest.h>

TEST(CalculatorTest, Add) {
    EXPECT_EQ(Calculator::add(2, 3), 5);
    EXPECT_EQ(Calculator::add(-1, 1), 0);
    EXPECT_EQ(Calculator::add(0, 0), 0);
}

TEST(CalculatorTest, Subtract) {
    EXPECT_EQ(Calculator::subtract(5, 3), 2);
    EXPECT_EQ(Calculator::subtract(3, 5), -2);
}

TEST(CalculatorTest, Multiply) {
    EXPECT_EQ(Calculator::multiply(4, 3), 12);
    EXPECT_EQ(Calculator::multiply(-2, 3), -6);
    EXPECT_EQ(Calculator::multiply(0, 100), 0);
}

TEST(CalculatorTest, Divide) {
    EXPECT_DOUBLE_EQ(Calculator::divide(10, 2), 5.0);
    EXPECT_DOUBLE_EQ(Calculator::divide(7, 2), 3.5);
}

TEST(CalculatorTest, DivideByZero) {
    EXPECT_THROW(Calculator::divide(1, 0), std::invalid_argument);
}
