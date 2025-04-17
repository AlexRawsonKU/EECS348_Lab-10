#include "calculator.hpp"

static const bool is_digit(const char c)
{
    // I'm pretty sure that `std::isdigit` isn't an "external library",
    // but just in case...
    return (c >= '0') && (c <= '9');
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