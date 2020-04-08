#include "Interpreter.h"
#include "TreeTags.h"
#include "LibraryFunctions.h"

#include <iostream>
#include <cassert>

#define NIL_VAL Value(NilTypeValue::Nil);

#define EVAL_CHILD() dispatcher.Eval(*node[AST_TAG_CHILD]->ToObject_NoConst())

#define EVAL(type) dispatcher.Eval(*node[type]->ToObject_NoConst())

#define ASSERT_TYPE(type) assert(node[AST_TAG_TYPE_KEY]->ToString() == type);

extern int yylineno;

void Interpreter::RuntimeError(const std::string & msg) {
    std::cerr << "Runtime Error at line " << yylineno << ": " << msg << std::endl;
    exit(EXIT_FAILURE);
}

const Value * Interpreter::LookupCurrentScope(const std::string & symbol) const {
    assert(!symbol.empty());

    /* TODO: Use $previous for a valid lookup */
    if (!currentScope->ElementExists(symbol)) return nullptr;
    else return currentScope->operator[](symbol);
}

const Value * Interpreter::LookupGlobalScope(const std::string & symbol) const {
    assert(!symbol.empty());

    /* TODO: Find a solution for the slicing problem */
    if (!globalScope->ElementExists(symbol)) return nullptr;
    else return globalScope->operator[](symbol);
}

const Value * Interpreter::LookupAllScopes(const std::string & symbol) const {
    assert(!symbol.empty());

    /* TODO: Use $puter AND $previous for a valid lookup */
    if (!currentScope->ElementExists(symbol)) return nullptr;
    else return currentScope->operator[](symbol);
}

bool Interpreter::IsLibFunc(const std::string & symbol) const {
    assert(!symbol.empty());

    if (!globalScope->ElementExists(symbol)) return false;
    else return globalScope->operator[](symbol)->IsLibraryFunction();
}

void Interpreter::InstallEvaluators(void) {
    dispatcher.Install(AST_TAG_PROGRAM, [this](Object & node) -> Value { return EvalProgram(node); });
    dispatcher.Install(AST_TAG_STMTS, [this](Object & node) -> Value { return EvalStatements(node); });
    dispatcher.Install(AST_TAG_STMT, [this](Object & node) -> Value { return EvalStatement(node); });
    dispatcher.Install(AST_TAG_EXPR, [this](Object & node) -> Value { return EvalExpression(node); });
    dispatcher.Install(AST_TAG_ASSIGN, [this](Object & node) -> Value { return EvalAssign(node); });
    dispatcher.Install(AST_TAG_PLUS, [this](Object & node) -> Value { return EvalPlus(node); });
    dispatcher.Install(AST_TAG_MINUS, [this](Object & node) -> Value { return EvalMinus(node); });
    dispatcher.Install(AST_TAG_MUL, [this](Object & node) -> Value { return EvalMul(node); });
    dispatcher.Install(AST_TAG_DIV, [this](Object & node) -> Value { return EvalDiv(node); });
    dispatcher.Install(AST_TAG_MODULO, [this](Object & node) -> Value { return EvalModulo(node); });
    dispatcher.Install(AST_TAG_TERM, [this](Object & node) -> Value { return EvalTerm(node); });
    dispatcher.Install(AST_TAG_PRIMARY, [this](Object & node) -> Value { return EvalPrimary(node); });
    dispatcher.Install(AST_TAG_LVALUE, [this](Object & node) -> Value { return EvalLValue(node); });
    dispatcher.Install(AST_TAG_CONST, [this](Object & node) -> Value { return EvalConst(node); });
    dispatcher.Install(AST_TAG_NUMBER, [this](Object & node) -> Value { return EvalNumber(node); });
    dispatcher.Install(AST_TAG_STRING, [this](Object & node) -> Value { return EvalString(node); });
    dispatcher.Install(AST_TAG_NILL, [this](Object & node) -> Value { return EvalNill(node); });
    dispatcher.Install(AST_TAG_TRUE, [this](Object & node) -> Value { return EvalTrue(node); });
    dispatcher.Install(AST_TAG_FALSE, [this](Object & node) -> Value { return EvalFalse(node); });
    dispatcher.Install(AST_TAG_ID, [this](Object & node) -> Value { return EvalId(node); });
    dispatcher.Install(AST_TAG_DOUBLECOLON_ID, [this](Object & node) -> Value { return EvalDoubleColon(node); });
    dispatcher.Install(AST_TAG_LOCAL_ID, [this](Object & node) -> Value { return EvalLocal(node); });
}

