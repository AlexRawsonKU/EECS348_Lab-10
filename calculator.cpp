#include "calculator.hpp"

#include <algorithm>

/**
 * Checks if `c` is a character representing a valid ASCII digit.
 */
static const bool is_digit(const char c)
{
    // I'm pretty sure that `std::isdigit` isn't an "external library",
    // but just in case...
    return (c >= '0') && (c <= '9');
}

/**
 * Adds two characters representing digits (along with a carry digit).
 */
static char add_digits(char c1, char c2, char &carry)
{
    char sum = (c1 - '0') + (c2 - '0') + (carry - '0');
    carry = (sum / 10) + '0';
    return (sum % 10) + '0';
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

/**
 * Subtract `lhs - rhs`.
 *
 * The numbers must not have signs,
 * and the strings must have a matching number of characters on each side of their respective decimal point.
 */
static std::string sub_doubles(std::string lhs, std::string rhs)
{

    return "0.0";
}

/**
 * Add two strings as decimals.
 *
 * This function was primarily by ChatGPT due to time constraints (with modifications).
 */
static std::string addDecimalStrings(const std::string &num1, const std::string &num2)
{
    // figure out which/if either of the numbers are negative
    bool isNegative1 = (num1[0] == '-');
    bool isNegative2 = (num2[0] == '-');

    // remove the sign from either string, since we don't need it
    std::string str1 = is_digit(num1.front()) ? num1 : num1.substr(1);
    std::string str2 = is_digit(num2.front()) ? num2 : num2.substr(1);

    // locate the decimal separators in each string
    size_t lhs_dec_idx = str1.find('.');
    size_t rhs_dec_idx = str2.find('.');

    // make sure that if there wasn't a decimal point so that indices are still valid
    if (lhs_dec_idx == std::string::npos)
        lhs_dec_idx = str1.length();
    if (rhs_dec_idx == std::string::npos)
        rhs_dec_idx = str2.length();

    // make sure that the number of digits after the decimal matches
    size_t maxDecimalPlaces = std::max(str1.length() - lhs_dec_idx, str2.length() - rhs_dec_idx) - 1;
    if (str1.length() - lhs_dec_idx - 1 < maxDecimalPlaces)
        str1.append(maxDecimalPlaces - (str1.length() - lhs_dec_idx - 1), '0');
    if (str2.length() - rhs_dec_idx - 1 < maxDecimalPlaces)
        str2.append(maxDecimalPlaces - (str2.length() - rhs_dec_idx - 1), '0');

    // now make sure they match before the decimal too
    size_t maxIntLength = std::max(lhs_dec_idx, rhs_dec_idx);
    str1.insert(0, maxIntLength - lhs_dec_idx, '0');
    str2.insert(0, maxIntLength - rhs_dec_idx, '0');

    // when the signs are different, do subtraction
    if (isNegative1 != isNegative2)
    {
        // always subtract `larger - smaller` and fix sign later
        bool isFirstLarger = (str1 > str2); // character-by-character compare actually does work
        if (!isFirstLarger)
            std::swap(str1, str2);

        int carry = 0;
        std::string result;
        size_t len = str1.size();

        // do the subtraction
        for (int i = len - 1; i >= 0; --i)
        {
            int digit1 = str1[i] - '0';
            int digit2 = (i < str2.size()) ? str2[i] - '0' : 0;
            int diff = digit1 - digit2 - carry;
            carry = (diff < 0) ? 1 : 0; // overflowed, carry a -1
            if (carry)
                diff += 10; // bring the different back into range

            result.push_back('0' + diff);
        }

        // put the string back in order
        std::reverse(result.begin(), result.end());

        // the decimal point is still needed
        result.insert(result.size() - maxDecimalPlaces, 1, '.');

        // add the sign correctly
        if (!isFirstLarger)
            result.insert(0, 1, '-');

        // done subtracting
        return result;
    }

    // signs match, addition will work
    int carry = 0;
    std::string result;
    size_t len = std::max(str1.size(), str2.size());

    for (int i = len - 1; i >= 0; --i)
    {
        int digit1 = (i < str1.size()) ? str1[i] - '0' : 0;
        int digit2 = (i < str2.size()) ? str2[i] - '0' : 0;
        int sum = digit1 + digit2 + carry;
        carry = sum / 10;
        result.push_back('0' + (sum % 10));
    }

    // If there is a carry left over
    if (carry)
        result.push_back('0' + carry);

    std::reverse(result.begin(), result.end());

    // Place the decimal point back
    result.insert(result.size() - maxDecimalPlaces, 1, '.');

    // Add negative sign if both numbers were negative
    if (isNegative1 && isNegative2)
    {
        result.insert(0, 1, '-');
    }

    return result;
}

std::string add_doubles(std::string lhs, std::string rhs)
{
    // separate ChatGPT function out due to shame
    return addDecimalStrings(lhs, rhs);
}