#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#include "EvalDispatcher.h"
#include "HiddenTags.h"
#include "InterpreterMacros.h"
#include "Object.h"
#include "Symbol.h"
#include "Value.h"
#include "ValueStack.h"

#include <list>
#include <string>

class Interpreter {
private:
    /****** Defines ******/

    struct BreakException {};
    struct ContinueException {};
    struct ReturnException {};

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
    static unsigned lineNumber;
    bool inFunctionScope;    // Do not recreate scope on function body enter. We do not allow shadowing. See EvalCall, EvalBlock.

    Object *currentScope;
    Object *globalScope;

    std::list<Object *> scopeStack;
    std::list<std::string> libraryFuncs;
    ValueStack callStack;

    /****** Lvalue Write Access ******/

    Symbol EvalLvalueWrite(Object &node);
    Symbol EvalMemberWrite(Object &node);
    Symbol EvalDotWrite(Object &node);
    Symbol EvalBracketWrite(Object &node);
    Symbol EvalIdWrite(Object &node);
    Symbol EvalGlobalIdWrite(Object &node);
    Symbol EvalLocalIdWrite(Object &node);
    Symbol EvalFormalWrite(Object &node);
    Symbol EvalDollarLambdaWrite(Object &node);
    Symbol TableSetElem(const Value &lvalue, const Value &index);
    Symbol EvalClosureWrite(Object & node);

    /****** Evaluation Helpers ******/

    const Value TableGetElem(const Value &lvalue, const Value &index);
    const Value GetIdName(const Object &node);
    const Value HandleAggregators(Object &node, MathOp op, bool returnChanged);
    const Value EvalMath(Object &node, MathOp op);
    bool ValuesAreEqual(const Value &v1, const Value &v2);
    void AssignToContext(const Symbol &lvalue, const Value &rvalue);
    void RemoveFromContext(const Symbol &lvalue, const Value &rvalue);
    void ChangeClosure(const Symbol & lvalue, const Value & rvalue);
    void CleanupForLoop(Value & elist1, Value & elist2);
    void ValidateAssignment(const Symbol & lvalue, const Value & rvalue);

    /****** Evaluation Side Actions ******/

    void BlockEnter(void);
    void BlockExit(void);

    /****** Function Call Evaluation Helpers ******/

    Value CallProgramFunction(Object &functionAst, Object * functionClosure, const Object &actuals, const std::vector<std::string> &actualNames);
    Value CallLibraryFunction(const std::string &functionId, LibraryFunc functionLib, Object &actuals);
    void ProgramFunctionRuntimeChecks(const Object &formals, const std::vector<std::string> &formalNames, const Object &actuals, const std::vector<std::string> &actualNames);
    std::vector<std::string> GetFormalNames(const Object &formals);

    /****** Symbol Lookup ******/

    const Value *LookupScope(Object *scope, const std::string &symbol) const;
    const Value *LookupCurrentScope(const std::string &symbol) const;
    const Value *LookupGlobalScope(const std::string &symbol) const;
    Object *FindScope(const std::string &symbol) const;
    Object *GetGlobalScope(void) const;
    bool IsGlobalScope(Object *scope) const;

    /****** Environment Actions ******/

    Object *PushScopeSpace(Object *outerScope);
    void PopScopeSpace(void);
    Object *PushSlice(void);
    Object *PushNested(void);
    Object *PopScope(void);
    Object *PushScope(const std::string &tag);

    /****** Start-up ******/

    void InstallEvaluators(void);
    void InstallLibFuncs(void);

    /****** Helpers ******/

    bool IsLibFunc(const std::string &symbol) const;
    bool IsReservedField(const std::string &index) const;
    const Value GetStringFromContext(Object *table, const Value &index, bool lookupFail);
    const Value GetNumberFromContext(Object *table, const Value &index, bool lookupFail);
    Symbol ClosureSetElem(const Value &lvalue, const Value &index);
    Symbol ObjectSetElem(const Value &lvalue, const Value &index);
    unsigned GetLineNumber(void) const;
    void SetLineNumber(unsigned num);

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
    const Value EvalDollarLambda(Object &node); //?!
    const Value EvalMember(Object &node);
    const Value EvalDot(Object &node);
    const Value EvalBracket(Object &node);
    const Value EvalCall(Object &node);
    const Value EvalCallSuffix(Object &node);
    const Value EvalNormalCall(Object &node);
    const Value EvalMethodCall(Object &node);
    const Value EvalArgumentList(Object &node);
    const Value EvalNamedArgument(Object &node);
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
    const Value EvalFormal(Object &node);
    const Value EvalIf(Object &node);
    const Value EvalWhile(Object &node);
    const Value EvalFor(Object &node);
    const Value EvalReturn(Object &node);
    const Value EvalBreak(Object &node);
    const Value EvalContinue(Object &node);

public:
    Interpreter(void);

    void Execute(Object &program);

    static void RuntimeError(const std::string &msg, unsigned line = 0);

    static void RuntimeWarning(const std::string &msg, unsigned line = 0);

    static void Assert(const std::string &msg, unsigned line = 0);

    virtual ~Interpreter();
};

#endif
