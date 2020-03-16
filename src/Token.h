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

    /****** Constructor ******/

    Token(unsigned _line, unsigned _number, const std::string & _content, const std::string & _type, const std::string & _attribute);

    /****** Verifier ******/

    bool IsValid(void) const;

    /****** Getters ******/

    unsigned GetLineNumber(void) const;

    unsigned GetTokenNumber(void) const;

    const std::string & GetContent(void) const;

    const std::string & GetType(void) const;

    const std::string & GetAttribute(void) const;

    /****** Setters ******/

    void SetLineNumber(unsigned num);

    void SetTokenNumber(unsigned num);

    void SetContent(const std::string & str);

    void SetType(const std::string & str);

    void SetAttribute(const std::string & str);

    /****** Visitor ******/

    void Accept(VisitorFunction func);

    /****** Observer ******/

    std::string ToString(void) const;

    /****** Destructor ******/

    virtual ~Token();
};

#endif