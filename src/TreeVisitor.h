#ifndef _TREE_VISITOR_H_
#define _TREE_VISITOR_H_

class Object;

class TreeVisitor {
	public:
	virtual void VisitStmt (const Object& node) = 0; 
	virtual void VisitExpr (const Object& node) = 0; 
	virtual void VisitIf (const Object& node) = 0; 
	virtual void VisitWhile (const Object& node) = 0;
    virtual void VisitFor (const Object& node) = 0; 
    virtual void VisitReturn (const Object& node) = 0;
    virtual void VisitBreak (const Object& node) = 0;
    virtual void VisitContinue (const Object& node) = 0;
	virtual void VisitFunctionDef (const Object& node) = 0; 
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
    virtual void VisitLvalue (const Object& node) = 0;
    virtual void VisitCall (const Object& node) = 0;
    virtual void VisitCallSuffix (const Object& node) = 0;
    virtual void VisitNormalCall (const Object& node) = 0;
    virtual void VisitMethodCall (const Object& node) = 0;

	virtual void VisitVarDecl (const Object& node) = 0;
   
	virtual TreeVisitor* Clone (void) const = 0;
	TreeVisitor(void);
	TreeVisitor (const TreeVisitor&)=default;
};

#endif