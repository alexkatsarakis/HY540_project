#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <string>

#ifndef SET_LINE
#define SET_LINE(node) node->Set(LINE_NUMBER_RESERVED_FIELD, double(yylineno))
#endif
#ifndef GET_LINE
#define GET_LINE(node) static_cast<unsigned>(node[LINE_NUMBER_RESERVED_FIELD]->ToNumber())
#endif

namespace Utilities {

    bool IsZero(double);

    bool IsInt(double);

    bool DoublesAreEqual(double a, double b);

    std::string UnparserFormatEscChars(const std::string &str);

    void SyntaxError(const std::string &msg, unsigned line = 0);

    void SyntaxWarning(const std::string &msg, unsigned line = 0);

}    // namespace Utilities

#endif
