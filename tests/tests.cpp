#include <gtest/gtest.h>

#include "calculator.hpp"

TEST(CalculatorImplementation, TestBasic)
{
    double expr = parse_number("1");
    EXPECT_NEAR(expr, 1.0, 0.001);
}

TEST(CalculatorImplementation, TestLargeNumber)
{
    double expr = parse_number("12345");
    EXPECT_NEAR(expr, 12345.0, 0.001);
}

TEST(CalculatorImplementation, TestDecimal)
{
    double expr = parse_number("1.0");
    EXPECT_NEAR(expr, 1.0, 0.001);
}

TEST(CalculatorImplementation, TestWithLeadingPlus)
{
    double expr = parse_number("+1");
    EXPECT_NEAR(expr, 1.0, 0.001);
}

TEST(CalculatorImplementation, TestWithLeadingMinus)
{
    double expr = parse_number("-1");
    EXPECT_NEAR(expr, -1.0, 0.001);
}

TEST(CalculatorImplementation, TestWithLeadingZeros)
{
    double expr = parse_number("001");
    EXPECT_NEAR(expr, 1.0, 0.001);
}

TEST(CalculatorImplementation, TestWithLeadingZerosAndPlus)
{
    double expr = parse_number("+001");
    EXPECT_NEAR(expr, 1.0, 0.001);
}

TEST(CalculatorImplementation, TestWithLeadingZerosAndMinus)
{
    double expr = parse_number("-001");
    EXPECT_NEAR(expr, -1.0, 0.001);
}

TEST(CalculatorImplementation, TestWithLeadingPlusDecimal)
{
    double expr = parse_number("+1.0");
    EXPECT_NEAR(expr, 1.0, 0.001);
}

TEST(CalculatorImplementation, TestWithLeadingMinusDecimal)
{
    double expr = parse_number("-1.0");
    EXPECT_NEAR(expr, -1.0, 0.001);
}

TEST(CalculatorImplementation, TestWithLeadingZerosDecimal)
{
    double expr = parse_number("001.0");
    EXPECT_NEAR(expr, 1.0, 0.001);
}

TEST(CalculatorImplementation, TestWithLeadingZerosAndPlusDecimal)
{
    double expr = parse_number("+001.0");
    EXPECT_NEAR(expr, 1.0, 0.001);
}

TEST(CalculatorImplementation, TestWithLeadingZerosAndMinusDecimal)
{
    double expr = parse_number("-001.0");
    EXPECT_NEAR(expr, -1.0, 0.001);
}
