#include "Interpreter.h"
#include "LibraryFunctions.h"
#include "TreeTags.h"
#include "Utilities.h"

#include <algorithm>
#include <cassert>
#include <iostream>

/****** Reserved Fields ******/
#define PREVIOUS_RESERVED_FIELD "$previous"
#define OUTER_RESERVED_FIELD "$outer"
#define RETVAL_RESERVED_FIELD "$retval"
#define CLOSURE_RESERVED_FIELD "$closure"

/****** Macros Shortcuts ******/
#define NIL_VAL Value(NilTypeValue::Nil);
#define EVAL_CHILD() dispatcher.Eval(*node[AST_TAG_CHILD]->ToObject_NoConst())
#define EVAL(type) dispatcher.Eval(*node[type]->ToObject_NoConst())
#define ASSERT_TYPE(type) assert(node[AST_TAG_TYPE_KEY]->ToString() == type);
#define INSTALL(tag, method) dispatcher.Install(tag, [this](Object & node) { return method(node); });
#define INSTALL_WRITE_FUNC(tag, method) dispatcher.InstallWriteFunc(tag, [this](Object & node) { return method(node); });
#define EVAL_WRITE(type) dispatcher.EvalWriteFunc(*node[type]->ToObject_NoConst())

void Interpreter::RuntimeError(const std::string &msg) {
    std::cerr << "\033[31;1m"
              << "Runtime Error: " << msg << "\033[0m" << std::endl;
    exit(EXIT_FAILURE);
}

const Value *Interpreter::LookupScope(Object *scope, const std::string &symbol) const {
    assert(scope && scope->IsValid());
    assert(!symbol.empty());

    /* If this scope contains the symbol simply return it */
    if (scope->ElementExists(symbol)) return (*scope)[symbol];

    /* Check if the scope is sliced */
    while (scope->ElementExists(PREVIOUS_RESERVED_FIELD)) {
        Object *previous = (*scope)[PREVIOUS_RESERVED_FIELD]->ToObject_NoConst();
        if (previous->ElementExists(symbol)) return (*previous)[symbol];
        scope = previous;
    }

    return nullptr;
}

const Value *Interpreter::LookupCurrentScope(const std::string &symbol) const {
    assert(!symbol.empty());
    return LookupScope(currentScope, symbol);
}

const Value *Interpreter::LookupGlobalScope(const std::string &symbol) const {
    assert(!symbol.empty());

    Object * scope = currentScope;
    const Value * value = LookupScope(scope, OUTER_RESERVED_FIELD);
    while(value) {
        scope = value->ToObject_NoConst();
        value = LookupScope(scope, OUTER_RESERVED_FIELD);
    }

    return LookupScope(scope, symbol);
}

Object *Interpreter::FindScope(const std::string &symbol) const {
    assert(!symbol.empty());

    Object *scope = currentScope;

    while (true) {
        if (scope->ElementExists(symbol)) return scope;

        /* Check if the scope is sliced */
        while (scope->ElementExists(PREVIOUS_RESERVED_FIELD)) {
            Object *previous = (*scope)[PREVIOUS_RESERVED_FIELD]->ToObject_NoConst();
            if (previous->ElementExists(symbol)) return previous;
            scope = previous;
        }

        if (!scope->ElementExists(OUTER_RESERVED_FIELD)) return nullptr;

        scope = (*scope)[OUTER_RESERVED_FIELD]->ToObject_NoConst();
    }

    assert(false);
}

bool Interpreter::IsLibFunc(const std::string &symbol) const {
    assert(!symbol.empty());
    return std::find(libraryFuncs.begin(), libraryFuncs.end(), symbol) != libraryFuncs.end();
}

