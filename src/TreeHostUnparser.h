#ifndef _TREE_HOSTUNPARSER_H_
#define _TREE_HOSTUNPARSER_H_

#include "TreeHost.h"

class Object;
class TreeHostUnparser : public TreeHost {
   public:
	void UnparseProgram(const Object &node);
	void UnparseStatements(const Object &node);
	void UnparseStatement(const Object &node);
	void UnparseExpressionList(const Object &node);
	void UnparseCommaExpressions(const Object &node);
	void UnparseExpression(const Object &node);
	void UnparseIf(const Object &node);
	void UnparseWhile(const Object &node);
	void UnparseFor(const Object &node);
	void UnparseReturn(const Object &node);
	void UnparseBreak(const Object &node);
	void UnparseContinue(const Object &node);
	void UnparseFunctionDef(const Object &node);
	void UnparseBlock(const Object &node);
	void UnparseEmptyStatement(const Object &node); /* semicolon */

	void UnparseAssign(const Object &node);
	void UnparsePlus(const Object &node);
	void UnparseMinus(const Object &node);
	void UnparseMul(const Object &node);
	void UnparseDiv(const Object &node);
	void UnparseModulo(const Object &node);
	void UnparseGreater(const Object &node);
	void UnparseLess(const Object &node);
	void UnparseGreaterEqual(const Object &node);
	void UnparseLessEqual(const Object &node);
	void UnparseEqual(const Object &node);
	void UnparseNotEqual(const Object &node);
	void UnparseAnd(const Object &node);
	void UnparseOr(const Object &node);
	void UnparseTerm(const Object &node);

	void UnparseUnaryMinus(const Object &node);
	void UnparseNot(const Object &node);
	void UnparsePlusPlusBefore(const Object &node);
	void UnparsePlusPlusAfter(const Object &node);
	void UnparseMinusMinusBefore(const Object &node);
	void UnparseMinusMinusAfter(const Object &node);
	void UnparsePrimary(const Object &node);

	void UnparseLValue(const Object &node);
	void UnparseNumber(const Object &node);
	void UnparseString(const Object &node);
	void UnparseNill(const Object &node);
	void UnparseTrue(const Object &node);
	void UnparseFalse(const Object &node);

	void UnparseId(const Object &node);

	void UnparseLocal(const Object &node);
	void UnparseDoubleColon(const Object &node);

	void UnparseCall(const Object &node);
	void UnparseObjectDefList(const Object &node);
	void UnparseObjectDef(const Object &node);
	void UnparseConst(const Object &node);

	void UnparseMember(const Object &node);
	void UnparseDot(const Object &node);
	void UnparseBracket(const Object &node);

	void UnparseCallSuffix(const Object &node);

	void UnparseNormalCall(const Object &node);
	void UnparseMethodCall(const Object &node);
	void UnparseMethodCallId(const Object &node);

	void UnparseIndexed(const Object &node);
	void UnparseCommaIndexedElems(const Object &node);
	void UnparseIndexedElem(const Object &node);

	void UnparseIdList(const Object &node);
};

#endif