const Value Interpreter::EvalProgram(Object &node) {
    ASSERT_TYPE(AST_TAG_PROGRAM);
    EVAL_CHILD();
    return NIL_VAL;
}

const Value Interpreter::EvalStatements(Object &node) {
    ASSERT_TYPE(AST_TAG_STMTS);

    for(register unsigned i = 0; i < node.GetNumericSize(); ++i) {
        dispatcher.Eval(*node[i]->ToObject_NoConst());
    }

    return NIL_VAL;
}

const Value Interpreter::EvalStatement(Object &node) {
    ASSERT_TYPE(AST_TAG_STMT);
    EVAL_CHILD();
    return NIL_VAL;
}

const Value Interpreter::EvalExpression(Object &node) {
    ASSERT_TYPE(AST_TAG_EXPR);
    return EVAL_CHILD();
}

const Value Interpreter::EvalAssign(Object &node) {
    ASSERT_TYPE(AST_TAG_ASSIGN);

    auto lvalue = EVAL(AST_TAG_LVALUE);
    auto rvalue = EVAL(AST_TAG_RVALUE);

    currentScope->Set(lvalue.ToString(), rvalue);
    //std::cout << lvalue.ToString() << " = " << rvalue.ToNumber() << std::endl;

    return rvalue;
}

const Value Interpreter::EvalMath(Object & node, MathOp op) {
    assert(node.IsValid());

    auto op1 = EVAL(AST_TAG_FIRST_EXPR);
    auto op2 = EVAL(AST_TAG_SECOND_EXPR);

    if (!op1.IsNumber()) RuntimeError("First operand is not a number in an arithmetic operation");
    if (!op2.IsNumber()) RuntimeError("Second operand is not a number in an arithmetic operation");

    switch(op) {
        case MathOp::Plus: return op1.ToNumber() + op2.ToNumber();
        case MathOp::Minus: return op1.ToNumber() - op2.ToNumber();
        case MathOp::Mul: return op1.ToNumber() * op2.ToNumber();
        case MathOp::Div: return op1.ToNumber() / op2.ToNumber();
        case MathOp::Mod: {
            double val = static_cast<unsigned>(op1.ToNumber()) % static_cast<unsigned>(op2.ToNumber());
            return val;
        };
        default: assert(false);
    }
}

const Value Interpreter::EvalPlus(Object &node) {
    ASSERT_TYPE(AST_TAG_PLUS);
    return EvalMath(node, MathOp::Plus);
}

const Value Interpreter::EvalMinus(Object &node) {
    ASSERT_TYPE(AST_TAG_MINUS);
    return EvalMath(node, MathOp::Minus);
}

const Value Interpreter::EvalMul(Object &node) {
    ASSERT_TYPE(AST_TAG_MUL);
    return EvalMath(node, MathOp::Mul);
}

const Value Interpreter::EvalDiv(Object &node) {
    ASSERT_TYPE(AST_TAG_DIV);
    return EvalMath(node, MathOp::Div);
}

const Value Interpreter::EvalModulo(Object &node) {
    ASSERT_TYPE(AST_TAG_MODULO);
    return EvalMath(node, MathOp::Mod);
}

const Value Interpreter::EvalGreater(Object &node) {
    ASSERT_TYPE(AST_TAG_GREATER);
    return NIL_VAL;
}

const Value Interpreter::EvalLess(Object &node) {
    ASSERT_TYPE(AST_TAG_LESS);
    return NIL_VAL;
}

const Value Interpreter::EvalGreaterEqual(Object &node) {
    ASSERT_TYPE(AST_TAG_GEQUAL);
    return NIL_VAL;
}