void Interpreter::InstallEvaluators(void) {
    /* Evaluators used for read-only access */
    INSTALL(AST_TAG_PROGRAM, EvalProgram);
    INSTALL(AST_TAG_STMTS, EvalStatements);
    INSTALL(AST_TAG_STMT, EvalStatement);
    INSTALL(AST_TAG_EXPR, EvalExpression);
    INSTALL(AST_TAG_ASSIGN, EvalAssign);
    INSTALL(AST_TAG_PLUS, EvalPlus);
    INSTALL(AST_TAG_MINUS, EvalMinus);
    INSTALL(AST_TAG_MUL, EvalMul);
    INSTALL(AST_TAG_DIV, EvalDiv);
    INSTALL(AST_TAG_MODULO, EvalModulo);
    INSTALL(AST_TAG_GREATER, EvalGreater);
    INSTALL(AST_TAG_LESS, EvalLess);
    INSTALL(AST_TAG_GEQUAL, EvalGreaterEqual);
    INSTALL(AST_TAG_LEQUAL, EvalLessEqual);
    INSTALL(AST_TAG_TERM, EvalTerm);
    INSTALL(AST_TAG_PRIMARY, EvalPrimary);
    INSTALL(AST_TAG_LVALUE, EvalLValue);
    INSTALL(AST_TAG_CONST, EvalConst);
    INSTALL(AST_TAG_NUMBER, EvalNumber);
    INSTALL(AST_TAG_STRING, EvalString);
    INSTALL(AST_TAG_NIL, EvalNil);
    INSTALL(AST_TAG_TRUE, EvalTrue);
    INSTALL(AST_TAG_FALSE, EvalFalse);
    INSTALL(AST_TAG_ID, EvalId);
    INSTALL(AST_TAG_DOUBLECOLON_ID, EvalDoubleColon);
    INSTALL(AST_TAG_LOCAL_ID, EvalLocal);
    INSTALL(AST_TAG_FUNCTION_DEF, EvalFunctionDef);
    INSTALL(AST_TAG_EQUAL, EvalEqual);
    INSTALL(AST_TAG_NEQUAL, EvalNotEqual);
    INSTALL(AST_TAG_AND, EvalAnd);
    INSTALL(AST_TAG_OR, EvalOr);
    INSTALL(AST_TAG_UMINUS, EvalUnaryMinus);
    INSTALL(AST_TAG_NOT, EvalNot);
    INSTALL(AST_TAG_BPLUSPLUS, EvalPlusPlusBefore);
    INSTALL(AST_TAG_APLUSPLUS, EvalPlusPlusAfter);
    INSTALL(AST_TAG_BMINUSMINUS, EvalMinusMinusBefore);
    INSTALL(AST_TAG_AMINUSMINUS, EvalMinusMinusAfter);
    INSTALL(AST_TAG_ELIST, EvalExpressionList);
    INSTALL(AST_TAG_CALL_SUFFIX, EvalCallSuffix);
    INSTALL(AST_TAG_CALL, EvalCall);
    INSTALL(AST_TAG_NORMAL_CALL, EvalNormalCall);
    INSTALL(AST_TAG_BLOCK, EvalBlock);
    INSTALL(AST_TAG_IF, EvalIf);
    INSTALL(AST_TAG_WHILE, EvalWhile);
    INSTALL(AST_TAG_FOR, EvalFor);
    INSTALL(AST_TAG_BREAK, EvalBreak);
    INSTALL(AST_TAG_CONTINUE, EvalContinue);
    INSTALL(AST_TAG_INDEXED_ELEM, EvalIndexedElem);
    INSTALL(AST_TAG_INDEXED, EvalIndexed);
    INSTALL(AST_TAG_OBJECT_DEF, EvalObjectDef);
    INSTALL(AST_TAG_MEMBER, EvalMember);
    INSTALL(AST_TAG_DOT, EvalDot);
    INSTALL(AST_TAG_BRACKET, EvalBracket);
    /* Evaluators used for write access */
    INSTALL_WRITE_FUNC(AST_TAG_LVALUE, EvalLvalueWrite);
    INSTALL_WRITE_FUNC(AST_TAG_MEMBER, EvalMemberWrite);
    INSTALL_WRITE_FUNC(AST_TAG_DOT, EvalDotWrite);
    INSTALL_WRITE_FUNC(AST_TAG_BRACKET, EvalBracketWrite);
    INSTALL_WRITE_FUNC(AST_TAG_ID, EvalIdWrite);
    INSTALL_WRITE_FUNC(AST_TAG_DOUBLECOLON_ID, EvalGlobalIdWrite);
    INSTALL_WRITE_FUNC(AST_TAG_LOCAL_ID, EvalLocalIdWrite);
}

const Value Interpreter::EvalProgram(Object &node) {
    ASSERT_TYPE(AST_TAG_PROGRAM);
    EVAL_CHILD();
    return NIL_VAL;
}

