#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#include "EvalDispatcher.h"
#include "Object.h"
#include "Value.h"
#include "WriteField.h"

#include <list>
#include <string>
#include <utility> /* For std::pair */

class Interpreter {
private:
    /****** Defines ******/

    class BreakException {};
    class ContinueException {};

    enum MathOp { Plus,
                  Minus,
                  Mul,
                  Div,
                  Mod,
                  Greater,
                  Less,
                  GreaterEqual,
                  LessEqual };

    /****** Fields ******/

    EvalDispatcher dispatcher;
    Value retvalRegister;

    Object *currentScope;
    Object *globalScope;

    std::list<Object *> scopeStack;
    std::list<std::string> libraryFuncs;

    /****** Lvalue Write Access ******/

    Symbol EvalLvalueWrite(Object &node);
    Symbol EvalMemberWrite(Object &node);
    Symbol EvalDotWrite(Object &node);
    Symbol EvalBracketWrite(Object &node);
    Symbol EvalIdWrite(Object &node);
    Symbol EvalGlobalIdWrite(Object &node);
    Symbol EvalLocalIdWrite(Object &node);
    Symbol TableSetElem(const Value lvalue, const Value index);

    /****** Evaluation Helpers ******/

    const Value TableGetElem(const Value lvalue, const Value index);
    const Value GetIdName(const Object &node);
    const Value HandleAggregators(Object &node, MathOp op, bool returnChanged);
    const Value EvalMath(Object &node, MathOp op);
    bool ValuesAreEqual(const Value &v1, const Value &v2);

    /****** Evaluation Side Actions ******/

    void BlockEnter(void);
    void BlockExit(void);
    const Value CallProgramFunction(Object *functionAst, Object *functionClosure, Object *arguments);
    const Value CallLibraryFunction(const std::string &functionId, LibraryFunc functionLib, Object *arguments);

    /****** Symbol Lookup ******/

    const Value *LookupScope(Object *scope, const std::string &symbol) const;
    const Value *LookupCurrentScope(const std::string &symbol) const;
    const Value *LookupGlobalScope(const std::string &symbol) const;
    Object *FindScope(const std::string &symbol) const;

    /****** Environment Actions ******/
    void PushScopeSpace(Object *scope);
    void PopScopeSpace();
    void PushSlice();
    void PushNested();
    void PopScope();

    /****** Start-up ******/

    void InstallEvaluators(void);
    void InstallLibFuncs(void);

    /****** Helpers ******/

    bool IsLibFunc(const std::string &symbol) const;
    bool IsReservedField(const std::string &index) const;

    /****** Evaluators ******/

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
    const Value EvalNil(Object &node);
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

    void Execute(Object &program);

    void RuntimeError(const std::string &msg);

    virtual ~Interpreter();
};

#endif
