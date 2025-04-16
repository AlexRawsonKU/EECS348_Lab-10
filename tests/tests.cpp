#include <gtest/gtest.h>

#include "calculator.hpp"

TEST(CalculatorImplementation, TestExamples)
{
    EXPECT_TRUE(validate_double("1"));
    EXPECT_TRUE(validate_double("1.0"));
    EXPECT_TRUE(validate_double("+1.0"));
    EXPECT_TRUE(validate_double("+0001.0"));
    EXPECT_TRUE(validate_double("-0001.005"));

    EXPECT_FALSE(validate_double("A"));
    EXPECT_FALSE(validate_double("+-1"));
    EXPECT_FALSE(validate_double("-5."));
    EXPECT_FALSE(validate_double("-5.-5"));
}

TEST(CalculatorImplementation, MoreEdgeCases)
{
    // empty string
    EXPECT_FALSE(validate_double(""));
    // trailing period
    EXPECT_FALSE(validate_double("5."));
    // no leading number
    EXPECT_FALSE(validate_double(".1"));
}