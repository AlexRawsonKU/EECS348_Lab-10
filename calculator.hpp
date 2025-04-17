#ifndef __CALCULATOR_HPP
#define __CALCULATOR_HPP

#include <string>

/**
 * Determine if a given string is a representation of a valid double.
 */
bool validate_double(const std::string &expression) noexcept;

/**
 * Add two doubles in the form of strings.
 *
 * These strings MUST have been validated as doubles before calling this function,
 * or behavior will be undefined.
 */
std::string add_doubles(std::string lhs, std::string rhs);

#endif // __CALCULATOR_HPP
