#include "Interpreter.h"
#include "LibraryFunctions.h"
#include "TreeTags.h"
#include "Utilities.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>

/****** Start-up ******/

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
    INSTALL(AST_TAG_ARGLIST, EvalArgumentList);
    INSTALL(AST_TAG_NAMED, EvalNamedArgument);
    INSTALL(AST_TAG_CALL, EvalCall);
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

#define INSTALL_LIB_FUNC(x, y)                 \
    globalScope->Set(x, Value(LibFunc::y, x)); \
    libraryFuncs.push_front(x);

void Interpreter::InstallLibFuncs(void) {
    INSTALL_LIB_FUNC("print", Print);
    INSTALL_LIB_FUNC("typeof", Typeof);
    INSTALL_LIB_FUNC("object_keys", ObjectKeys);
    INSTALL_LIB_FUNC("object_size", ObjectSize);
    INSTALL_LIB_FUNC("sleep", Sleep);
    INSTALL_LIB_FUNC("assert", Assert);
    INSTALL_LIB_FUNC("sqrt", Sqrt);
    INSTALL_LIB_FUNC("pow", Pow);
    INSTALL_LIB_FUNC("sin", Sin);
    INSTALL_LIB_FUNC("cos", Cos);
    INSTALL_LIB_FUNC("get_time", GetTime);
    INSTALL_LIB_FUNC("input", Input);
    INSTALL_LIB_FUNC("to_number", ToNumber);
    INSTALL_LIB_FUNC("rand", Random);
}

/****** Public Members ******/

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

