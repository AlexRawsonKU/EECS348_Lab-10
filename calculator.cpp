#include "calculator.hpp"

bool validate_double(const std::string &expression) noexcept
{
    std::string::const_iterator it = expression.cbegin();

    // consume any leading sign
    if (*it == '+' || *it == '-')
        ++it;

    // double always starts with a digit (after a potential sign)
    if (!std::isdigit(*it))
        return false;

    // consume up until the separator or end of the string
    while (++it != expression.cend() && std::isdigit(*it))
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
        if (!std::isdigit(*it))
            return false;
    } while (++it != expression.cend());

    // reached end successfully, passes test
    return true;
}