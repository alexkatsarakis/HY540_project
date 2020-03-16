#include "SyntaxPrinter.h"
#include "TokenTypes.h"

#include <string>
#include <cassert>
#include <sstream>
#include <exception>

SyntaxPrinter::SyntaxPrinter(const std::string & fileName) {
    outputFile.open(fileName, std::ios::out | std::ios::trunc);

    if (!outputFile.is_open()) throw::std::runtime_error("Could not open file \"" + fileName + "\".");
}

SyntaxPrinter::SyntaxPrinter(void) : SyntaxPrinter("output.txt") { }

void SyntaxPrinter::operator() (const Token & t) {
    assert(outputFile.is_open());

    std::stringstream ss;

    ss << t.GetLineNumber() << ": #"
       << t.GetTokenNumber() << " \""
       << (t.GetContent().empty() ? "" : t.GetContent()) << "\" "
       << t.GetType() << " ";

    const std::string type = t.GetType();

    if(type == C_KEYWORD || type == C_OPERATOR || type == C_PUNCTUATION || type == C_COMMENT) ss << t.GetAttribute();
    else if (type == C_INTCONST || type == C_REALCONST) ss << t.GetContent();
    else if (type == "C_STRING" || type == C_IDENTIFIER) ss << "\"" << t.GetContent() << "\"";

    outputFile << ss.str() << std::endl;
}

void SyntaxPrinter::operator() (const std::string & str) {
    assert(outputFile.is_open());
    outputFile << str << std::endl;
}