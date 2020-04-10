#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#include "EvalDispatcher.h"
#include "Object.h"
#include "Value.h"

#include <list>
#include <string>
#include <utility> /* For std::pair */
#include <cassert>

class WriteField {
private:
    std::string strVal;
    double numVal;

    bool isNumber = false;
    bool isString = false;

public:

    WriteField(const std::string & str) : strVal(str) {
        isString = true;
        isNumber = false;
    }

    WriteField(double num) : numVal(num) {
        isNumber = true;
        isString = false;
    }

    bool IsString(void) const { return isString; }
    bool IsNumber(void) const { return isNumber; }

    double ToNumber(void) const {
        assert(IsNumber());
        return numVal;
    }

    std::string ToString(void) const {
        assert(IsString());
        return strVal;
    }
};

using Symbol = std::pair<Object *, WriteField>;

class Interpreter {
private:

    class BreakException { };
    class ContinueException { };

    EvalDispatcher dispatcher;

    Object * currentScope;

    Object * globalScope;

    std::list<Object *> scopeStack;

    std::list<std::string> libraryFuncs;

    enum MathOp { Plus, Minus, Mul, Div, Mod, Greater, Less, GreaterEqual, LessEqual };

    Symbol EvalLvalueWrite(Object &node);
    Symbol EvalMemberWrite(Object &node);
    Symbol EvalDotWrite(Object & node);
    Symbol EvalBracketWrite(Object & node);
    Symbol EvalIdWrite(Object & node);

    const Value TableGetElem(const Value lvalue, const Value index);
    Symbol TableSetElem(const Value lvalue, const Value index);

    const Value GetIdName(const Object & node);

    void BlockEnter(void);

    void BlockExit(void);

    void RuntimeError(const std::string & msg);

    const Value HandleAggregators(Object & node, MathOp op, bool returnChanged);

    const Value * LookupScope(Object * scope, const std::string & symbol) const;

    const Value * LookupCurrentScope(const std::string & symbol) const;

    const Value * LookupGlobalScope(const std::string & symbol) const;

    const Value * LookupAllScopes(const std::string & symbol) const;

    Object * FindScope(const std::string & symbol) const;

    bool IsLibFunc(const std::string & symbol) const;

    void InstallEvaluators(void);
    void InstallLibFuncs(void);

    const Value EvalMath(Object & node, MathOp op);

    bool ValuesAreEqual(const Value & v1, const Value & v2);

    const Value EvalProgram(Object &node);
    const Value EvalStatements(Object &node);
    const Value EvalStatement(Object &node);
    const Value EvalExpression(Object &node);
    const Value EvalAssign(Object &node);
    const Value EvalPlus(Object &node);
    const Value EvalMinus(Object &node);
    const Value EvalMul(Object &node);
    const Value EvalDiv(Object &node);
    const Value EvalModulo(Object &node);
    const Value EvalGreater(Object &node);
    const Value EvalLess(Object &node);
    const Value EvalGreaterEqual(Object &node);
    const Value EvalLessEqual(Object &node);
    const Value EvalEqual(Object &node);
    const Value EvalNotEqual(Object &node);
    const Value EvalAnd(Object &node);
    const Value EvalOr(Object &node);
    const Value EvalTerm(Object &node);
    const Value EvalUnaryMinus(Object &node);
    const Value EvalNot(Object &node);
    const Value EvalPlusPlusBefore(Object &node);
    const Value EvalPlusPlusAfter(Object &node);
    const Value EvalMinusMinusBefore(Object &node);
    const Value EvalMinusMinusAfter(Object &node);
    const Value EvalPrimary(Object &node);
    const Value EvalLValue(Object &node);
    const Value EvalId(Object &node);
    const Value EvalLocal(Object &node);
    const Value EvalDoubleColon(Object &node);
    const Value EvalDollar(Object &node);
    const Value EvalMember(Object &node);
    const Value EvalDot(Object &node);
    const Value EvalBracket(Object &node);
    const Value EvalCall(Object &node);
    const Value EvalCallSuffix(Object &node);
    const Value EvalNormalCall(Object &node);
    const Value EvalMethodCall(Object &node);
    const Value EvalExpressionList(Object &node);
    const Value EvalObjectDef(Object &node);
    const Value EvalIndexed(Object &node);
    const Value EvalIndexedElem(Object &node);
    const Value EvalBlock(Object &node);
    const Value EvalFunctionDef(Object &node);
    const Value EvalConst(Object &node);
    const Value EvalNumber(Object &node);
    const Value EvalString(Object &node);
    const Value EvalNill(Object &node);
    const Value EvalTrue(Object &node);
    const Value EvalFalse(Object &node);
    const Value EvalIdList(Object &node);
    const Value EvalIf(Object &node);
    const Value EvalWhile(Object &node);
    const Value EvalFor(Object &node);
    const Value EvalReturn(Object &node);
    const Value EvalBreak(Object &node);
    const Value EvalContinue(Object &node);

public:
    Interpreter(void);

    void Execute(Object & program);

    virtual ~Interpreter();
};

#endif