const Value Interpreter::EvalStatements(Object &node) {
    ASSERT_TYPE(AST_TAG_STMTS);

    for (register unsigned i = 0; i < node.GetNumericSize(); ++i) {
        dispatcher.Eval(*node[i]->ToObject_NoConst());
    }

    return NIL_VAL;
}

const Value Interpreter::EvalStatement(Object &node) {
    ASSERT_TYPE(AST_TAG_STMT);
    if (node.ElementExists(AST_TAG_CHILD)) EVAL_CHILD();
    return NIL_VAL;
}

const Value Interpreter::EvalExpression(Object &node) {
    ASSERT_TYPE(AST_TAG_EXPR);
    return EVAL_CHILD();
}

const Value Interpreter::EvalAssign(Object &node) {
    ASSERT_TYPE(AST_TAG_ASSIGN);

    Symbol lvalue = EVAL_WRITE(AST_TAG_LVALUE);
    auto rvalue = EVAL(AST_TAG_RVALUE);

    if (lvalue.second.IsString()) lvalue.first->Set(lvalue.second.ToString(), rvalue);
    else if (lvalue.second.IsNumber()) lvalue.first->Set(lvalue.second.ToNumber(), rvalue);
    else assert(false);

    return rvalue;
}

const Value Interpreter::EvalMath(Object &node, MathOp op) {
    assert(node.IsValid());

    auto op1 = EVAL(AST_TAG_FIRST_EXPR);
    auto op2 = EVAL(AST_TAG_SECOND_EXPR);

    if (!op1.IsNumber()) RuntimeError("First operand is not a number in an arithmetic operation");
    if (!op2.IsNumber()) RuntimeError("Second operand is not a number in an arithmetic operation");

    switch (op) {
        case MathOp::Plus: return op1.ToNumber() + op2.ToNumber();
        case MathOp::Minus: return op1.ToNumber() - op2.ToNumber();
        case MathOp::Mul: return op1.ToNumber() * op2.ToNumber();
        case MathOp::Div: return op1.ToNumber() / op2.ToNumber();
        case MathOp::Greater: return op1.ToNumber() > op2.ToNumber();
        case MathOp::Less: return op1.ToNumber() < op2.ToNumber();
        case MathOp::GreaterEqual: return op1.ToNumber() >= op2.ToNumber();
        case MathOp::LessEqual: return op1.ToNumber() <= op2.ToNumber();
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
    return EvalMath(node, MathOp::Greater);
}

const Value Interpreter::EvalLess(Object &node) {
    ASSERT_TYPE(AST_TAG_LESS);
    return EvalMath(node, MathOp::Less);
}

const Value Interpreter::EvalGreaterEqual(Object &node) {
    ASSERT_TYPE(AST_TAG_GEQUAL);
    return EvalMath(node, MathOp::GreaterEqual);
}

const Value Interpreter::EvalLessEqual(Object &node) {
    ASSERT_TYPE(AST_TAG_LEQUAL);
    return EvalMath(node, MathOp::LessEqual);
}

bool Interpreter::ValuesAreEqual(const Value &v1, const Value &v2) {
    assert(v1.IsValid());
    assert(v2.IsValid());

    /* We do not allow comparisons with Undef */
    if (v1.IsUndef() || v2.IsUndef()) RuntimeError("Undef found in equality operator");
    /* If one of the operands is boolean simply convert the other operand to
     * boolean and compare them. */
    if (v1.IsBoolean() || v2.IsBoolean()) return (static_cast<bool>(v1) == static_cast<bool>(v2));
    /* If one of the operands is nil and the other operand is not boolean then
     * the result is true only if both operands are nil */
    if (v1.IsNil() || v2.IsNil()) return (v1.IsNil() && v2.IsNil());
    /* In any other case the operands must have the same type */
    if (v1.GetType() != v2.GetType()) RuntimeError("Cannot compare operands of different types (" + v1.GetTypeToString() + " and " + v2.GetTypeToString() + ")");

    /* Compare based on type of operands */
    if (v1.IsNumber())
        return Utilities::DoublesAreEqual(v1.ToNumber(), v2.ToNumber());
    else if (v1.IsString())
        return v1.ToString() == v2.ToString();
    else if (v1.IsProgramFunction())
        return v1.ToProgramFunctionAST() == v2.ToProgramFunctionAST();
    else if (v1.IsLibraryFunction())
        return v1.ToLibraryFunction() == v2.ToLibraryFunction();
    else if (v1.IsNativePtr())
        return v1.ToNativePtr() == v2.ToNativePtr();
    else
        assert(false);
}

const Value Interpreter::EvalEqual(Object &node) {
    ASSERT_TYPE(AST_TAG_EQUAL);

    auto op1 = EVAL(AST_TAG_FIRST_EXPR);
    auto op2 = EVAL(AST_TAG_SECOND_EXPR);

    return ValuesAreEqual(op1, op2);
}

const Value Interpreter::EvalNotEqual(Object &node) {
    ASSERT_TYPE(AST_TAG_NEQUAL);

    auto op1 = EVAL(AST_TAG_FIRST_EXPR);
    auto op2 = EVAL(AST_TAG_SECOND_EXPR);

    return !ValuesAreEqual(op1, op2);
}

const Value Interpreter::EvalAnd(Object &node) {
    ASSERT_TYPE(AST_TAG_AND);

    /* Short-circuit evaluation */
    auto op1 = EVAL(AST_TAG_FIRST_EXPR);
    if (!op1) return false;
    auto op2 = EVAL(AST_TAG_SECOND_EXPR);
    return static_cast<bool>(op2);
}

const Value Interpreter::EvalOr(Object &node) {
    ASSERT_TYPE(AST_TAG_OR);

    /* Short-circuit evaluation */
    auto op1 = EVAL(AST_TAG_FIRST_EXPR);
    if (op1) return true;
    auto op2 = EVAL(AST_TAG_SECOND_EXPR);
    return static_cast<bool>(op2);
}

const Value Interpreter::EvalTerm(Object &node) {
    ASSERT_TYPE(AST_TAG_TERM);
    return EVAL_CHILD();
}

const Value Interpreter::EvalUnaryMinus(Object &node) {
    ASSERT_TYPE(AST_TAG_UMINUS);
    const Value val = EVAL_CHILD();
    if (!val.IsNumber()) RuntimeError("Unary minus (-) cannot be applied to " + val.GetTypeToString());
    return -val.ToNumber();
}

const Value Interpreter::EvalNot(Object &node) {
    ASSERT_TYPE(AST_TAG_NOT);
    const Value val = EVAL_CHILD();
    return !static_cast<bool>(val);
}

const Value Interpreter::HandleAggregators(Object &node, MathOp op, bool returnChanged) {
    assert(node.IsValid());

    Symbol lvalue = EVAL_WRITE(AST_TAG_CHILD);
    assert(lvalue.first);

    const Value * value = nullptr;
    if (lvalue.second.IsString()) value = (*lvalue.first)[lvalue.second.ToString()];
    else if (lvalue.second.IsNumber()) value = (*lvalue.first)[lvalue.second.ToNumber()];
    else assert(false);

    if (!value->IsNumber()) RuntimeError("Increment/decrement operators can only be applied to numbers not to " + value->GetTypeToString());

    double number = value->ToNumber();
    double result = number;

    if (op == MathOp::Plus)
        result = number + 1;
    else if (op == MathOp::Minus)
        result = number - 1;
    else
        assert(false);

    if (lvalue.second.IsString()) lvalue.first->Set(lvalue.second.ToString(), result);
    else if (lvalue.second.IsNumber()) lvalue.first->Set(lvalue.second.ToNumber(), result);
    else assert(false);

    if (returnChanged)
        return result;
    else
        return number;
}

const Value Interpreter::EvalPlusPlusBefore(Object &node) {
    ASSERT_TYPE(AST_TAG_BPLUSPLUS);
    return HandleAggregators(node, MathOp::Plus, true);
}

const Value Interpreter::EvalPlusPlusAfter(Object &node) {
    ASSERT_TYPE(AST_TAG_APLUSPLUS);
    return HandleAggregators(node, MathOp::Plus, false);
}

const Value Interpreter::EvalMinusMinusBefore(Object &node) {
    ASSERT_TYPE(AST_TAG_BMINUSMINUS);
    return HandleAggregators(node, MathOp::Minus, true);
}

const Value Interpreter::EvalMinusMinusAfter(Object &node) {
    ASSERT_TYPE(AST_TAG_AMINUSMINUS);
    return HandleAggregators(node, MathOp::Minus, false);
}

const Value Interpreter::EvalPrimary(Object &node) {
    ASSERT_TYPE(AST_TAG_PRIMARY);
    return EVAL_CHILD();
}

Symbol Interpreter::EvalMemberWrite(Object &node) {
    ASSERT_TYPE(AST_TAG_MEMBER);
    return EVAL_WRITE(AST_TAG_CHILD);
}

bool Interpreter::IsReservedField(const std::string & index) const {
    return (index == OUTER_RESERVED_FIELD ||
            index == PREVIOUS_RESERVED_FIELD ||
            index == RETVAL_RESERVED_FIELD ||
            index == CLOSURE_RESERVED_FIELD);
}

Symbol Interpreter::EvalDotWrite(Object & node) {
    ASSERT_TYPE(AST_TAG_DOT);

    auto lvalue = EVAL(AST_TAG_LVALUE);
    auto index = GetIdName(*node[AST_TAG_ID]->ToObject());
    return TableSetElem(lvalue, index);
}

Symbol Interpreter::EvalBracketWrite(Object & node) {
    ASSERT_TYPE(AST_TAG_BRACKET);
    const Value lvalue = EVAL(AST_TAG_LVALUE);
    const Value index = EVAL(AST_TAG_EXPR);
    return TableSetElem(lvalue, index);
}

Symbol Interpreter::TableSetElem(const Value lvalue, const Value index) {
    if (!lvalue.IsObject())
        RuntimeError("Cannot set field \"" + (index.IsString() ? index.ToString() : std::to_string(index.ToNumber())) + "\" of something that is not an object");

    if (!index.IsString() && !index.IsNumber())
        RuntimeError("Keys of objects can only be strings or numbers");

    Object * table = lvalue.ToObject_NoConst();

    if (index.IsNumber()) return Symbol(table, index.ToNumber());
    else if (index.IsString()) return Symbol(table, index.ToString());
    else assert(false);
}

Symbol Interpreter::EvalIdWrite(Object & node) {
    ASSERT_TYPE(AST_TAG_ID);

    std::string name = node[AST_TAG_ID]->ToString();
    Object * scope = FindScope(name);
    if(!scope) scope = currentScope;

    return Symbol(scope, name);
}

Symbol Interpreter::EvalGlobalIdWrite(Object & node){
    ASSERT_TYPE(AST_TAG_DOUBLECOLON_ID);

    std::string symbol = node[AST_TAG_ID]->ToString();
    if (!LookupGlobalScope(symbol)) RuntimeError("Global symbol \"" + symbol + "\" does not exist (Undefined Symbol)");

    return Symbol(globalScope, symbol);
}

Symbol Interpreter::EvalLocalIdWrite(Object & node){
    ASSERT_TYPE(AST_TAG_LOCAL_ID);
    std::string symbol = node[AST_TAG_ID]->ToString();
    return Symbol(currentScope, symbol);
}

Symbol Interpreter::EvalLvalueWrite(Object &node) {
    ASSERT_TYPE(AST_TAG_LVALUE);
    return EVAL_WRITE(AST_TAG_CHILD);
}

const Value Interpreter::EvalLValue(Object &node) {
    ASSERT_TYPE(AST_TAG_LVALUE);
    return EVAL_CHILD();
}

const Value Interpreter::EvalId(Object &node) {
    ASSERT_TYPE(AST_TAG_ID);

    /* Lookup all scopes starting from the current one and moving backwards. If
     * the given symbol is not found in any scope then add in the current one.
     * If the symbol already exists in a scope, we get its value */

    std::string symbol = node[AST_TAG_ID]->ToString();
    Object * scope = FindScope(symbol);

    if (!scope) {
        currentScope->Set(symbol, Value());
        scope = currentScope;
    }

    return (*(*scope)[symbol]);
}

const Value Interpreter::EvalLocal(Object &node) {
    ASSERT_TYPE(AST_TAG_LOCAL_ID);

    /* Lookup the current scope. If the symbol is found then get its value. If
     * nothing is found, we have to create a new symbol in the current scope.
     * Before doing so we must check for a collision with a library function. */

    std::string symbol = node[AST_TAG_ID]->ToString();

    if (LookupCurrentScope(symbol)) {
        return *(*currentScope)[symbol];
    }

    if (IsLibFunc(symbol)) RuntimeError("Local variable \"" + symbol + "\" shadows library function");

    currentScope->Set(symbol, Value());
    return Value();
}

const Value Interpreter::EvalDoubleColon(Object &node) {
    ASSERT_TYPE(AST_TAG_DOUBLECOLON_ID);

    /* Lookup global scope. If a symbol is found then get its value. If none is
     * found: Runtime error */

    std::string symbol = node[AST_TAG_ID]->ToString();
    const Value * val = LookupGlobalScope(symbol);
    if (!val) RuntimeError("Global symbol \"" + symbol + "\" does not exist (Undefined Symbol)");

    return *val;
}

const Value Interpreter::EvalDollar(Object &node) {
    assert(false);
    return NIL_VAL;
}

const Value Interpreter::EvalMember(Object &node) {
    ASSERT_TYPE(AST_TAG_MEMBER);
    return EVAL_CHILD();
}

const Value Interpreter::GetIdName(const Object & node) {
    assert(node.ElementExists(AST_TAG_ID));
    auto name = node[AST_TAG_ID];
    assert(name->IsString());
    return *name;
}

const Value Interpreter::TableGetElem(const Value lvalue, const Value index) {

    if (!index.IsString() && !index.IsNumber()) RuntimeError("Keys of objects can only be strings or numbers");
    if (!lvalue.IsObject()) RuntimeError("Cannot get field \"" + (index.IsString() ? index.ToString() : std::to_string(index.ToNumber())) + "\" of something that is not an object");

    const Object * table = lvalue.ToObject();

    if (index.IsString()) {
        if(!table->ElementExists(index.ToString())) return Value(); // Undef
        else return *(*table)[index.ToString()];
    } else if (index.IsNumber()) {
        if(!table->ElementExists(index.ToNumber())) return Value(); // Undef
        else return *(*table)[index.ToNumber()];
    }

    assert(false);
}

const Value Interpreter::EvalDot(Object &node) {
    ASSERT_TYPE(AST_TAG_DOT);
    const Value lvalue = EVAL(AST_TAG_LVALUE);
    const Value index = GetIdName(*node[AST_TAG_ID]->ToObject());
    return TableGetElem(lvalue, index);
}

const Value Interpreter::EvalBracket(Object &node) {
    ASSERT_TYPE(AST_TAG_BRACKET);
    const Value lvalue = EVAL(AST_TAG_LVALUE);
    const Value index = EVAL(AST_TAG_EXPR);
    return TableGetElem(lvalue, index);
}

// std::vector<Object> Interpreter::FuncEnter(const Value arguments) {
// }

// void Interpreter::FuncCall(const Value function) {
// }

// void Interpreter::FuncExit() {
// }

const Value Interpreter::EvalCall(Object &node) {
    ASSERT_TYPE(AST_TAG_CALL);

    //FUNC_ENTER
    const Value functionVal = EVAL(AST_TAG_FUNCTION);
    const Value argumentsVal = EVAL(AST_TAG_SUFFIX);    //actuals table
    assert(functionVal.IsLibraryFunction() || functionVal.IsProgramFunction());
    assert(argumentsVal.IsObject());
    const Object *arguments = argumentsVal.ToObject_NoConst();

    Object *functionAst = functionVal.ToProgramFunctionAST_NoConst();
    Object *functionClosure = functionVal.ToProgramFunctionClosure_NoConst();

    //CALL
    //new function scope list
    scopeStack.push_front(functionClosure);
    functionClosure->IncreaseRefCounter();

    //new function scope
    Object *scope = new Object();
    scope->Set(OUTER_RESERVED_FIELD, Value(functionClosure));
    functionClosure->IncreaseRefCounter();
    currentScope = scope;
    currentScope->IncreaseRefCounter();

    //formals - actuals mapping
    Object *formals = (*functionAst)[AST_TAG_FUNCTION_FORMALS]->ToObject_NoConst();
    for (register unsigned i = 0; i < formals->GetStringSize(); ++i) {
        std::string namedFormal = (*formals)[i]->ToString();
        Value actualValue = (*arguments)[i];
        currentScope->Set(namedFormal, actualValue);
    }

    //function body evaluation
    dispatcher.Eval(*((*functionAst)[AST_TAG_STMT]->ToObject_NoConst()));

    //RETURN_VAL

    //FUNC_EXIT
    //remove function scope
    currentScope->DecreaseRefCounter();
    currentScope = nullptr;
    functionClosure->DecreaseRefCounter();
    scope->Set(OUTER_RESERVED_FIELD, Value());
    scope->DecreaseRefCounter();

    //remove function scope list
    functionClosure->DecreaseRefCounter();
    scopeStack.pop_front();
    currentScope = functionClosure;

    return NIL_VAL;

    /* if (function.IsLibraryFunction()) {
        function.ToLibraryFunction()(*arguments.ToObject_NoConst());

        if (retvalRegister.IsObject()) retvalRegister.ToObject_NoConst()->DecreaseRefCounter();

        const Value * retval = (*arguments.ToObject())[RETVAL_RESERVED_FIELD];

        /* TODISCUSS: Is this a good practice? */
        if(!retval) retvalRegister.FromUndef();
        else new (&retvalRegister) Value(*retval);

    } else {
        assert(false);
    }

    const Value *retval = (*arguments.ToObject())[RETVAL_RESERVED_FIELD];
    Value result;

    // TODISCUSS: Is this a good practice?
    if (!retval)
        result.FromUndef();
    else
        new (&result) Value(*retval);

    arguments.ToObject_NoConst()->Clear();
    delete arguments.ToObject_NoConst();
    arguments.FromUndef();
 */
    // return result;
}

