#ifndef _UNPARSEVISITOR_H_
#define _UNPARSEVISITOR_H_

#include "TreeVisitor.h"

#include <string>
#include <vector>

class UnparseVisitor : public TreeVisitor {
private:
    std::string fileName;
    void WriteFile(const std::string &program);
    const std::string GetUnparsed(const Value *val) const;

    const std::string UnparseProgram(const std::string &stmts);
    const std::string UnparseStatements(const std::vector<std::string> &statements);
    const std::string UnparseStatement(const std::string &stmt = "");
    const std::string UnparseStatementNOSEMICOLON(const std::string &stmt);
    const std::string UnparseExpression(const std::string &expr);
    const std::string UnparseAssign(const std::string &lvalue, const std::string &rvalue);
    const std::string UnparsePlus(const std::string &expr1, const std::string &expr2);
    const std::string UnparseMinus(const std::string &expr1, const std::string &expr2);
    const std::string UnparseMul(const std::string &expr1, const std::string &expr2);
    const std::string UnparseDiv(const std::string &expr1, const std::string &expr2);
    const std::string UnparseModulo(const std::string &expr1, const std::string &expr2);
    const std::string UnparseGreater(const std::string &expr1, const std::string &expr2);
    const std::string UnparseLess(const std::string &expr1, const std::string &expr2);
    const std::string UnparseGreaterEqual(const std::string &expr1, const std::string &expr2);
    const std::string UnparseLessEqual(const std::string &expr1, const std::string &expr2);
    const std::string UnparseEqual(const std::string &expr1, const std::string &expr2);
    const std::string UnparseNotEqual(const std::string &expr1, const std::string &expr2);
    const std::string UnparseAnd(const std::string &expr1, const std::string &expr2);
    const std::string UnparseOr(const std::string &expr1, const std::string &expr2);
    const std::string UnparseTerm(const std::string &expr);
    const std::string UnparseTermPARENTHESIS(const std::string &expr);
    const std::string UnparseUnaryMinus(const std::string &expr);
    const std::string UnparseNot(const std::string &expr);
    const std::string UnparsePlusPlusBefore(const std::string &expr);
    const std::string UnparsePlusPlusAfter(const std::string &expr);
    const std::string UnparseMinusMinusBefore(const std::string &expr);
    const std::string UnparseMinusMinusAfter(const std::string &expr);
    const std::string UnparsePrimary(const std::string &primary);
    const std::string UnparsePrimaryPARENTHESIS(const std::string &funcdef);
    const std::string UnparseLValue(const std::string &lvalue);
    const std::string UnparseId(const std::string &value);
    const std::string UnparseLocal(const std::string &value);
    const std::string UnparseDoubleColon(const std::string &value);
    const std::string UnparseDollar(const std::string &value);
    const std::string UnparseMember(const std::string &member);
    const std::string UnparseDot(const std::string &lvalue, const std::string &id);
    const std::string UnparseBracket(const std::string &lvalue, const std::string &id);
    const std::string UnparseCallPARENTHESIS(const std::string &call, const std::string &arglist);
    const std::string UnparseCall(const std::string &call, const std::string &arglist);
    const std::string UnparseCallPARENTHESISTWIN(const std::string &funcdef, const std::string &arglist);
    const std::string UnparseCallSuffix(const std::string &call);
    const std::string UnparseNormalCall(const std::string &arglist);
    const std::string UnparseMethodCall(const std::string &id, const std::string &arglist);
    const std::string UnparseArgumentList(const std::vector<std::string> &posArguments, const std::vector<std::string> &namedArguments);
    const std::string UnparseExpressionList(const std::vector<std::string> &expressions);
    const std::string UnparseObjectDef(const std::string &child);
    const std::string UnparseIndexed(const std::vector<std::string> &indexedElements);
    const std::string UnparseIndexedElem(const std::string &key, const std::string &value);
    const std::string UnparseBlock(const std::string &stmts);
    const std::string UnparseFunctionDef(const std::string &id, const std::string &idlist, const std::string &block);
    const std::string UnparseConst(const std::string &child);
    const std::string UnparseNumber(const double &value);
    const std::string UnparseString(const std::string &value);
    const std::string UnparseNil();
    const std::string UnparseTrue();
    const std::string UnparseFalse();
    const std::string UnparseIdList(const std::vector<std::string> &ids);
    const std::string UnparseFormal(const std::string &value);
    const std::string UnparseIf(const std::string &cond, const std::string &stmt, const std::string &elseStmt = "");
    const std::string UnparseWhile(const std::string &expr, const std::string &stmt);
    const std::string UnparseFor(const std::string &elist1, const std::string &expr, const std::string &elist2, const std::string &stmt);
    const std::string UnparseReturn(const std::string &expr = "");
    const std::string UnparseBreak(void);
    const std::string UnparseContinue(void);

public:
    UnparseVisitor(const std::string &_fileName = "alpha_unparse.alpha");
    virtual ~UnparseVisitor(){};
    TreeVisitor *Clone(void) const override;

