#include "Interpreter.h"
#include "LibraryFunctions.h"
#include "TreeTags.h"

#include <cassert>
#include <iostream>

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

const Value Interpreter::EvalCall(Object &node) {
    ASSERT_TYPE(AST_TAG_CALL);

    auto function = EVAL(AST_TAG_FUNCTION);
    auto arguments = EVAL(AST_TAG_SUFFIX);

    if (!function.IsLibraryFunction() && !function.IsProgramFunction()) RuntimeError("Cannot call " + function.GetTypeToString());
    assert(arguments.IsObject());

    if (function.IsLibraryFunction()) {
        function.ToLibraryFunction()(*arguments.ToObject_NoConst());

        if (retvalRegister.IsObject()) retvalRegister.ToObject_NoConst()->DecreaseRefCounter();

        const Value * retval = (*arguments.ToObject())[RETVAL_RESERVED_FIELD];

        /* TODISCUSS: Is this a good practice? */
        if(!retval) retvalRegister.FromUndef();
        else new (&retvalRegister) Value(*retval);

    } else {
        assert(false);
    }

    arguments.ToObject_NoConst()->Clear();
    delete arguments.ToObject_NoConst();
    arguments.FromUndef();

    return retvalRegister;
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