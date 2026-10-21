#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include <vector>

// Returns true if s matches the format (+|-)A(.B), where A and B are
// non-empty sequences of digits. Pure character checks, no conversion.
bool isValidDouble(const std::string& s);

// Adds two valid double strings and returns the sum as a string.
// All arithmetic is done digit by digit on strings.
std::string addStrings(const std::string& a, const std::string& b);

// Reads every line of the file into `lines`. Returns false if the file
// cannot be opened.
bool loadLines(const std::string& filename, std::vector<std::string>& lines);

// Splits a line into whitespace-separated tokens.
std::vector<std::string> splitTokens(const std::string& line);

#endif