const Value Interpreter::EvalCallSuffix(Object &node) {
    ASSERT_TYPE(AST_TAG_CALL_SUFFIX);
    return EVAL_CHILD();
}

const Value Interpreter::EvalNormalCall(Object &node) {
    ASSERT_TYPE(AST_TAG_NORMAL_CALL);
    return EVAL_CHILD();
}

const Value Interpreter::EvalMethodCall(Object &node) {
    ASSERT_TYPE(AST_TAG_METHOD_CALL);
    return NIL_VAL;
}

const Value Interpreter::EvalExpressionList(Object &node) {
    ASSERT_TYPE(AST_TAG_ELIST);

    Object *table = new Object();
    for (register unsigned i = 0; i < node.GetNumericSize(); ++i) {
        const Value v = dispatcher.Eval(*node[i]->ToObject_NoConst());
        table->Set(i, v);
    }

    return table;
}

const Value Interpreter::EvalObjectDef(Object &node) {
    ASSERT_TYPE(AST_TAG_OBJECT_DEF);
    return EVAL(AST_TAG_CHILD);
}

const Value Interpreter::EvalIndexed(Object &node) {
    ASSERT_TYPE(AST_TAG_INDEXED);

    Object *table = new Object();
    for (register unsigned i = 0; i < node.GetNumericSize(); ++i) {
        const Value v = dispatcher.Eval(*node[i]->ToObject_NoConst());
        assert(v.IsObject());

        Object *o = v.ToObject_NoConst();
        o->Visit([table](const Value &key, const Value &val) {
            if (key.IsString()) table->Set(key.ToString(), val);
            else if (key.IsNumber()) table->Set(key.ToNumber(), val);
            else assert(false);
        });

        o->Clear();
        delete o;
    }

    return table;
}

