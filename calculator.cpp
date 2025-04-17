#include "calculator.hpp"

#include <algorithm>

/**
 * Checks if `c` is a character representing a valid ASCII digit.
 */
static bool is_digit(const char c)
{
    // I'm pretty sure that `std::isdigit` isn't an "external library",
    // but just in case...
    return (c >= '0') && (c <= '9');
}

/**
 * Ensure that the provided number starts with either `-` or `+`.
 */
static void ensure_signed(std::string &expression)
{
    if (expression[0] == '+' || expression[0] == '-')
    {
        return;
    }
    expression.insert(expression.begin(), '+');
}

/**
 * Determine the length of the pre-decimal and post-decimal halves of a number (which MUST have a sign).
 */
static void part_lengths(const std::string &expression, size_t &integer, size_t &decimal)
{
    integer = expression.find('.');
    if (integer == std::string::npos)
    {
        // decimal not found
        integer = expression.length() - 1; // there is a sign
        decimal = 0;
    }
    else
    {
        // decimal was found
        integer--;                                   // remove the sign
        decimal = expression.length() - integer - 2; // there is a sign AND decimal
    }
}

/**
 * Ensure that there are zeros as needed to make the strings have the right length.
 *
 * It is assumed that the expression has a leading sign.
 */
static void zeropad_number(std::string &expression, size_t integer_size, size_t decimal_size)
{
    // get the current lengths
    size_t current_integer_size = 0, current_decimal_size = 0;
    part_lengths(expression, current_integer_size, current_decimal_size);

    // pad out the beginning of the string
    for (; current_integer_size < integer_size; current_integer_size++)
        expression.insert(++expression.begin(), '0');

    // pad the end of the string
    if (current_decimal_size == 0)
        expression.insert(expression.end(), '.');
    for (; current_decimal_size < decimal_size; current_decimal_size++)
        expression.insert(expression.end(), '0');
}

bool validate_double(const std::string &expression) noexcept
{
    std::string::const_iterator it = expression.cbegin();

    // consume any leading sign
    if (*it == '+' || *it == '-')
        ++it;

    // double always starts with a digit (after a potential sign)
    if (!is_digit(*it))
        return false;

    // consume up until the separator or end of the string
    while (++it != expression.cend() && is_digit(*it))
        continue;

    // if this is the end of the string, we're done
    if (it == expression.cend())
        return true;

    // not the end, so this needs to be the decimal separator
    if (*it != '.')
        return false;
    // decimal separator may not be the last character
    if (++it == expression.cend())
        return false;

    // everything after the separator must be a digit
    do
    {
        if (!is_digit(*it))
            return false;
    } while (++it != expression.cend());

    // reached end successfully, passes test
    return true;
}

std::string add_doubles(std::string lhs, std::string rhs)
{
    // prep the LHS
    ensure_signed(lhs);
    size_t lhs_integer = 0, lhs_decimal = 0;
    part_lengths(lhs, lhs_integer, lhs_decimal);

    // prep the RHS
    ensure_signed(rhs);
    size_t rhs_integer = 0, rhs_decimal = 0;
    part_lengths(rhs, rhs_integer, rhs_decimal);

    // pad each side as needed
    zeropad_number(lhs, std::max(lhs_integer, rhs_integer), std::max(lhs_decimal, rhs_decimal));
    zeropad_number(rhs, std::max(lhs_integer, rhs_integer), std::max(lhs_decimal, rhs_decimal));

    // check the signs
    if (lhs.front() == rhs.front())
    {
        // the signs match, do decimal addition
        signed char carry = 0;
        std::string sum;
        for (size_t idx = lhs.length() - 1; idx > 0; idx--)
        {
            if (lhs[idx] == '.')
            {
                sum.push_back('.');
                continue;
            }
            char result = (lhs[idx] - '0') + (rhs[idx] - '0') + carry;
            carry = result / 10;
            sum.push_back((result % 10) + '0');
        }
        if (carry != 0)
        {
            sum.push_back(carry + '0'); // if there's more carry, add it
        }
        sum.push_back(lhs.front());           // save the sign
        std::reverse(sum.begin(), sum.end()); // fix the order
        return sum;
    }
    else
    {
        // the signs differ, so we need subtraction
        // ensure the subtraction is in order (i.e. `a + -b => a - b`, `-a + b => b - a`)
        bool lhs_is_positive = lhs.front() == '+';
        if (!lhs_is_positive)
            std::swap(lhs, rhs);
        // remove the signs from the numbers to make the math easier
        lhs.replace(0, 1, " ");
        rhs.replace(0, 1, " ");
        // if the result is going to be negative, we need to subtract "backwards"
        bool result_is_negative = lhs < rhs; // comparing the strings left-to-right by byte value works due to 0-padding
        if (result_is_negative)
            std::swap(lhs, rhs); // make sure the results are in order

        signed char carry = 0;
        std::string difference;
        for (size_t idx = lhs.length() - 1; idx > 0; idx--)
        {
            if (lhs[idx] == '.')
            {
                difference.push_back('.');
                continue;
            }
            signed char result = (lhs[idx] - '0') - (rhs[idx] - '0') + carry;
            carry = result / 10;
            // make sure the digit is positive
            if (result < 0)
                result += (carry * 10);
            difference.push_back(result + '0');
        }
        if (carry != 0)
        {
            difference.push_back(std::abs(carry) + '0');
        }
        if (result_is_negative)
        {
            difference.push_back('-');
        }
        std::reverse(difference.begin(), difference.end()); // fix the order
        return difference;
    }
}