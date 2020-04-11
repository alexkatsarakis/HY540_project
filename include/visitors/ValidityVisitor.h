#ifndef _VALIDITYVISITOR_H_
#define _VALIDITYVISITOR_H_

#include "TreeVisitor.h"

#include <string>

class ValidityVisitor : public TreeVisitor {

public:
    ValidityVisitor();
    virtual ~ValidityVisitor(){};
    TreeVisitor *Clone(void) const override;

    #define IMPL_VISIT(type) void Visit##type(const Object &node) override;
        IMPL_VISIT(Program)
        IMPL_VISIT(Statements)
        IMPL_VISIT(Statement)
        IMPL_VISIT(Expression)
        IMPL_VISIT(Assign)
        IMPL_VISIT(Plus)
        IMPL_VISIT(Minus)
        IMPL_VISIT(Mul)
        IMPL_VISIT(Div)
        IMPL_VISIT(Modulo)
        IMPL_VISIT(Greater)
        IMPL_VISIT(Less)
        IMPL_VISIT(GreaterEqual)
        IMPL_VISIT(LessEqual)
        IMPL_VISIT(Equal)
        IMPL_VISIT(NotEqual)
        IMPL_VISIT(And)
        IMPL_VISIT(Or)
        IMPL_VISIT(Term)
        IMPL_VISIT(UnaryMinus)
        IMPL_VISIT(Not)
        IMPL_VISIT(PlusPlusBefore)
        IMPL_VISIT(PlusPlusAfter)
        IMPL_VISIT(MinusMinusBefore)
        IMPL_VISIT(MinusMinusAfter)
        IMPL_VISIT(Primary)
        IMPL_VISIT(LValue)
        IMPL_VISIT(Id)
        IMPL_VISIT(Local)
        IMPL_VISIT(DoubleColon)
        IMPL_VISIT(Dollar)
        IMPL_VISIT(Member)
        IMPL_VISIT(Dot)
        IMPL_VISIT(Bracket)
        IMPL_VISIT(Call)
        IMPL_VISIT(CallSuffix)
        IMPL_VISIT(NormalCall)
        IMPL_VISIT(MethodCall)
        IMPL_VISIT(ExpressionList)
        IMPL_VISIT(ObjectDef)
        IMPL_VISIT(Indexed)
        IMPL_VISIT(IndexedElem)
        IMPL_VISIT(Block)
        IMPL_VISIT(FunctionDef)
        IMPL_VISIT(Const)
        IMPL_VISIT(Number)
        IMPL_VISIT(String)
        IMPL_VISIT(Nil)
        IMPL_VISIT(True)
        IMPL_VISIT(False)
        IMPL_VISIT(IdList)
        IMPL_VISIT(If)
        IMPL_VISIT(While)
        IMPL_VISIT(For)
        IMPL_VISIT(Return)
        IMPL_VISIT(Break)
        IMPL_VISIT(Continue)
    #undef IMPL_VISIT
};

#endif
