#ifndef _SYNTAX_PRINTER_H_
#define _SYNTAX_PRINTER_H_

#include "Token.h"

#include <fstream>

class SyntaxPrinter {
private:
    std::ofstream outputFile;

public:
    SyntaxPrinter(const std::string & fileName);

    SyntaxPrinter(void);

    void operator() (const Token & t);

    void operator() (const std::string & str);

    /* A destructor will create many problems! */
};

#endif