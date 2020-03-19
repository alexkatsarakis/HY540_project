#ifndef _UNPARSER_H_
#define _UNPARSER_H_

#include "TreeVisitor.h"

#include <stack>
#include <string>

class Unparser : public TreeVisitor {
   public:
	std::stack<std::string> stack;

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
	void VisitMember(const Object &node) override;
	void VisitDot(const Object &node) override;
	void VisitBracket(const Object &node) override;
	void VisitCall(const Object &node) override;
	void VisitCallSuffix(const Object &node) override;
	void VisitNormalCall(const Object &node) override;
	void VisitMethodCall(const Object &node) override;
	void VisitExpressionList(const Object &node) override;
	void VisitObjectDef(const Object &node) override;
	void VisitIndexed(const Object &node) override;
	void VisitIndexedElem(const Object &node) override;
	void VisitBlock(const Object &node) override;
	void VisitFunctionDef(const Object &node) override;
	void VisitConst(const Object &node) override;
	void VisitNumber(const Object &node) override;
	void VisitString(const Object &node) override;
	void VisitNill(const Object &node) override;
	void VisitTrue(const Object &node) override;
	void VisitFalse(const Object &node) override;
	void VisitIdList(const Object &node) override;
	void VisitIf(const Object &node) override;
	void VisitWhile(const Object &node) override;
	void VisitFor(const Object &node) override;
	void VisitReturn(const Object &node) override;
	void VisitBreak(const Object &node) override;
	void VisitContinue(const Object &node) override;

	TreeVisitor *Clone(void) const override;
	Unparser(void);
	Unparser(const Unparser &) = default;
};

#endif