const Value Interpreter::EvalLessEqual(Object &node) {
    ASSERT_TYPE(AST_TAG_LEQUAL);
    return NIL_VAL;
}

const Value Interpreter::EvalEqual(Object &node) {
    ASSERT_TYPE(AST_TAG_EQUAL);
    return NIL_VAL;
}

const Value Interpreter::EvalNotEqual(Object &node) {
    ASSERT_TYPE(AST_TAG_NEQUAL);
    return NIL_VAL;
}

const Value Interpreter::EvalAnd(Object &node) {
    ASSERT_TYPE(AST_TAG_AND);
    return NIL_VAL;
}

const Value Interpreter::EvalOr(Object &node) {
    ASSERT_TYPE(AST_TAG_OR);
    return NIL_VAL;
}

const Value Interpreter::EvalTerm(Object &node) {
    ASSERT_TYPE(AST_TAG_TERM);
    return EVAL_CHILD();
}

const Value Interpreter::EvalUnaryMinus(Object &node) {
    ASSERT_TYPE(AST_TAG_UMINUS);
    return NIL_VAL;
}

const Value Interpreter::EvalNot(Object &node) {
    ASSERT_TYPE(AST_TAG_NOT);
    return NIL_VAL;
}

const Value Interpreter::EvalPlusPlusBefore(Object &node) {
    ASSERT_TYPE(AST_TAG_BPLUSPLUS);
    return NIL_VAL;
}

const Value Interpreter::EvalPlusPlusAfter(Object &node) {
    ASSERT_TYPE(AST_TAG_APLUSPLUS);
    return NIL_VAL;
}

const Value Interpreter::EvalMinusMinusBefore(Object &node) {
    ASSERT_TYPE(AST_TAG_BMINUSMINUS);
    return NIL_VAL;
}

const Value Interpreter::EvalMinusMinusAfter(Object &node) {
    ASSERT_TYPE(AST_TAG_AMINUSMINUS);
    return NIL_VAL;
}

const Value Interpreter::EvalPrimary(Object &node) {
    ASSERT_TYPE(AST_TAG_PRIMARY);
    return EVAL_CHILD();
}

const Value Interpreter::EvalLValue(Object &node) {
    ASSERT_TYPE(AST_TAG_LVALUE);
    return EVAL_CHILD();;
}

const Value Interpreter::EvalId(Object &node) {
    ASSERT_TYPE(AST_TAG_ID);

    auto symbol = node[AST_TAG_ID]->ToString();
    if (!LookupAllScopes(symbol)) currentScope->Set(symbol, Value());

    return symbol;
}

const Value Interpreter::EvalLocal(Object &node) {
    ASSERT_TYPE(AST_TAG_LOCAL_ID);

    auto symbol = node[AST_TAG_ID]->ToString();

    if (LookupCurrentScope(symbol)) return symbol;

    if (IsLibFunc(symbol)) RuntimeError("Local variable \"" + symbol + "\" shadows library function");

    currentScope->Set(symbol, Value());

    return NIL_VAL;
}

const Value Interpreter::EvalDoubleColon(Object &node) {
    ASSERT_TYPE(AST_TAG_DOUBLECOLON_ID);

    auto symbol = node[AST_TAG_ID]->ToString();
    if (!LookupGlobalScope(symbol)) RuntimeError("Global symbol \"" + symbol + "\" does not exist");

    return symbol;
}

const Value Interpreter::EvalDollar(Object &node) {
    ASSERT_TYPE(AST_TAG_DOLLAR_ID);
    return NIL_VAL;
}

const Value Interpreter::EvalMember(Object &node) {
    ASSERT_TYPE(AST_TAG_MEMBER);
    return NIL_VAL;
}

const Value Interpreter::EvalDot(Object &node) {
    ASSERT_TYPE(AST_TAG_DOT);
    return NIL_VAL;
}

const Value Interpreter::EvalBracket(Object &node) {
    ASSERT_TYPE(AST_TAG_BRACKET);
    return NIL_VAL;
}

const Value Interpreter::EvalCall(Object &node) {
    ASSERT_TYPE(AST_TAG_CALL);
    return NIL_VAL;
}

