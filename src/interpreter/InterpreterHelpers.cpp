#include "Interpreter.h"
#include "LibraryFunctions.h"
#include "TreeTags.h"
#include "Utilities.h"

#include <algorithm>
#include <cassert>
#include <iostream>

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
    INSTALL(AST_TAG_METHOD_CALL, EvalMethodCall);
    INSTALL(AST_TAG_BLOCK, EvalBlock);
    INSTALL(AST_TAG_IF, EvalIf);
    INSTALL(AST_TAG_WHILE, EvalWhile);
    INSTALL(AST_TAG_FOR, EvalFor);
    INSTALL(AST_TAG_BREAK, EvalBreak);
    INSTALL(AST_TAG_CONTINUE, EvalContinue);
    INSTALL(AST_TAG_RETURN, EvalReturn);
    INSTALL(AST_TAG_INDEXED_ELEM, EvalIndexedElem);
    INSTALL(AST_TAG_INDEXED, EvalIndexed);
    INSTALL(AST_TAG_OBJECT_DEF, EvalObjectDef);
    INSTALL(AST_TAG_MEMBER, EvalMember);
    INSTALL(AST_TAG_DOT, EvalDot);
    INSTALL(AST_TAG_BRACKET, EvalBracket);
    INSTALL(AST_TAG_FORMAL, EvalFormal);
    /* Evaluators used for write access */
    INSTALL_WRITE_FUNC(AST_TAG_LVALUE, EvalLvalueWrite);
    INSTALL_WRITE_FUNC(AST_TAG_MEMBER, EvalMemberWrite);
    INSTALL_WRITE_FUNC(AST_TAG_DOT, EvalDotWrite);
    INSTALL_WRITE_FUNC(AST_TAG_BRACKET, EvalBracketWrite);
    INSTALL_WRITE_FUNC(AST_TAG_ID, EvalIdWrite);
    INSTALL_WRITE_FUNC(AST_TAG_DOUBLECOLON_ID, EvalGlobalIdWrite);
    INSTALL_WRITE_FUNC(AST_TAG_LOCAL_ID, EvalLocalIdWrite);
    INSTALL_WRITE_FUNC(AST_TAG_FORMAL, EvalFormalWrite);
}

void Interpreter::RuntimeError(const std::string &msg) {
    std::cerr << "\033[31;1m"
              << "Runtime Error: "
              << "\033[0m" << msg << std::endl;
    exit(EXIT_FAILURE);
}

void Interpreter::Assert(const std::string &msg) {
    std::cerr << "\033[31;1m"     //34;1m"
              << "Assertion: "
              << "\033[0m" << msg << std::endl;
    exit(EXIT_FAILURE);
}

