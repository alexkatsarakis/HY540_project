#ifndef _TREE_HOST_H_
#define _TREE_HOST_H_

#include "TreeVisitor.h"
#include <functional>
#include <map>

class Object;

class TreeHost {
private:
    using Acceptor = std::function<void(const Object&)>;
    using Acceptors = std::map<std::string, Acceptor>;
public: // move it later, stupid vs code intellisense
    Acceptors    acceptors;
	TreeVisitor* visitor = nullptr;

    void Accept(const Object& node);
    void InstallAcceptor (const std::string& tag, const Acceptor& f);
	void InstallAllAcceptors (void);
    
    void AcceptStatement(const Object& node);

    void AcceptExpression(const Object& node);
    void AcceptIf(const Object& node);
    void AcceptWhile(const Object& node);
    void AcceptFor(const Object& node);
    void AcceptReturn(const Object& node);
    void AcceptBreak(const Object& node);
    void AcceptContinue(const Object& node);
    void AcceptFunctionDef(const Object& node);
    void AcceptBlock(const Object& node);
    void AcceptEmptyStatement(const Object& node);      /* semicolon */ 
    
    void AcceptAssign(const Object& node);
    void AcceptPlus(const Object& node);
    void AcceptMinus(const Object& node);
    void AcceptMul(const Object& node);
    void AcceptDiv(const Object& node);
    void AcceptModulo(const Object& node);
    void AcceptGreater(const Object& node);
    void AcceptLess(const Object& node);
    void AcceptGreaterEqual(const Object& node);
    void AcceptLessEqual(const Object& node);
    void AcceptEqual(const Object& node);
    void AcceptNotEqual(const Object& node);
    void AcceptAnd(const Object& node);
    void AcceptOr(const Object& node);
    void AcceptTerm(const Object& node);

    void AcceptUnaryMinus(const Object& node);
    void AcceptNot(const Object& node);
    void AcceptPlusPlusBefore(const Object& node);
    void AcceptPlusPlusAfter(const Object& node);
    void AcceptMinusMinusBefore(const Object& node);
    void AcceptMinusMinusAfter(const Object& node);
    void AcceptPrimary(const Object& node);

    void AcceptLvalue(const Object& node);
    void AcceptCall(const Object& node);
    void AcceptObjectDef(const Object& node);
    void AcceptConst(const Object& node);

    void AcceptMember(const Object& node);

    void AcceptCallSuffix(const Object& node);

    void AcceptNormalCall(const Object& node);
    void AcceptMethodCall(const Object& node);
    void AcceptMethodCallId(const Object& node);

};

#endif