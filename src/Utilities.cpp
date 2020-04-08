#include "Utilities.h"

#include <cmath>
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