const Value Interpreter::EvalIndexedElem(Object &node) {
    ASSERT_TYPE(AST_TAG_INDEXED_ELEM);

    Object *pair = new Object();

    auto key = EVAL(AST_TAG_OBJECT_KEY);
    auto value = EVAL(AST_TAG_OBJECT_VALUE);

    if (key.IsString()) pair->Set(key.ToString(), value);
    else if (key.IsNumber()) pair->Set(key.ToNumber(), value);
    else RuntimeError("Keys of objects can only be strings or numbers");

    return pair;
}

void Interpreter::BlockEnter(void) {
    Object *scope = new Object();
    scope->Set(OUTER_RESERVED_FIELD, currentScope);
    scope->IncreaseRefCounter();
    currentScope = scope;
}

void Interpreter::BlockExit(void) {
    Object *scope = nullptr;
    Object *tmp = nullptr;

    bool shouldSlice = currentScope->ElementExists(PREVIOUS_RESERVED_FIELD);
    while (currentScope->ElementExists(PREVIOUS_RESERVED_FIELD)) {
        tmp = currentScope;
        currentScope = (*currentScope)[PREVIOUS_RESERVED_FIELD]->ToObject_NoConst();
        tmp->DecreaseRefCounter();
    }

    assert(currentScope->ElementExists(OUTER_RESERVED_FIELD));
    tmp = currentScope;
    currentScope = (*currentScope)[OUTER_RESERVED_FIELD]->ToObject_NoConst();
    tmp->DecreaseRefCounter();

    if (shouldSlice) {
        scope = new Object();
        scope->Set(PREVIOUS_RESERVED_FIELD, currentScope);
        scope->IncreaseRefCounter();
        currentScope = scope;
    }
}

