#ifndef _VISUALIZE_VISITOR_H_
#define _VISUALIZE_VISITOR_H_

#include "TreeVisitor.h"

#include <fstream>
#include <sstream>
#include <stack>

class VisualizeVisitor : public TreeVisitor {
private:
    unsigned lastNode;
    std::stringstream output;
    std::string outputFile;
    std::stack<unsigned> orphans;

    void CreateNewNode(const std::string &str);

    void LinkToPreviousNode(void);

    void LinkToOrphan();

    void LinkToNode(unsigned node);

    void SaveOrphan(void);

    void DumpToFile(void);

    void VisitEmptyStatements(const Object &node);

    void VisitNormalStatements(const Object &node);

    void VisitEmptyStatement(const Object &node);

    void VisitNormalStatement(const Object &node);

    void VisitEmptyExpressionList(const Object &node);

    void VisitNormalExpressionList(const Object &node);

    void VisitEmptyIndexed(const Object &node);

    void VisitNormalIndexed(const Object &node);

    void VisitEmptyIdlist(const Object &node);

    void VisitNormalIdlist(const Object &node);

public:
    virtual void VisitProgram(const Object &node) override;
    virtual void VisitStatements(const Object &node) override;
    virtual void VisitStatement(const Object &node) override;
    virtual void VisitExpression(const Object &node) override;
    virtual void VisitAssign(const Object &node) override;
    virtual void VisitPlus(const Object &node) override;
    virtual void VisitMinus(const Object &node) override;
    virtual void VisitMul(const Object &node) override;
    virtual void VisitDiv(const Object &node) override;
    virtual void VisitModulo(const Object &node) override;
    virtual void VisitGreater(const Object &node) override;
    virtual void VisitLess(const Object &node) override;
    virtual void VisitGreaterEqual(const Object &node) override;
    virtual void VisitLessEqual(const Object &node) override;
    virtual void VisitEqual(const Object &node) override;
    virtual void VisitNotEqual(const Object &node) override;
    virtual void VisitAnd(const Object &node) override;
    virtual void VisitOr(const Object &node) override;
    virtual void VisitTerm(const Object &node) override;
    virtual void VisitUnaryMinus(const Object &node) override;
    virtual void VisitNot(const Object &node) override;
    virtual void VisitPlusPlusBefore(const Object &node) override;
    virtual void VisitPlusPlusAfter(const Object &node) override;
    virtual void VisitMinusMinusBefore(const Object &node) override;
    virtual void VisitMinusMinusAfter(const Object &node) override;
    virtual void VisitPrimary(const Object &node) override;
    virtual void VisitLValue(const Object &node) override;
    virtual void VisitId(const Object &node) override;
    virtual void VisitLocal(const Object &node) override;
    virtual void VisitDoubleColon(const Object &node) override;
    virtual void VisitDollar(const Object &node) override;
    virtual void VisitMember(const Object &node) override;
    virtual void VisitDot(const Object &node) override;
    virtual void VisitBracket(const Object &node) override;
    virtual void VisitCall(const Object &node) override;
    virtual void VisitCallSuffix(const Object &node) override;
    virtual void VisitNormalCall(const Object &node) override;
    virtual void VisitMethodCall(const Object &node) override;
    virtual void VisitExpressionList(const Object &node) override;
    virtual void VisitObjectDef(const Object &node) override;
    virtual void VisitIndexed(const Object &node) override;
    virtual void VisitIndexedElem(const Object &node) override;
    virtual void VisitBlock(const Object &node) override;
    virtual void VisitFunctionDef(const Object &node) override;
    virtual void VisitConst(const Object &node) override;
    virtual void VisitNumber(const Object &node) override;
    virtual void VisitString(const Object &node) override;
    virtual void VisitNil(const Object &node) override;
    virtual void VisitTrue(const Object &node) override;
    virtual void VisitFalse(const Object &node) override;
    virtual void VisitIdList(const Object &node) override;
    virtual void VisitFormal(const Object &node) override;
    virtual void VisitIf(const Object &node) override;
    virtual void VisitWhile(const Object &node) override;
    virtual void VisitFor(const Object &node) override;
    virtual void VisitReturn(const Object &node) override;
    virtual void VisitBreak(const Object &node) override;
    virtual void VisitContinue(const Object &node) override;

    virtual TreeVisitor *Clone(void) const override;

    VisualizeVisitor(void);
    VisualizeVisitor(const std::string &filename);
    VisualizeVisitor(const VisualizeVisitor &) = default;

    virtual ~VisualizeVisitor();
};

#endif
