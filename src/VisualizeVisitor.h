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

    void CreateNewNode(const std::string & str);

    void LinkToPreviousNode(void);

    void LinkToOrphan();

    void LinkToNode(unsigned node);

    void SaveOrphan(void);

    void VisitEmptyStatement(const Object& node);

    void VisitNormalStatemens(const Object& node);

    void DumpToFile(void);

public:
    virtual void VisitProgram (const Object& node);
    virtual void VisitStatements (const Object& node);
	virtual void VisitStatement (const Object& node);
	virtual void VisitExpression (const Object& node);
    virtual void VisitAssign (const Object& node);
  	virtual void VisitPlus (const Object& node);
	virtual void VisitMinus (const Object& node);
	virtual void VisitMul (const Object& node);
	virtual void VisitDiv (const Object& node);
	virtual void VisitModulo (const Object& node);
    virtual void VisitGreater (const Object& node);
	virtual void VisitLess (const Object& node);
	virtual void VisitGreaterEqual (const Object& node);
	virtual void VisitLessEqual (const Object& node);
	virtual void VisitEqual (const Object& node);
	virtual void VisitNotEqual (const Object& node);
	virtual void VisitAnd (const Object& node);
	virtual void VisitOr (const Object& node);
    virtual void VisitTerm (const Object& node);
	virtual void VisitUnaryMinus (const Object& node);
	virtual void VisitNot (const Object& node);
	virtual void VisitPlusPlusBefore (const Object& node);
	virtual void VisitPlusPlusAfter (const Object& node);
	virtual void VisitMinusMinusBefore (const Object& node);
	virtual void VisitMinusMinusAfter (const Object& node);
	virtual void VisitPrimary (const Object& node);
    virtual void VisitLValue (const Object& node);
    virtual void VisitId (const Object& node);
    virtual void VisitLocal (const Object& node);
    virtual void VisitDoubleColon (const Object& node);
    virtual void VisitMember (const Object& node);
    virtual void VisitDot (const Object& node);
    virtual void VisitBracket (const Object& node);
    virtual void VisitCall (const Object& node);
    virtual void VisitCallSuffix (const Object& node);
    virtual void VisitNormalCall (const Object& node);
    virtual void VisitMethodCall (const Object& node);
    virtual void VisitExpressionList (const Object& node);
    virtual void VisitObjectDef (const Object& node);
    virtual void VisitIndexed (const Object& node);
    virtual void VisitIndexedElem (const Object& node);
	virtual void VisitBlock (const Object& node);
	virtual void VisitFunctionDef (const Object& node);
    virtual void VisitConst (const Object& node);
    virtual void VisitNumber (const Object& node);
    virtual void VisitString (const Object& node);
    virtual void VisitNill (const Object& node);
    virtual void VisitTrue (const Object& node);
    virtual void VisitFalse (const Object& node);
    virtual void VisitIdList (const Object& node);
	virtual void VisitIf (const Object& node);
	virtual void VisitWhile (const Object& node);
    virtual void VisitFor (const Object& node);
    virtual void VisitReturn (const Object& node);
    virtual void VisitBreak (const Object& node);
    virtual void VisitContinue (const Object& node);


	virtual TreeVisitor * Clone (void) const;
	VisualizeVisitor(void);
    VisualizeVisitor(const std::string & filename);
    VisualizeVisitor (const VisualizeVisitor &)=default;
};

#endif