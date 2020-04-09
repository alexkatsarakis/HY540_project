#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <string>

namespace Utilities {

    bool IsZero(double);

    bool IsInt(double);

    bool DoublesAreEqual(double a, double b);

    std::string UnparserFormatEscChars(const std::string &str);

}    // namespace Utilities

#endif