const Value Interpreter::EvalBlock(Object &node) {
    ASSERT_TYPE(AST_TAG_BLOCK);

    BlockEnter();
    EVAL_CHILD();
    BlockExit();

    return NIL_VAL;
}

const Value Interpreter::EvalFunctionDef(Object &node) {
    ASSERT_TYPE(AST_TAG_FUNCTION_DEF);

    const Object *child = node[AST_TAG_FUNCTION_ID]->ToObject();
    std::string name = (*child)[AST_TAG_ID]->ToString();

    if (IsLibFunc(name)) RuntimeError("Cannot define function \"" + name + "\". It shadows the library function.");
    if (LookupCurrentScope(name)) RuntimeError("Cannot define function \"" + name + "\". Symbol name already exists.");

    currentScope->Set(name, Value(&node, currentScope));
    currentScope->IncreaseRefCounter();

    Object *slice = new Object();
    slice->Set(PREVIOUS_RESERVED_FIELD, currentScope);
    currentScope = slice;
    currentScope->IncreaseRefCounter();

    return Value(&node, currentScope);
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

const Value Interpreter::EvalNil(Object &node) {
    ASSERT_TYPE(AST_TAG_NIL);
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

    if (EVAL(AST_TAG_CONDITION))
        EVAL(AST_TAG_STMT);
    else if (node.ElementExists(AST_TAG_ELSE_STMT))
        EVAL(AST_TAG_ELSE_STMT);

    return NIL_VAL;
}

const Value Interpreter::EvalWhile(Object &node) {
    ASSERT_TYPE(AST_TAG_WHILE);

    while (EVAL(AST_TAG_CONDITION)) {
        try {
            EVAL(AST_TAG_STMT);
        }
        catch (const BreakException &e) { break; }
        catch (const ContinueException &e) { continue; }
    }

    return NIL_VAL;
}

const Value Interpreter::EvalFor(Object &node) {
    ASSERT_TYPE(AST_TAG_FOR);

    for (EVAL(AST_TAG_FOR_PRE_ELIST); EVAL(AST_TAG_CONDITION); EVAL(AST_TAG_FOR_POST_ELIST)) {
        try {
            EVAL(AST_TAG_STMT);
        }
        catch (const BreakException &e) { break; }
        catch (const ContinueException &e) { continue; }
    }

    return NIL_VAL;
}

const Value Interpreter::EvalReturn(Object &node) {
    ASSERT_TYPE(AST_TAG_RETURN);
    return NIL_VAL;
}

const Value Interpreter::EvalBreak(Object &node) {
    ASSERT_TYPE(AST_TAG_BREAK);
    throw BreakException();
}

const Value Interpreter::EvalContinue(Object &node) {
    ASSERT_TYPE(AST_TAG_CONTINUE);
    throw ContinueException();
}

Interpreter::Interpreter(void) {
    globalScope = new Object();
    globalScope->IncreaseRefCounter();
    scopeStack.push_front(globalScope);

    currentScope = globalScope;
    currentScope->IncreaseRefCounter();

    InstallLibFuncs();
    InstallEvaluators();
}

void Interpreter::InstallLibFuncs(void) {
    globalScope->Set("print", Value(LibFunc::Print, "print"));
    globalScope->Set("typeof", Value(LibFunc::Typeof, "typeof"));
    globalScope->Set("object_keys", Value(LibFunc::ObjectKeys, "object_keys"));
    globalScope->Set("object_size", Value(LibFunc::ObjectSize, "object_size"));

    libraryFuncs.push_front("print");
    libraryFuncs.push_front("typeof");
    libraryFuncs.push_front("object_keys");
    libraryFuncs.push_front("object_size");
}

void Interpreter::Execute(Object &program) {
    assert(program[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_PROGRAM);
    assert(program.ElementExists(AST_TAG_CHILD));

    dispatcher.Eval(program);
}

Interpreter::~Interpreter() {
    /* Cleanup */
    dispatcher.Clear();
    libraryFuncs.clear();
    globalScope->Clear();
    delete globalScope;
}
