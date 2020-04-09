#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <string>

namespace Utilities {

    bool IsZero(double);

    bool IsInt(double);

    std::string UnparserFormatEscChars(const std::string &str);
    bool DoublesAreEqual(double a, double b);
}

}    // namespace Utilities

#endif