void Interpreter::Execute(Object &program) {
    assert(program[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_PROGRAM);
    assert(program.ElementExists(AST_TAG_CHILD));

    dispatcher.Eval(program);
}

void Interpreter::RuntimeError(const std::string &msg, unsigned line) {
    std::string lineMsg = (line != 0) ? (" at line " + std::to_string(line)) :
                          (lineNumber != 0) ? (" at line " + std::to_string(lineNumber)) : "";

    std::cerr << "\033[31;1m"    //RED
              << "Runtime Error" << lineMsg << ": "
              << "\033[0m" << msg << std::endl;
    exit(EXIT_FAILURE);
}

void Interpreter::RuntimeWarning(const std::string &msg, unsigned line) {
    std::string lineMsg = (line != 0) ? (" at line " + std::to_string(line)) :
                          (lineNumber != 0) ? (" at line " + std::to_string(lineNumber)) : "";

    std::cerr << "\033[31;1m"    //RED
              << "Runtime Warning" << lineMsg << ": "
              << "\033[0m" << msg << std::endl;
}

void Interpreter::Assert(const std::string &msg, unsigned line) {
    std::string lineMsg = (line != 0) ? (" at line " + std::to_string(line)) :
                          (lineNumber != 0) ? (" at line " + std::to_string(lineNumber)) : "";

    std::cerr << "\033[31;1m"    //RED
              << "Assertion Failed" << lineMsg << ": "
              << "\033[0m" << msg << std::endl;
    exit(EXIT_FAILURE);
}

Interpreter::~Interpreter() {
    /* Cleanup */
    dispatcher.Clear();
    libraryFuncs.clear();
    globalScope->Clear();
    delete globalScope;
    scopeStack.clear();
}

/****** Lvalue Write Access ******/

Symbol Interpreter::EvalLvalueWrite(Object &node) {
    ASSERT_TYPE(AST_TAG_LVALUE);
    return EVAL_WRITE(AST_TAG_CHILD);
}

Symbol Interpreter::EvalMemberWrite(Object &node) {
    ASSERT_TYPE(AST_TAG_MEMBER);
    return EVAL_WRITE(AST_TAG_CHILD);
}

Symbol Interpreter::EvalDotWrite(Object &node) {
    ASSERT_TYPE(AST_TAG_DOT);

    auto lvalue = EVAL(AST_TAG_LVALUE);
    auto index = GetIdName(*node[AST_TAG_ID]->ToObject());

    CHANGE_LINE();
    return TableSetElem(lvalue, index);
}

Symbol Interpreter::EvalBracketWrite(Object &node) {
    ASSERT_TYPE(AST_TAG_BRACKET);
    const Value lvalue = EVAL(AST_TAG_LVALUE);
    const Value index = EVAL(AST_TAG_EXPR);

    CHANGE_LINE();
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
    CHANGE_LINE();

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
    CHANGE_LINE();

    std::string formalName = node[AST_TAG_ID]->ToString();

    if (IsLibFunc(formalName)) RuntimeError("Formal argument \"" + formalName + "\" shadows library function");

    if (LookupCurrentScope(formalName)) RuntimeError("Formal argument \"" + formalName + "\" already defined as a formal");

    return Symbol(currentScope, formalName);
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

/****** Evaluation Helpers ******/

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

const Value Interpreter::GetIdName(const Object &node) {
    assert(node.ElementExists(AST_TAG_ID));
    const Value * name = node[AST_TAG_ID];
    assert(name->IsString());
    return *name;
}

const Value Interpreter::HandleAggregators(Object &node, MathOp op, bool returnChanged) {
    assert(node.IsValid());

    Symbol lvalue = EVAL_WRITE(AST_TAG_CHILD);
    assert(lvalue.IsValid());

    CHANGE_LINE();

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

const Value Interpreter::EvalMath(Object &node, MathOp op) {
    assert(node.IsValid());

    auto op1 = EVAL(AST_TAG_FIRST_EXPR);
    auto op2 = EVAL(AST_TAG_SECOND_EXPR);

    CHANGE_LINE();

    /* Special case for string concatenation */
    if (op1.IsString() && op2.IsString() && op == MathOp::Plus)
        return (op1.ToString() + op2.ToString());

    if (!op1.IsNumber()) RuntimeError("First operand is not a number in an arithmetic operation");
    if (!op2.IsNumber()) RuntimeError("Second operand is not a number in an arithmetic operation");

    if ((op == MathOp::Div || op == MathOp::Mod) &&
        Utilities::IsZero(op2.ToNumber())) RuntimeError("Cannot divide by zero");

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
    if (v1.IsUndef() || v2.IsUndef()) RuntimeError("Undefined value found in equality operator");
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
    else if (v1.IsObject())
        return v1.ToObject() == v2.ToObject();
    else
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

void Interpreter::CleanupForLoop(Value & elist1, Value & elist2) {
    assert(elist1.IsObject());

    elist1.ToObject_NoConst()->Clear();
    delete elist1.ToObject_NoConst();

    /* Elist2 can be undef if it has not been evaluated yet:
       for(i = 0; i < 10; ++i) if (i == 0) return;
     */
    if (elist2.IsObject()) {
        elist2.ToObject_NoConst()->Clear();
        delete elist2.ToObject_NoConst();
    }
}

/****** Evaluation Side Actions ******/

void Interpreter::BlockEnter(void) {
    currentScope = PushNested();
}

void Interpreter::BlockExit(void) {
    assert(currentScope);

    Object *tmp = nullptr;

    bool shouldSlice = currentScope->ElementExists(PREVIOUS_RESERVED_FIELD);
    while (currentScope->ElementExists(PREVIOUS_RESERVED_FIELD)) {
        tmp = currentScope;
        currentScope = (*currentScope)[PREVIOUS_RESERVED_FIELD]->ToObject_NoConst();
        assert(currentScope);
        tmp->DecreaseRefCounter();
    }

    assert(currentScope->ElementExists(OUTER_RESERVED_FIELD));
    PopScope();

    if (shouldSlice) currentScope = PushSlice();
}

/****** Function Call Evaluation Helpers ******/

Value Interpreter::CallProgramFunction(Object &functionAst, Object &functionClosure, const Object &actuals, const std::vector<std::string> &actualNames) {
    //function entry
    currentScope = PushScopeSpace(&functionClosure);
    inFunctionScope = true;

    const Object &formals = *(functionAst[AST_TAG_FUNCTION_FORMALS]->ToObject());
    std::vector<std::string> formalNames = GetFormalNames(formals);

    ProgramFunctionRuntimeChecks(formals, formalNames, actuals, actualNames);
    //actual-formal matching
    for (register unsigned i = 0; i < formals.GetNumericSize(); ++i) {
        Object &formal = *(formals[i]->ToObject_NoConst());
        std::string formalName = formalNames.at(i);    //Name of formal, i is index of formal/actual
        Value actualValue;                             //Value of actual
        bool matchExists = false;                      //actual-formal match exists

        if (i < actuals[POSITIONAL_SIZE_RESERVED_FIELD]->ToNumber()) {
            matchExists = true;
            actualValue = *(actuals[i]);
        } else if (actuals.ElementExists(formalName)) {
            matchExists = true;
            unsigned index = actuals[formalName]->ToNumber();
            actualValue = *(actuals[index]);
        }

        if (formal[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_FORMAL) {
            assert(matchExists);
            dispatcher.Eval(formal);
        } else if (formal[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_ASSIGN) {
            if (!matchExists)
                actualValue = dispatcher.Eval(formal);    //Evaluate optional expression
        } else {
            assert(false);
        }
        currentScope->Set(formalName, actualValue);
    }

    //function body evaluation
    try {
        dispatcher.Eval(*(functionAst[AST_TAG_STMT]->ToObject_NoConst()));
    } catch (const ReturnException &e) {
        ;
    }

    //function exit
    //PopScope(); Done in BlockExit
    PopScopeSpace();
    currentScope = scopeStack.front();

    return retvalRegister;
}

Value Interpreter::CallLibraryFunction(const std::string &functionId, LibraryFunc functionLib, Object &actuals) {
    assert(functionLib);
    assert(!functionId.empty());

    actuals.Set(RETVAL_RESERVED_FIELD, Value(NilTypeValue::Nil));

    functionLib(actuals);                                                                      //Library function Call
    Value result = *(actuals[RETVAL_RESERVED_FIELD]);                                          //do we need to modify retval?

    if (retvalRegister.IsObject()) retvalRegister.ToObject_NoConst()->DecreaseRefCounter();    //TODO need to know why
    retvalRegister = result;
    if (retvalRegister.IsObject()) retvalRegister.ToObject_NoConst()->IncreaseRefCounter();

    return retvalRegister;
}

void Interpreter::ProgramFunctionRuntimeChecks(const Object &formals, const std::vector<std::string> &formalNames, const Object &actuals, const std::vector<std::string> &actualNames) {
    //Number of args error
    unsigned formalSize = formals.GetNumericSize();
    unsigned actualPositionalSize = actuals[POSITIONAL_SIZE_RESERVED_FIELD]->ToNumber();
    if (actualPositionalSize > formalSize)
        RuntimeError("Number of Actual Positional Arguments(" + std::to_string(actualPositionalSize) + ") exceeds number of Formal Parameters(" + std::to_string(formalSize) + ")");

    //Unexpected named error (name not defined in formals)
    std::vector<std::string> actualMinusFormals;
    std::set<std::string> v1(actualNames.begin(), actualNames.end());
    std::set<std::string> v2(formalNames.begin(), formalNames.end());
    std::set_difference(v1.begin(), v1.end(),
                        v2.begin(), v2.end(),
                        std::inserter(actualMinusFormals, actualMinusFormals.begin()));
    if (!actualMinusFormals.empty())
        RuntimeError("Unexpected Named");    //TODO Add Info

    //checks on actual-formal matching
    for (register unsigned i = 0; i < formals.GetNumericSize(); ++i) {
        Object &formal = *(formals[i]->ToObject_NoConst());
        std::string formalName = formalNames.at(i);    //Name of formal, i is index of formal/actual
        bool matchExists = false;                      //actual-formal match exists
        assert(formalName != POSITIONAL_SIZE_RESERVED_FIELD);

        if (i < actuals[POSITIONAL_SIZE_RESERVED_FIELD]->ToNumber()) {
            //Both positional and named actual match error
            if (std::find(actualNames.begin(), actualNames.end(), formalName) != actualNames.end())
                RuntimeError("Both positional and named match");    //TODO Add info
            matchExists = true;
        } else if (actuals.ElementExists(formalName)) {
            assert(std::find(actualNames.begin(), actualNames.end(), formalName) != actualNames.end());
            matchExists = true;
        }

        if (formal[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_FORMAL) {
            //A non-optional formal is not matched
            if (!matchExists)
                RuntimeError("Formal does not match actual");    //TODO Add info
        } else if (formal[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_ASSIGN) {
            ;
        } else {
            assert(false);
        }
    }
}

std::vector<std::string> Interpreter::GetFormalNames(const Object &formals) {
    std::vector<std::string> formalNames;
    for (register unsigned i = 0; i < formals.GetNumericSize(); ++i) {
        const Object &formal = *(formals[i]->ToObject());
        assert((formal[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_FORMAL) || (formal[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_ASSIGN));
        std::string name;
        if (formal[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_FORMAL) {
            assert(formal.ElementExists(AST_TAG_ID));
            name = formal[AST_TAG_ID]->ToString();
        } else if (formal[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_ASSIGN) {
            assert(formal.ElementExists(AST_TAG_LVALUE));
            const Object &formalNode = *(formal[AST_TAG_LVALUE]->ToObject());
            assert(formalNode[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_FORMAL);
            assert(formalNode.ElementExists(AST_TAG_ID));
            name = formalNode[AST_TAG_ID]->ToString();
        } else {
            assert(false);
        }
        formalNames.push_back(name);
    }
    return formalNames;
}

/****** Symbol Lookup ******/

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

    Object *scope = GetGlobalScope();
    return LookupScope(scope, symbol);
}

Object *Interpreter::FindScope(const std::string &symbol) const {
    assert(!symbol.empty());
    assert(currentScope);

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

/****** Environment Actions ******/

Object *Interpreter::PushScopeSpace(Object *outerScope) {
    Object *newScopeSpace = new Object();
    newScopeSpace->Set(OUTER_RESERVED_FIELD, outerScope);
    newScopeSpace->IncreaseRefCounter();
    scopeStack.push_front(newScopeSpace);
    return newScopeSpace;
}

void Interpreter::PopScopeSpace(void) {
    assert(!scopeStack.empty());
    scopeStack.pop_front();
}

Object *Interpreter::PushSlice(void) {
    return PushScope(PREVIOUS_RESERVED_FIELD);
}

Object *Interpreter::PushNested(void) {
    return PushScope(OUTER_RESERVED_FIELD);
}

Object *Interpreter::PopScope(void) {
    assert(currentScope && currentScope->IsValid());
    assert(currentScope->ElementExists(OUTER_RESERVED_FIELD));

    Object *scope = currentScope;
    currentScope = (*scope)[OUTER_RESERVED_FIELD]->ToObject_NoConst();
    scope->DecreaseRefCounter();
    return scope;
}

Object *Interpreter::PushScope(const std::string &tag) {
    assert(!tag.empty());
    assert(tag == PREVIOUS_RESERVED_FIELD ||
           tag == OUTER_RESERVED_FIELD);
    assert(currentScope);

    Object *scope = new Object();
    scope->Set(tag, Value(currentScope));

    currentScope->IncreaseRefCounter();
    scope->IncreaseRefCounter();

    assert(!scopeStack.empty());
    scopeStack.pop_front();
    scopeStack.push_front(scope);

    return scope;
}

/****** Helpers ******/

bool Interpreter::IsLibFunc(const std::string &symbol) const {
    assert(!symbol.empty());
    return std::find(libraryFuncs.begin(), libraryFuncs.end(), symbol) != libraryFuncs.end();
}

bool Interpreter::IsReservedField(const std::string &index) const {
    return (index == OUTER_RESERVED_FIELD ||
            index == PREVIOUS_RESERVED_FIELD ||
            index == RETVAL_RESERVED_FIELD ||
            index == CLOSURE_RESERVED_FIELD);
}

const Value Interpreter::GetStringFromContext(Object *table, const Value &index, bool lookupFail) {
    assert(table && table->IsValid());
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
    assert(table && table->IsValid());
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

Symbol Interpreter::ClosureSetElem(const Value &lvalue, const Value &index) {
    assert(lvalue.IsObject());

    Object *table = lvalue.ToObject_NoConst();
    assert(table);

    if (index.IsString() && index.ToString() == CLOSURE_RESERVED_FIELD)
        RuntimeError("Cannot change the closure object of a function");
    else if (index.IsNumber() && !table->ElementExists(index.ToNumber()))
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
    assert(table);

    if (index.IsNumber())
        return Symbol(table, index.ToNumber(), true);
    else if (index.IsString())
        return Symbol(table, index.ToString(), true);
    else
        assert(false);
}

unsigned Interpreter::GetLineNumber(void) const {
    assert(lineNumber);
    return lineNumber;
}

void Interpreter::SetLineNumber(unsigned num) {
    assert(num);
    lineNumber = num;
}