bool Interpreter::IsReservedField(const std::string &index) const {
    return (index == OUTER_RESERVED_FIELD ||
            index == PREVIOUS_RESERVED_FIELD ||
            index == RETVAL_RESERVED_FIELD ||
            index == CLOSURE_RESERVED_FIELD);
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

bool Interpreter::IsGlobalScope(Object *scope) const {
    assert(scope);

    /* Move backwards towards the first slice of the scope */
    while (scope->ElementExists(PREVIOUS_RESERVED_FIELD)) {
        scope = (*scope)[PREVIOUS_RESERVED_FIELD]->ToObject_NoConst();
    }

    /* The first slice of the global scope should contain library functions */
    return (!scope->ElementExists(OUTER_RESERVED_FIELD) &&
            scope->ElementExists("print") &&
            (*scope)["print"]->IsLibraryFunction());
}

Object *Interpreter::GetGlobalScope(void) const {
    assert(currentScope);

    Object *scope = currentScope;
    const Value *value = LookupScope(scope, OUTER_RESERVED_FIELD);
    while (value) {
        scope = value->ToObject_NoConst();
        value = LookupScope(scope, OUTER_RESERVED_FIELD);
    }

    return scope;
}

const Value *Interpreter::LookupGlobalScope(const std::string &symbol) const {
    assert(!symbol.empty());

    Object *scope = GetGlobalScope();
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

void Interpreter::PushScopeSpace(Object *scope) {
    scope->IncreaseRefCounter();
    scopeStack.push_front(scope);
    scope->IncreaseRefCounter();
}
void Interpreter::PopScopeSpace() {
    Object *scope = scopeStack.front();
    scopeStack.pop_front();
    scope->DecreaseRefCounter();
}
Object *Interpreter::PushSlice() {
    Object *scope = new Object();
    scope->Set(PREVIOUS_RESERVED_FIELD, Value(currentScope));
    scope->IncreaseRefCounter();
    return scope;
}
Object *Interpreter::PushNested() {
    Object *scope = new Object();
    scope->Set(OUTER_RESERVED_FIELD, Value(currentScope));
    scope->IncreaseRefCounter();
    return scope;
}

Object *Interpreter::PopScope() {
    Object *scope = currentScope;
    currentScope = (*scope)[OUTER_RESERVED_FIELD]->ToObject_NoConst();
    scope->DecreaseRefCounter();
    return scope;
}

bool Interpreter::IsLibFunc(const std::string &symbol) const {
    assert(!symbol.empty());
    return std::find(libraryFuncs.begin(), libraryFuncs.end(), symbol) != libraryFuncs.end();
}

const Value Interpreter::EvalMath(Object &node, MathOp op) {
    assert(node.IsValid());

    auto op1 = EVAL(AST_TAG_FIRST_EXPR);
    auto op2 = EVAL(AST_TAG_SECOND_EXPR);

    /* Special case for string concatenation */
    if (op1.IsString() && op2.IsString() && op == MathOp::Plus)
        return (op1.ToString() + op2.ToString());

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
    else if (v1.IsObject()) {
        return v1.ToObject() == v2.ToObject();
    } else
        assert(false);
}

void Interpreter::AssignToContext(const Symbol &lvalue, const Value &rvalue) {
    assert(lvalue.IsValid());
    assert(rvalue.IsValid());

    /* TODO: Please refactor this mess */
    if (lvalue.IsIndexString()) {
        Object *context = lvalue.GetContext();
        const Value *v = (*context)[lvalue.ToString()];
        if (v && v->IsObject()) v->ToObject_NoConst()->DecreaseRefCounter();
        context->Set(lvalue.ToString(), rvalue);
    } else if (lvalue.IsIndexNumber()) {
        Object *context = lvalue.GetContext();
        const Value *v = (*context)[lvalue.ToNumber()];
        if (v && v->IsObject()) v->ToObject_NoConst()->DecreaseRefCounter();
        lvalue.GetContext()->Set(lvalue.ToNumber(), rvalue);
    } else
        assert(false);

    if (rvalue.IsObject()) rvalue.ToObject_NoConst()->IncreaseRefCounter();
}

void Interpreter::RemoveFromContext(const Symbol &lvalue, const Value &rvalue) {
    assert(lvalue.IsValid());
    assert(rvalue.IsValid());

    const Value *old = nullptr;
    if (lvalue.IsIndexString())
        old = lvalue.GetContext()->GetAndRemove(lvalue.ToString());
    else if (lvalue.IsIndexNumber())
        old = lvalue.GetContext()->GetAndRemove(lvalue.ToNumber());
    else
        assert(false);

    if (old && old->IsObject()) old->ToObject_NoConst()->DecreaseRefCounter();
}

const Value Interpreter::HandleAggregators(Object &node, MathOp op, bool returnChanged) {
    assert(node.IsValid());

    Symbol lvalue = EVAL_WRITE(AST_TAG_CHILD);
    assert(lvalue.IsValid());

    const Value *value = nullptr;
    if (lvalue.IsIndexString())
        value = (*lvalue.GetContext())[lvalue.ToString()];
    else if (lvalue.IsIndexNumber())
        value = (*lvalue.GetContext())[lvalue.ToNumber()];
    else
        assert(false);

    if (!value->IsNumber()) RuntimeError("Increment/decrement operators can only be applied to numbers not to " + value->GetTypeToString());

    double number = value->ToNumber();
    double result = number;

    if (op == MathOp::Plus)
        result = number + 1;
    else if (op == MathOp::Minus)
        result = number - 1;
    else
        assert(false);

    if (lvalue.IsIndexString())
        lvalue.GetContext()->Set(lvalue.ToString(), result);
    else if (lvalue.IsIndexNumber())
        lvalue.GetContext()->Set(lvalue.ToNumber(), result);
    else
        assert(false);

    if (returnChanged)
        return result;
    else
        return number;
}

const Value Interpreter::GetIdName(const Object &node) {
    assert(node.ElementExists(AST_TAG_ID));
    auto name = node[AST_TAG_ID];
    assert(name->IsString());
    return *name;
}

const Value Interpreter::GetStringFromContext(Object *table, const Value &index, bool lookupFail) {
    assert(table);
    assert(index.IsString());

    std::string str = index.ToString();

    bool elementExists = table->ElementExists(str);

    if (!elementExists && lookupFail)
        RuntimeError("Field \"" + str + "\" does not exist");
    else if (!elementExists)
        return Value();
    else
        return *(*table)[str];

    assert(false); /* Keep this to suppress -Wreturn-type warning */
}

const Value Interpreter::GetNumberFromContext(Object *table, const Value &index, bool lookupFail) {
    assert(table);
    assert(index.IsNumber());

    double num = index.ToNumber();

    bool elementExists = table->ElementExists(num);

    if (!elementExists && lookupFail)
        RuntimeError("Field " + std::to_string(num) + " does not exist");
    else if (!elementExists)
        return Value();
    else
        return *(*table)[num];

    assert(false); /* Keep this to suppress -Wreturn-type warning */
}

const Value Interpreter::TableGetElem(const Value &lvalue, const Value &index) {
    if (!lvalue.IsObject() && !lvalue.IsProgramFunction()) RuntimeError("Cannot get field \"" + (index.IsString() ? index.ToString() : std::to_string(index.ToNumber())) + "\" of something that is not an object");
    if (!index.IsString() && !index.IsNumber()) RuntimeError("Keys of objects can only be strings or numbers");

    if (lvalue.IsProgramFunction() &&
        index.ToString() == CLOSURE_RESERVED_FIELD)
        return Value(lvalue.ToProgramFunctionClosure_NoConst(), true);

    Object *table = nullptr;
    if (lvalue.IsObject())
        table = lvalue.ToObject_NoConst();
    else
        table = lvalue.ToProgramFunctionClosure_NoConst();

    bool shouldFail = false;
    if (lvalue.IsProgramFunction())
        shouldFail = true;
    else if (lvalue.IsObject() && lvalue.IsObjectClosure())
        shouldFail = true;

    if (index.IsString())
        return GetStringFromContext(table, index, shouldFail);
    else
        return GetNumberFromContext(table, index, shouldFail);
}

Symbol Interpreter::ClosureSetElem(const Value &lvalue, const Value &index) {
    assert(lvalue.IsObject());

    Object *table = lvalue.ToObject_NoConst();

    if (index.IsNumber() && !table->ElementExists(index.ToNumber()))
        RuntimeError("Cannot set field. Field \"" + std::to_string(index.ToNumber()) + "\" does not exist in closure.");
    else if (index.IsString() && !table->ElementExists(index.ToString()))
        RuntimeError("Cannot set field. Field \"" + index.ToString() + "\" does not exist in closure.");

    if (index.IsNumber())
        return Symbol(table, index.ToNumber());
    else if (index.IsString())
        return Symbol(table, index.ToString());
    else
        assert(false);
}

Symbol Interpreter::ObjectSetElem(const Value &lvalue, const Value &index) {
    assert(lvalue.IsObject());

    Object *table = lvalue.ToObject_NoConst();

    if (index.IsNumber())
        return Symbol(table, index.ToNumber());
    else if (index.IsString())
        return Symbol(table, index.ToString());
    else
        assert(false);
}

Symbol Interpreter::TableSetElem(const Value &lvalue, const Value &index) {
    if (!lvalue.IsObject() && !lvalue.IsProgramFunction())
        RuntimeError("Cannot set field \"" + (index.IsString() ? index.ToString() : std::to_string(index.ToNumber())) + "\" of something that is not an object");

    if (!index.IsString() && !index.IsNumber())
        RuntimeError("Keys of objects can only be strings or numbers");

    if (index.IsString() &&
        index.ToString()[0] == '$' &&
        !IsReservedField(index.ToString()))
        RuntimeError("Cannot write to field \"" + index.ToString() + "\". No write access to user-defined $ indices");

    /* The 3 cases of a TABLESETELEM instruction are:
     *
     * func.x = 0;
     * func.$closure.x = 0;
     * table.x = ;
     */
    if (lvalue.IsProgramFunction())
        return ClosureSetElem(lvalue.ToProgramFunctionClosure_NoConst(), index);
    else if (lvalue.IsObjectClosure())
        return ClosureSetElem(lvalue, index);
    else
        return ObjectSetElem(lvalue, index);
}

Symbol Interpreter::EvalMemberWrite(Object &node) {
    ASSERT_TYPE(AST_TAG_MEMBER);
    return EVAL_WRITE(AST_TAG_CHILD);
}

Symbol Interpreter::EvalDotWrite(Object &node) {
    ASSERT_TYPE(AST_TAG_DOT);

    auto lvalue = EVAL(AST_TAG_LVALUE);
    auto index = GetIdName(*node[AST_TAG_ID]->ToObject());
    return TableSetElem(lvalue, index);
}

Symbol Interpreter::EvalBracketWrite(Object &node) {
    ASSERT_TYPE(AST_TAG_BRACKET);
    const Value lvalue = EVAL(AST_TAG_LVALUE);
    const Value index = EVAL(AST_TAG_EXPR);
    return TableSetElem(lvalue, index);
}

Symbol Interpreter::EvalIdWrite(Object &node) {
    ASSERT_TYPE(AST_TAG_ID);

    std::string name = node[AST_TAG_ID]->ToString();
    Object *scope = FindScope(name);
    if (!scope) scope = currentScope;

    return Symbol(scope, name);
}

Symbol Interpreter::EvalGlobalIdWrite(Object &node) {
    ASSERT_TYPE(AST_TAG_DOUBLECOLON_ID);

    std::string symbol = node[AST_TAG_ID]->ToString();
    if (!LookupGlobalScope(symbol)) RuntimeError("Global symbol \"" + symbol + "\" does not exist (Undefined Symbol)");

    return Symbol(GetGlobalScope(), symbol);
}

Symbol Interpreter::EvalLocalIdWrite(Object &node) {
    ASSERT_TYPE(AST_TAG_LOCAL_ID);
    std::string symbol = node[AST_TAG_ID]->ToString();
    return Symbol(currentScope, symbol);
}

Symbol Interpreter::EvalFormalWrite(Object &node) {
    ASSERT_TYPE(AST_TAG_FORMAL);

    std::string formalName = node[AST_TAG_ID]->ToString();

    if (IsLibFunc(formalName)) RuntimeError("Formal argument \"" + formalName + "\" shadows library function");

    if (LookupCurrentScope(formalName)) RuntimeError("Formal argument \"" + formalName + "\" already defined as a formal");

    return Symbol(currentScope, formalName);
}

Symbol Interpreter::EvalLvalueWrite(Object &node) {
    ASSERT_TYPE(AST_TAG_LVALUE);
    return EVAL_WRITE(AST_TAG_CHILD);
}

void Interpreter::BlockEnter(void) {
    /*  Object *scope = new Object();
    scope->Set(OUTER_RESERVED_FIELD, currentScope);
    scope->IncreaseRefCounter(); */
    Object *scope = PushNested();
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
    /* tmp = currentScope;
    currentScope = (*currentScope)[OUTER_RESERVED_FIELD]->ToObject_NoConst();
    tmp->DecreaseRefCounter(); */
    PopScope();

    if (shouldSlice) {
        scope = new Object();
        scope->Set(PREVIOUS_RESERVED_FIELD, currentScope);
        scope->IncreaseRefCounter();
        currentScope = scope;
    }
}

Value Interpreter::CallProgramFunction(Object *functionAst, Object *functionClosure, Object *arguments) {
    //Push scope space pointing to function closure
    PushScopeSpace(functionClosure);
    //Push function environment
    //Current scope is now the function environment
    currentScope = PushNested();
    inFunctionScope = true;

    const Object &functionFormals = *((*functionAst)[AST_TAG_FUNCTION_FORMALS]->ToObject());
    const Object &functionActuals = *arguments;
    for (register unsigned i = 0; i < functionFormals.GetNumericSize(); ++i) {
        Object &child = *(functionFormals[i]->ToObject_NoConst());
        assert((child[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_FORMAL) || (child[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_ASSIGN));
        std::string formalName;       //Name of formal, i is index of formal/actual
        Value actualValue;            //Value of actual
        bool actualExists = false;    //actual/formal match exists
        if (i < functionActuals.GetNumericSize()) {
            actualExists = true;
            actualValue = *(functionActuals[i]);
        }

        if (child[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_FORMAL) {
            assert(child.ElementExists(AST_TAG_ID));
            formalName = child[AST_TAG_ID]->ToString();

            assert(actualExists);
            dispatcher.Eval(child);
        }
        if (child[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_ASSIGN) {
            assert(child.ElementExists(AST_TAG_LVALUE));
            const Object &formalNode = *(child[AST_TAG_LVALUE]->ToObject_NoConst());
            assert(formalNode[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_FORMAL);
            assert(formalNode.ElementExists(AST_TAG_ID));
            formalName = formalNode[AST_TAG_ID]->ToString();

            if (!actualExists) actualValue = dispatcher.Eval(child);    //Get Value of optional expression
        }
        currentScope->Set(formalName, actualValue);
    }

    //function body evaluation
    Value retVal;    // initialized to undef, in case function never returns a value
    try {
        dispatcher.Eval(*((*functionAst)[AST_TAG_STMT]->ToObject_NoConst()));
    } catch (const ReturnException &e) {
        //RETURN_VAL
        retVal = e.retVal;
    }

    //FUNC_EXIT
    //Pop function environment
    // PopScope(); //Done in BlockExit

    //Push scope space pointing to function closure
    PopScopeSpace();
    //Current scope is now the function closure
    currentScope = functionClosure;

    return retVal;
}

Value Interpreter::CallLibraryFunction(const std::string &functionId, LibraryFunc functionLib, Object *arguments) {
    arguments->Set(RETVAL_RESERVED_FIELD, Value(NilTypeValue::Nil));
    assert(functionLib);
    functionLib(*arguments);
    retvalRegister = *((*arguments)[RETVAL_RESERVED_FIELD]);    //do we need to modify retval?
    return retvalRegister;
    // if (retvalRegister.IsObject()) retvalRegister.ToObject_NoConst()->DecreaseRefCounter(); //need to know why
}

Interpreter::Interpreter(void) {
    globalScope = new Object();
    globalScope->IncreaseRefCounter();
    scopeStack.push_front(globalScope);

    currentScope = globalScope;
    currentScope->IncreaseRefCounter();
    inFunctionScope = false;

    InstallLibFuncs();
    InstallEvaluators();
}

void Interpreter::InstallLibFuncs(void) {
    globalScope->Set("print", Value(LibFunc::Print, "print"));
    globalScope->Set("typeof", Value(LibFunc::Typeof, "typeof"));
    globalScope->Set("object_keys", Value(LibFunc::ObjectKeys, "object_keys"));
    globalScope->Set("object_size", Value(LibFunc::ObjectSize, "object_size"));
    globalScope->Set("sleep", Value(LibFunc::Sleep, "sleep"));
    globalScope->Set("assert",Value(LibFunc::Assert, "assert"));
    globalScope->Set("sqrt",Value(LibFunc::Sqrt, "sqrt"));
    globalScope->Set("pow",Value(LibFunc::Pow, "pow"));
    globalScope->Set("get_time",Value(LibFunc::GetTime, "get_time"));
    globalScope->Set("input",Value(LibFunc::Input, "input"));

    libraryFuncs.push_front("print");
    libraryFuncs.push_front("typeof");
    libraryFuncs.push_front("object_keys");
    libraryFuncs.push_front("object_size");
    libraryFuncs.push_front("sleep");
    libraryFuncs.push_front("assert");
    libraryFuncs.push_front("sqrt");
    libraryFuncs.push_front("pow");
    libraryFuncs.push_front("get_time");
    libraryFuncs.push_front("input");
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
