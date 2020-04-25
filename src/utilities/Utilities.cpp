#include "Utilities.h"

#include <cmath>
#include <iostream>
#include <limits>

bool Utilities::IsZero(double num) {
    unsigned val = static_cast<unsigned>(num * 100000.0f);
    return val == 0;
}

bool Utilities::IsInt(double num) {
    double flooredValue = std::floor(std::abs(num));
    double epsilon = std::numeric_limits<double>::epsilon();
    return (std::abs(num - flooredValue) < epsilon);
}

bool Utilities::DoublesAreEqual(double a, double b) {
    double epsilon = std::numeric_limits<double>::epsilon();
    return std::fabs(a - b) < epsilon;
}

std::string Utilities::UnparserFormatEscChars(const std::string &str) {
    std::string out;
    for (const auto &c : str) {
        if (c == '\n')
            out += "\\n";
        else if (c == '\t')
            out += "\\t";
        else if (c == '\"')
            out += "\\\"";
        else if (c == '\\')
            out += "\\\\";
        else
            out += c;
    }
    return out;
}

void Utilities::SyntaxError(const std::string &msg, unsigned line) {
    std::string lineMsg = (line != 0) ? "Line " + std::to_string(line) + " " : "";
    std::cerr << "\033[36;1m"    //CYAN
              << lineMsg
              << "\033[31;1m"    //RED
              << "Syntax Error: "
              << "\033[0m" << msg << std::endl;
    exit(EXIT_FAILURE);
}

void Utilities::SyntaxWarning(const std::string &msg, unsigned line) {
    std::string lineMsg = (line != 0) ? "Line " + std::to_string(line) + " " : "";
    std::cerr << "\033[36;1m"    //CYAN
              << lineMsg
              << "\033[33;1m"    //YELLOW
              << "Syntax Warning: "
              << "\033[0m" << msg << std::endl;
}
