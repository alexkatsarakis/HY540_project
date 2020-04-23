#ifndef _DEALLOCATE_VISITOR_H_
#define _DEALLOCATE_VISITOR_H_

#include "TreeVisitor.h"

#define DELETE()                             \
    ;                                        \
    Object *n = const_cast<Object *>(&node); \
    n->Clear();                              \
    delete n;

class DeallocateVisitor : public TreeVisitor {
public:
    virtual void VisitProgram(const Object &node) override { DELETE(); }
    virtual void VisitStatements(const Object &node) override { DELETE(); }
    virtual void VisitStatement(const Object &node) override { DELETE(); }
    virtual void VisitExpression(const Object &node) override { DELETE(); }
    virtual void VisitAssign(const Object &node) override { DELETE(); }
    virtual void VisitPlus(const Object &node) override { DELETE(); }
    virtual void VisitMinus(const Object &node) override { DELETE(); }
    virtual void VisitMul(const Object &node) override { DELETE(); }
    virtual void VisitDiv(const Object &node) override { DELETE(); }
    virtual void VisitModulo(const Object &node) override { DELETE(); }
    virtual void VisitGreater(const Object &node) override { DELETE(); }
    virtual void VisitLess(const Object &node) override { DELETE(); }
    virtual void VisitGreaterEqual(const Object &node) override { DELETE(); }
    virtual void VisitLessEqual(const Object &node) override { DELETE(); }
    virtual void VisitEqual(const Object &node) override { DELETE(); }
    virtual void VisitNotEqual(const Object &node) override { DELETE(); }
    virtual void VisitAnd(const Object &node) override { DELETE(); }
    virtual void VisitOr(const Object &node) override { DELETE(); }
    virtual void VisitTerm(const Object &node) override { DELETE(); }
    virtual void VisitUnaryMinus(const Object &node) override { DELETE(); }
    virtual void VisitNot(const Object &node) override { DELETE(); }
    virtual void VisitPlusPlusBefore(const Object &node) override { DELETE(); }
    virtual void VisitPlusPlusAfter(const Object &node) override { DELETE(); }
    virtual void VisitMinusMinusBefore(const Object &node) override { DELETE(); }
    virtual void VisitMinusMinusAfter(const Object &node) override { DELETE(); }
    virtual void VisitPrimary(const Object &node) override { DELETE(); }
    virtual void VisitLValue(const Object &node) override { DELETE(); }
    virtual void VisitId(const Object &node) override { DELETE(); }
    virtual void VisitLocal(const Object &node) override { DELETE(); }
    virtual void VisitDoubleColon(const Object &node) override { DELETE(); }
    virtual void VisitDollar(const Object &node) override { DELETE(); }
    virtual void VisitMember(const Object &node) override { DELETE(); }
    virtual void VisitDot(const Object &node) override { DELETE(); }
    virtual void VisitBracket(const Object &node) override { DELETE(); }
    virtual void VisitCall(const Object &node) override { DELETE(); }
    virtual void VisitCallSuffix(const Object &node) override { DELETE(); }
    virtual void VisitNormalCall(const Object &node) override { DELETE(); }
    virtual void VisitMethodCall(const Object &node) override { DELETE(); }
    virtual void VisitArgumentList(const Object &node) override { DELETE(); }
    virtual void VisitNamedArgument(const Object &node) override { DELETE(); }
    virtual void VisitExpressionList(const Object &node) override { DELETE(); }
    virtual void VisitObjectDef(const Object &node) override { DELETE(); }
    virtual void VisitIndexed(const Object &node) override { DELETE(); }
    virtual void VisitIndexedElem(const Object &node) override { DELETE(); }
    virtual void VisitBlock(const Object &node) override { DELETE(); }
    virtual void VisitFunctionDef(const Object &node) override { DELETE(); }
    virtual void VisitConst(const Object &node) override { DELETE(); }
    virtual void VisitNumber(const Object &node) override { DELETE(); }
    virtual void VisitString(const Object &node) override { DELETE(); }
    virtual void VisitNil(const Object &node) override { DELETE(); }
    virtual void VisitTrue(const Object &node) override { DELETE(); }
    virtual void VisitFalse(const Object &node) override { DELETE(); }
    virtual void VisitIdList(const Object &node) override { DELETE(); }
    virtual void VisitFormal(const Object &node) override { DELETE(); }
    virtual void VisitIf(const Object &node) override { DELETE(); }
    virtual void VisitWhile(const Object &node) override { DELETE(); }
    virtual void VisitFor(const Object &node) override { DELETE(); }
    virtual void VisitReturn(const Object &node) override { DELETE(); }
    virtual void VisitBreak(const Object &node) override { DELETE(); }
    virtual void VisitContinue(const Object &node) override { DELETE(); }

    virtual TreeVisitor *Clone(void) const override { return new DeallocateVisitor(); }
};

#endif
