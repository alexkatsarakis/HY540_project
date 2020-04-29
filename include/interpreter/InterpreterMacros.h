#ifndef _INTERPRETER_MACROS_H_
#define _INTERPRETER_MACROS_H_

/****** Macros Shortcuts ******/

#define NIL_VAL Value(NilTypeValue::Nil)
#define EVAL_CHILD() dispatcher.Eval(*node[AST_TAG_CHILD]->ToObject_NoConst())
#define EVAL(type) dispatcher.Eval(*node[type]->ToObject_NoConst())
#define ASSERT_TYPE(type) assert(node[AST_TAG_TYPE_KEY]->ToString() == type)
#define INSTALL(tag, method) dispatcher.Install(tag, [this](Object &node) { return method(node); })
#define INSTALL_WRITE_FUNC(tag, method) dispatcher.InstallWriteFunc(tag, [this](Object &node) { return method(node); })
#define EVAL_WRITE(type) dispatcher.EvalWriteFunc(*node[type]->ToObject_NoConst())
#define GET_LINE(node) static_cast<unsigned>(node[LINE_NUMBER_RESERVED_FIELD]->ToNumber())

#define CHANGE_LINE() \
    if (node.ElementExists(LINE_NUMBER_RESERVED_FIELD)) {\
        SetLineNumber(static_cast<unsigned>(node[LINE_NUMBER_RESERVED_FIELD]->ToNumber()));\
    }

#define IS_CLOSURE_CHANGE() \
    (lvalue.IsProgramFunction() && index.IsString() && index.ToString() == CLOSURE_RESERVED_FIELD)

#define IS_LIB_FUNC()\
    (lvalue.IsIndexString() &&\
     IsLibFunc(lvalue.ToString()) &&\
     IsGlobalScope(lvalue.GetContext()))

#define IS_DOLLAR_ID()\
    (lvalue.IsIndexString() &&\
     lvalue.ToString()[0] == '$' &&\
     lvalue.ToString() != CLOSURE_RESERVED_FIELD)

#define OBJECT_REQUIRING_FIELD()\
    (lvalue.ToString() == OUTER_RESERVED_FIELD ||\
     lvalue.ToString() == CLOSURE_RESERVED_FIELD ||\
     lvalue.ToString() == PREVIOUS_RESERVED_FIELD ||\
     lvalue.ToString() == LOCAL_RESERVED_FIELD ||\
     lvalue.ToString() == PARENT_RESERVED_FIELD)

#define IS_OBJECT_REQUIRING_FIELD()\
    (lvalue.IsIndexString() &&\
     lvalue.ToString()[0] == '$' &&\
     OBJECT_REQUIRING_FIELD() &&\
     !rvalue.IsObject())\

#define IS_READ_ONLY_SYMBOL()\
    (lvalue.IsIndexString() &&\
     (lvalue.ToString() == LAMBDA_RESERVER_FIELD || lvalue.ToString() == ENV_RESERVED_FIELD))

#endif