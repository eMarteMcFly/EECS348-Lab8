#include <iostream>
#include <string>
#include <vector>

#include "calculator.h"

int main(int argc, char* argv[]) {
    std::string filename;
    if (argc >= 2) {
        filename = argv[1];
    } else {
        std::cout << "Enter the input file name: ";
        std::getline(std::cin, filename);
    }

    std::vector<std::string> lines;
    if (!loadLines(filename, lines)) {
        std::cerr << "ERROR: could not open file \"" << filename << "\"" << std::endl;
        return 1;
    }

    for (size_t i = 0; i < lines.size(); i++) {
        std::cout << "line " << (i + 1) << ": ";

        std::vector<std::string> tokens = splitTokens(lines[i]);
        if (tokens.size() != 2) {
            std::cout << "ERROR: expected exactly two numbers" << std::endl;
            continue;
        }

        bool firstValid = isValidDouble(tokens[0]);
        bool secondValid = isValidDouble(tokens[1]);
        if (!firstValid || !secondValid) {
            std::cout << "ERROR: invalid number";
            if (!firstValid) std::cout << " \"" << tokens[0] << "\"";
            if (!secondValid) std::cout << " \"" << tokens[1] << "\"";
            std::cout << std::endl;
            continue;
        }

        std::cout << tokens[0] << " + " << tokens[1] << " = "
                  << addStrings(tokens[0], tokens[1]) << std::endl;
    }

    return 0;
}
