#include "ValidityVisitor.h"

#include "Object.h"
#include "TreeTags.h"
#include "Utilities.h"

#include <cassert>
#include <iostream>

#define PARENT_FIELD "$$Parent"

#define IMPL_VISIT(type) \
    void ValidityVisitor::Visit##type(const Object &node) {}
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
IMPL_VISIT(ArgumentList)
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
IMPL_VISIT(Formal)
IMPL_VISIT(If)
IMPL_VISIT(While)
IMPL_VISIT(For)
#undef IMPL_VISIT

ValidityVisitor::ValidityVisitor() {
}

TreeVisitor *ValidityVisitor::Clone(void) const {
    ValidityVisitor *clone = new ValidityVisitor();
    return clone;
}

void ValidityVisitor::VisitReturn(const Object &node) {
    auto current = *node[PARENT_FIELD]->ToObject();
    while (current[AST_TAG_TYPE_KEY]->ToString() != AST_TAG_PROGRAM) {
        auto type = current[AST_TAG_TYPE_KEY]->ToString();
        if (type == AST_TAG_FUNCTION_DEF) return;
        current = *current[PARENT_FIELD]->ToObject();
        assert(current.IsValid());
    }
    std::cerr << "DONT USE FCKING RETURN LIKE THAT MORON" << std::endl;
    exit(0);
}

void ValidityVisitor::VisitBreak(const Object &node) {
    auto current = *node[PARENT_FIELD]->ToObject();
    while (current[AST_TAG_TYPE_KEY]->ToString() != AST_TAG_PROGRAM && current[AST_TAG_TYPE_KEY]->ToString() != AST_TAG_FUNCTION_DEF) {
        auto type = current[AST_TAG_TYPE_KEY]->ToString();
        if (type == AST_TAG_FOR || type == AST_TAG_WHILE) return;
        current = *current[PARENT_FIELD]->ToObject();
        assert(current.IsValid());
    }
    std::cerr << "DONT USE FCKING BREAK LIKE THAT MORON" << std::endl;
    exit(0);
}

void ValidityVisitor::VisitContinue(const Object &node) {
    auto current = *node[PARENT_FIELD]->ToObject();
    while (current[AST_TAG_TYPE_KEY]->ToString() != AST_TAG_PROGRAM && current[AST_TAG_TYPE_KEY]->ToString() != AST_TAG_FUNCTION_DEF) {
        auto type = current[AST_TAG_TYPE_KEY]->ToString();
        if (type == AST_TAG_FOR || type == AST_TAG_WHILE) return;
        current = *current[PARENT_FIELD]->ToObject();
        assert(current.IsValid());
    }
    std::cerr << "DONT USE FCKING CONTINUE LIKE THAT MORON" << std::endl;
    exit(0);
}
