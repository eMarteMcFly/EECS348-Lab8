#include "calculator.h"

#include <fstream>
#include <sstream>

namespace {

bool isDigit(char c) { return c >= '0' && c <= '9'; }

// A number broken into its parts, all kept as strings.
struct Parts {
    bool negative;
    std::string intPart;   // no leading zeros (at least "0")
    std::string fracPart;  // no trailing zeros (may be empty)
};

// Splits a valid number string into sign, integer digits and fraction digits,
// stripping leading zeros from the integer part and trailing zeros from the
// fraction part.
Parts splitNumber(const std::string& s) {
    Parts p;
    p.negative = false;
    size_t i = 0;
    if (s[i] == '+' || s[i] == '-') {
        p.negative = (s[i] == '-');
        i++;
    }

    size_t dot = s.find('.', i);
    if (dot == std::string::npos) {
        p.intPart = s.substr(i);
        p.fracPart = "";
    } else {
        p.intPart = s.substr(i, dot - i);
        p.fracPart = s.substr(dot + 1);
    }

    size_t firstNonZero = 0;
    while (firstNonZero + 1 < p.intPart.size() && p.intPart[firstNonZero] == '0') {
        firstNonZero++;
    }
    p.intPart = p.intPart.substr(firstNonZero);

    while (!p.fracPart.empty() && p.fracPart.back() == '0') {
        p.fracPart.pop_back();
    }
    return p;
}

// Compares two non-negative digit strings of equal length.
// Returns -1, 0 or 1.
int compareDigits(const std::string& a, const std::string& b) {
    for (size_t i = 0; i < a.size(); i++) {
        if (a[i] < b[i]) return -1;
        if (a[i] > b[i]) return 1;
    }
    return 0;
}

// Adds two digit strings of equal length. The result may be one digit longer.
std::string addDigits(const std::string& a, const std::string& b) {
    std::string result(a.size(), '0');
    int carry = 0;
    for (int i = static_cast<int>(a.size()) - 1; i >= 0; i--) {
        int sum = (a[i] - '0') + (b[i] - '0') + carry;
        result[i] = static_cast<char>('0' + sum % 10);
        carry = sum / 10;
    }
    if (carry) result = "1" + result;
    return result;
}

// Subtracts b from a (digit strings of equal length, a >= b).
std::string subtractDigits(const std::string& a, const std::string& b) {
    std::string result(a.size(), '0');
    int borrow = 0;
    for (int i = static_cast<int>(a.size()) - 1; i >= 0; i--) {
        int diff = (a[i] - '0') - (b[i] - '0') - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result[i] = static_cast<char>('0' + diff);
    }
    return result;
}

}  // namespace

bool isValidDouble(const std::string& s) {
    if (s.empty()) return false;

    size_t i = 0;
    if (s[i] == '+' || s[i] == '-') i++;

    // A: at least one digit
    size_t intStart = i;
    while (i < s.size() && isDigit(s[i])) i++;
    if (i == intStart) return false;

    if (i == s.size()) return true;  // no decimal point

    // Optional .B: a dot followed by at least one digit
    if (s[i] != '.') return false;
    i++;
    size_t fracStart = i;
    while (i < s.size() && isDigit(s[i])) i++;
    if (i == fracStart) return false;

    return i == s.size();
}

std::string addStrings(const std::string& a, const std::string& b) {
    Parts x = splitNumber(a);
    Parts y = splitNumber(b);

    // Pad the fraction parts on the right and the integer parts on the left
    // so both numbers have the same shape, then join them into one digit
    // string with an implied decimal point.
    size_t fracLen = x.fracPart.size() > y.fracPart.size() ? x.fracPart.size() : y.fracPart.size();
    size_t intLen = x.intPart.size() > y.intPart.size() ? x.intPart.size() : y.intPart.size();

    x.fracPart.append(fracLen - x.fracPart.size(), '0');
    y.fracPart.append(fracLen - y.fracPart.size(), '0');
    x.intPart.insert(0, intLen - x.intPart.size(), '0');
    y.intPart.insert(0, intLen - y.intPart.size(), '0');

    std::string dx = x.intPart + x.fracPart;
    std::string dy = y.intPart + y.fracPart;

    std::string digits;
    bool negative;
    if (x.negative == y.negative) {
        digits = addDigits(dx, dy);
        negative = x.negative;
    } else {
        int cmp = compareDigits(dx, dy);
        if (cmp == 0) return "0";
        if (cmp > 0) {
            digits = subtractDigits(dx, dy);
            negative = x.negative;
        } else {
            digits = subtractDigits(dy, dx);
            negative = y.negative;
        }
    }

    // Put the decimal point back and tidy up the result.
    std::string intResult = digits.substr(0, digits.size() - fracLen);
    std::string fracResult = digits.substr(digits.size() - fracLen);

    size_t firstNonZero = 0;
    while (firstNonZero + 1 < intResult.size() && intResult[firstNonZero] == '0') {
        firstNonZero++;
    }
    intResult = intResult.substr(firstNonZero);
    if (intResult.empty()) intResult = "0";

    while (!fracResult.empty() && fracResult.back() == '0') {
        fracResult.pop_back();
    }

    std::string result = intResult;
    if (!fracResult.empty()) result += "." + fracResult;
    if (result == "0") return "0";  // never print "-0"
    return negative ? "-" + result : result;
}

bool loadLines(const std::string& filename, std::vector<std::string>& lines) {
    std::ifstream in(filename);
    if (!in) return false;
    std::string line;
    while (std::getline(in, line)) {
        lines.push_back(line);
    }
    return true;
}

std::vector<std::string> splitTokens(const std::string& line) {
    std::vector<std::string> tokens;
    std::istringstream ss(line);
    std::string token;
    while (ss >> token) tokens.push_back(token);
    return tokens;
}
