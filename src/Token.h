#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <string>
#include <functional>

class Token {
private:
    unsigned line;
    unsigned number;
    std::string content;
    std::string type;
    std::string attribute;

public:
    typedef std::function<void(const Token &)> VisitorFunction;

    Token(unsigned _line, unsigned _number, const std::string & _content, const std::string & _type, const std::string & _attribute);

    bool IsValid(void) const;

    unsigned GetLineNumber(void) const;

    unsigned GetTokenNumber(void) const;

    const std::string & GetContent(void) const;

    const std::string & GetType(void) const;

    const std::string & GetAttribute(void) const;

    void SetLineNumber(unsigned num);

    void SetTokenNumber(unsigned num);

    void SetContent(const std::string & str);

    void SetType(const std::string & str);

    void SetAttribute(const std::string & str);

    void Accept(VisitorFunction func);

    std::string ToString(void) const;
};

#endif