const Value Interpreter::EvalCallSuffix(Object &node) {
    ASSERT_TYPE(AST_TAG_CALL_SUFFIX);
    return NIL_VAL;
}

const Value Interpreter::EvalNormalCall(Object &node) {
    ASSERT_TYPE(AST_TAG_NORMAL_CALL);
    return NIL_VAL;
}

const Value Interpreter::EvalMethodCall(Object &node) {
    ASSERT_TYPE(AST_TAG_METHOD_CALL);
    return NIL_VAL;
}

const Value Interpreter::EvalExpressionList(Object &node) {
    ASSERT_TYPE(AST_TAG_ELIST);
    return NIL_VAL;
}

const Value Interpreter::EvalObjectDef(Object &node) {
    ASSERT_TYPE(AST_TAG_OBJECT_DEF);
    return NIL_VAL;
}

const Value Interpreter::EvalIndexed(Object &node) {
    ASSERT_TYPE(AST_TAG_INDEXED);
    return NIL_VAL;
}

const Value Interpreter::EvalIndexedElem(Object &node) {
    ASSERT_TYPE(AST_TAG_INDEXED_ELEM);
    return NIL_VAL;
}

const Value Interpreter::EvalBlock(Object &node) {
    ASSERT_TYPE(AST_TAG_BLOCK);
    return NIL_VAL;
}

const Value Interpreter::EvalFunctionDef(Object &node) {
    ASSERT_TYPE(AST_TAG_FUNCTION_DEF);
    return NIL_VAL;
}

const Value Interpreter::EvalConst(Object &node) {
    ASSERT_TYPE(AST_TAG_CONST);
    return EVAL_CHILD();
}

const Value Interpreter::EvalNumber(Object &node) {
    ASSERT_TYPE(AST_TAG_NUMBER);
    return *node[AST_TAG_VALUE];
}

const Value Interpreter::EvalString(Object &node) {
    ASSERT_TYPE(AST_TAG_STRING);
    return *node[AST_TAG_VALUE];
}

const Value Interpreter::EvalNill(Object &node) {
    ASSERT_TYPE(AST_TAG_NILL);
    return NIL_VAL;
}

const Value Interpreter::EvalTrue(Object &node) {
    ASSERT_TYPE(AST_TAG_TRUE);
    return Value(true);
}

const Value Interpreter::EvalFalse(Object &node) {
    ASSERT_TYPE(AST_TAG_FALSE);
    return Value(false);
}

const Value Interpreter::EvalIdList(Object &node) {
    ASSERT_TYPE(AST_TAG_ID_LIST);
    return NIL_VAL;
}

const Value Interpreter::EvalIf(Object &node) {
    ASSERT_TYPE(AST_TAG_IF);
    return NIL_VAL;
}

const Value Interpreter::EvalWhile(Object &node) {
    ASSERT_TYPE(AST_TAG_WHILE);
    return NIL_VAL;
}

const Value Interpreter::EvalFor(Object &node) {
    ASSERT_TYPE(AST_TAG_FOR);
    return NIL_VAL;
}

const Value Interpreter::EvalReturn(Object &node) {
    ASSERT_TYPE(AST_TAG_RETURN);
    return NIL_VAL;
}

const Value Interpreter::EvalBreak(Object &node) {
    ASSERT_TYPE(AST_TAG_BREAK);
    return NIL_VAL;
}

const Value Interpreter::EvalContinue(Object &node) {
    ASSERT_TYPE(AST_TAG_CONTINUE);
    return NIL_VAL;
}

Interpreter::Interpreter(void) {
    globalScope = new Object();
    currentScope = globalScope;

    /* TODO: Install Library Functions in global scope */
    globalScope->Set("print", Value(LibFunc::Print, "print"));

    InstallEvaluators();
}

void Interpreter::Execute(Object & program) {
    assert(program[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_PROGRAM);
    assert(program.ElementExists(AST_TAG_CHILD));

    std::cout << "Executing program" << std::endl;

    dispatcher.Eval(program);
}

Interpreter::~Interpreter() {
    /* Cleanup */
    dispatcher.Clear();
}