    void VisitProgram(const Object &node) override;
    void VisitStatements(const Object &node) override;
    void VisitStatement(const Object &node) override;
    void VisitExpression(const Object &node) override;
    void VisitAssign(const Object &node) override;
    void VisitPlus(const Object &node) override;
    void VisitMinus(const Object &node) override;
    void VisitMul(const Object &node) override;
    void VisitDiv(const Object &node) override;
    void VisitModulo(const Object &node) override;
    void VisitGreater(const Object &node) override;
    void VisitLess(const Object &node) override;
    void VisitGreaterEqual(const Object &node) override;
    void VisitLessEqual(const Object &node) override;
    void VisitEqual(const Object &node) override;
    void VisitNotEqual(const Object &node) override;
    void VisitAnd(const Object &node) override;
    void VisitOr(const Object &node) override;
    void VisitTerm(const Object &node) override;
    void VisitUnaryMinus(const Object &node) override;
    void VisitNot(const Object &node) override;
    void VisitPlusPlusBefore(const Object &node) override;
    void VisitPlusPlusAfter(const Object &node) override;
    void VisitMinusMinusBefore(const Object &node) override;
    void VisitMinusMinusAfter(const Object &node) override;
    void VisitPrimary(const Object &node) override;
    void VisitLValue(const Object &node) override;
    void VisitId(const Object &node) override;
    void VisitLocal(const Object &node) override;
    void VisitDoubleColon(const Object &node) override;
    void VisitDollar(const Object &node) override;
    void VisitMember(const Object &node) override;
    void VisitDot(const Object &node) override;
    void VisitBracket(const Object &node) override;
    void VisitCall(const Object &node) override;
    void VisitCallSuffix(const Object &node) override;
    void VisitNormalCall(const Object &node) override;
    void VisitMethodCall(const Object &node) override;
    void VisitArgumentList(const Object &node) override;
    void VisitExpressionList(const Object &node) override;
    void VisitObjectDef(const Object &node) override;
    void VisitIndexed(const Object &node) override;
    void VisitIndexedElem(const Object &node) override;
    void VisitBlock(const Object &node) override;
    void VisitFunctionDef(const Object &node) override;
    void VisitConst(const Object &node) override;
    void VisitNumber(const Object &node) override;
    void VisitString(const Object &node) override;
    void VisitNil(const Object &node) override;
    void VisitTrue(const Object &node) override;
    void VisitFalse(const Object &node) override;
    void VisitIdList(const Object &node) override;
    void VisitFormal(const Object &node) override;
    void VisitIf(const Object &node) override;
    void VisitWhile(const Object &node) override;
    void VisitFor(const Object &node) override;
    void VisitReturn(const Object &node) override;
    void VisitBreak(const Object &node) override;
    void VisitContinue(const Object &node) override;
};

#endif
