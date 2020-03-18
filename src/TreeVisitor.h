#ifndef _TREE_VISITOR_H_
#define _TREE_VISITOR_H_

class Object;

class TreeVisitor {
public:

    virtual void VisitProgram (const Object& node) = 0;
    virtual void VisitStmts (const Object& node) = 0;
	virtual void VisitStmt (const Object& node) = 0; 
	virtual void VisitExpr (const Object& node) = 0; 
	virtual void VisitIf (const Object& node) = 0; 
	virtual void VisitWhile (const Object& node) = 0;
    virtual void VisitFor (const Object& node) = 0; 
    virtual void VisitReturn (const Object& node) = 0;
    virtual void VisitReturnValue(const Object& node) = 0;
    virtual void VisitBreak (const Object& node) = 0;
    virtual void VisitContinue (const Object& node) = 0;
	virtual void VisitFunctionDef (const Object& node) = 0;
    virtual void VisitFuncId (const Object& node) = 0; 
	virtual void VisitBlock (const Object& node) = 0; 
    virtual void VisitEmptyStatement (const Object& node) = 0; 
    virtual void VisitAssign (const Object& node) = 0; 
  	virtual void VisitPlus (const Object& node) = 0;
	virtual void VisitMinus (const Object& node) = 0;
	virtual void VisitMul (const Object& node) = 0;
	virtual void VisitDiv (const Object& node) = 0;
	virtual void VisitModulo (const Object& node) = 0;
    virtual void VisitGreater (const Object& node) = 0;
	virtual void VisitLess (const Object& node) = 0;
	virtual void VisitGreaterEqual (const Object& node) = 0;
	virtual void VisitLessEqual (const Object& node) = 0;
	virtual void VisitEqual (const Object& node) = 0;
	virtual void VisitNotEqual (const Object& node) = 0;
	virtual void VisitAnd (const Object& node) = 0;
	virtual void VisitOr (const Object& node) = 0;
    virtual void VisitTerm (const Object& node) = 0;
	virtual void VisitUnaryMinus (const Object& node) = 0;
	virtual void VisitNot (const Object& node) = 0;
	virtual void VisitPlusPlusBefore (const Object& node) = 0;
	virtual void VisitPlusPlusAfter (const Object& node) = 0;
	virtual void VisitMinusMinusBefore (const Object& node) = 0;
	virtual void VisitMinusMinusAfter (const Object& node) = 0;
	virtual void VisitPrimary (const Object& node) = 0;
    virtual void VisitObjectDefList (const Object& node) = 0;
    virtual void VisitObjectDefIndex (const Object& node) = 0;
    virtual void VisitLvalue (const Object& node) = 0;
    virtual void VisitConsts (const Object& node) = 0;
    virtual void VisitConst (const Object& node) = 0;
    virtual void VisitNumber (const Object& node) = 0;
    virtual void VisitString (const Object& node) = 0;
    virtual void VisitNill (const Object& node) = 0;
    virtual void VisitTrue (const Object& node) = 0;
    virtual void VisitFalse (const Object& node) = 0;
    virtual void VisitId (const Object& node) = 0;
    virtual void VisitLocalId (const Object& node) = 0;
    virtual void VisitDoubleColonId (const Object& node) = 0;
    virtual void VisitMember (const Object& node) = 0;
    virtual void VisitDot (const Object& node) = 0;
    virtual void VisitBracket (const Object& node) = 0;
    virtual void VisitCall (const Object& node) = 0;
    virtual void VisitCallSuffix (const Object& node) = 0;
    virtual void VisitNormalCall (const Object& node) = 0;
    virtual void VisitMethodCall (const Object& node) = 0;
    virtual void VisitExpressionList (const Object& node) = 0;
    virtual void VisitCommaExpressions (const Object& node) = 0;
    virtual void VisitObjectDef (const Object& node) = 0;
    virtual void VisitIndexed (const Object& node) = 0;
    virtual void VisitCommaIndexedElems (const Object& node) = 0;
    virtual void VisitIndexedElem (const Object& node) = 0;
    virtual void VisitIdList (const Object& node) = 0;
    virtual void VisitCommaIds (const Object& node) = 0;
   
	virtual TreeVisitor* Clone (void) const = 0;
	TreeVisitor(void);
	TreeVisitor (const TreeVisitor&)=default;
};

#endif