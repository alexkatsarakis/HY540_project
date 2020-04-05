#include "Token.h"

#include <cassert>
#include <functional>
#include <sstream>
#include <string>

/****** Constructor ******/

Token::Token(unsigned _line, unsigned _number, const std::string &_content, const std::string &_type, const std::string &_attribute) {
    line = _line;
    number = _number;
    content = _content;
    type = _type;
    attribute = _attribute;
    assert(IsValid());
}

/****** Verifier ******/

bool Token::IsValid(void) const {
    return !type.empty() && !attribute.empty() && line > 0 && number > 0;
}

/****** Getters ******/

unsigned Token::GetLineNumber(void) const {
    return line;
}

unsigned Token::GetTokenNumber(void) const {
    return number;
}

const std::string &Token::GetContent(void) const {
    return content;
}

const std::string &Token::GetType(void) const {
    return type;
}

const std::string &Token::GetAttribute(void) const {
    return attribute;
}

/****** Setters ******/

void Token::SetLineNumber(unsigned num) {
    assert(num);
    line = num;
    assert(IsValid());
}

void Token::SetTokenNumber(unsigned num) {
    assert(num);
    number = num;
    assert(IsValid());
}

void Token::SetContent(const std::string &str) {
    content = str;
    assert(IsValid());
}

void Token::SetType(const std::string &str) {
    assert(!str.empty());
    type = str;
    assert(IsValid());
}

void Token::SetAttribute(const std::string &str) {
    assert(!str.empty());
    attribute = str;
    assert(IsValid());
}

/****** Visitor ******/

void Token::Accept(VisitorFunction func) {
    assert(func);
    assert(IsValid());
    func(*this);
}

/****** Observer ******/

std::string Token::ToString(void) const {
    assert(IsValid());

    std::stringstream ss;

    ss << line << ": #" << number << " \"" << (content.empty() ? "" : content) << "\" " << type << " " << attribute;
    return ss.str();
}

/****** Destructor ******/

Token::~Token() {
    assert(IsValid());
    line = 0;
    number = 0;
    content.clear();
    type.clear();
    attribute.clear();
}
