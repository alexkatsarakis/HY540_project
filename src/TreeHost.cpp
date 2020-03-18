#include "TreeHost.h"
#include "Object.h"
#include "TreeTags.h"

void TreeHost::Accept (const Object& node){ 
    acceptors[node[AST_TAG_TYPE_KEY]->ToString()](node); 
}

void TreeHost::InstallAcceptor (const std::string& tag, const Acceptor& f){ 
    acceptors[tag] = f ;
}

void TreeHost::InstallAllAcceptors (void){
    /* todo when I finish the rest of the accept functions */
}

void TreeHost::AcceptProgram(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitProgram(node);
}

void TreeHost::AcceptStatements(const Object& node){
    /* loop for all numeric keys */
    for (int i = 0; i< node.GetTotal() - 1; i++)
        Accept(*node[i]->ToObject());

    visitor->VisitStatements(node);
}

void TreeHost::AcceptStatement(const Object& node){
    if (node.ElementExists(AST_TAG_CHILD))
        Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitStatement(node);
}

void TreeHost::AcceptIf(const Object& node){
    Accept(*node[AST_TAG_CONDITION]->ToObject());
    Accept(*node[AST_TAG_STMT]->ToObject());
    if (node.ElementExists(AST_TAG_ELSE_STMT))
        Accept(*node[AST_TAG_ELSE_STMT]->ToObject());
    visitor->VisitIf(node);
}

void TreeHost::AcceptReturn(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitReturn(node);
}

void TreeHost::AcceptWhile(const Object& node){
    Accept(*node[AST_TAG_CONDITION]->ToObject());
    Accept(*node[AST_TAG_STMT]->ToObject());
    visitor->VisitWhile(node);
}

void TreeHost::AcceptFor(const Object& node){
    Accept(*node[AST_TAG_FOR_PRE_ELIST]->ToObject());
    Accept(*node[AST_TAG_CONDITION]->ToObject());
    Accept(*node[AST_TAG_FOR_POST_ELIST]->ToObject());
    Accept(*node[AST_TAG_STMT]->ToObject());
    visitor->VisitFor(node);
}

void TreeHost::AcceptFunctionDef(const Object& node){
    Accept(*node[AST_TAG_FUNCTION_ID]->ToObject());
    Accept(*node[AST_TAG_FUNCTION_FORMALS]->ToObject());
    Accept(*node[AST_TAG_STMT]->ToObject());
    visitor->VisitFunctionDef(node);
}

void TreeHost::AcceptBlock(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitBlock(node);
}

void TreeHost::AcceptBreak(const Object& node){
    visitor->VisitBreak(node);
}

void TreeHost::AcceptContinue(const Object& node){
    visitor->VisitContinue(node);
}

void TreeHost::AcceptExpression(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitExpression(node);
}

void TreeHost::AcceptAssign(const Object& node){
    Accept(*node[AST_TAG_LVALUE]->ToObject());
    Accept(*node[AST_TAG_RVALUE]->ToObject());
    visitor->VisitAssign(node);
}

void TreeHost::AcceptPlus(const Object& node){
    Accept(*node[AST_TAG_FIRST_EXPR]->ToObject());
    Accept(*node[AST_TAG_SECOND_EXPR]->ToObject());
    visitor->VisitPlus(node);
}

void TreeHost::AcceptMinus(const Object& node){
    Accept(*node[AST_TAG_FIRST_EXPR]->ToObject());
    Accept(*node[AST_TAG_SECOND_EXPR]->ToObject());
    visitor->VisitMinus(node);
}

void TreeHost::AcceptMul(const Object& node){
    Accept(*node[AST_TAG_FIRST_EXPR]->ToObject());
    Accept(*node[AST_TAG_SECOND_EXPR]->ToObject());
    visitor->VisitMul(node);
}

void TreeHost::AcceptDiv(const Object& node){
    Accept(*node[AST_TAG_FIRST_EXPR]->ToObject());
    Accept(*node[AST_TAG_SECOND_EXPR]->ToObject());
    visitor->VisitDiv(node);
}

void TreeHost::AcceptModulo(const Object& node){
    Accept(*node[AST_TAG_FIRST_EXPR]->ToObject());
    Accept(*node[AST_TAG_SECOND_EXPR]->ToObject());
    visitor->VisitModulo(node);
}

void TreeHost::AcceptGreater(const Object& node){
    Accept(*node[AST_TAG_FIRST_EXPR]->ToObject());
    Accept(*node[AST_TAG_SECOND_EXPR]->ToObject());
    visitor->VisitGreater(node);
}

void TreeHost::AcceptLess(const Object& node){
    Accept(*node[AST_TAG_FIRST_EXPR]->ToObject());
    Accept(*node[AST_TAG_SECOND_EXPR]->ToObject());
    visitor->VisitLess(node);
}

void TreeHost::AcceptGreaterEqual(const Object& node){
    Accept(*node[AST_TAG_FIRST_EXPR]->ToObject());
    Accept(*node[AST_TAG_SECOND_EXPR]->ToObject());
    visitor->VisitGreaterEqual(node);
}

void TreeHost::AcceptLessEqual(const Object& node){
    Accept(*node[AST_TAG_FIRST_EXPR]->ToObject());
    Accept(*node[AST_TAG_SECOND_EXPR]->ToObject());
    visitor->VisitLessEqual(node);
}

void TreeHost::AcceptEqual(const Object& node){
    Accept(*node[AST_TAG_FIRST_EXPR]->ToObject());
    Accept(*node[AST_TAG_SECOND_EXPR]->ToObject());
    visitor->VisitEqual(node);
}

void TreeHost::AcceptNotEqual(const Object& node){
    Accept(*node[AST_TAG_FIRST_EXPR]->ToObject());
    Accept(*node[AST_TAG_SECOND_EXPR]->ToObject());
    visitor->VisitNotEqual(node);
}

void TreeHost::AcceptAnd(const Object& node){
    Accept(*node[AST_TAG_FIRST_EXPR]->ToObject());
    Accept(*node[AST_TAG_SECOND_EXPR]->ToObject());
    visitor->VisitAnd(node);
}

void TreeHost::AcceptOr(const Object& node){
    Accept(*node[AST_TAG_FIRST_EXPR]->ToObject());
    Accept(*node[AST_TAG_SECOND_EXPR]->ToObject());
    visitor->VisitOr(node);
}

void TreeHost::AcceptTerm(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitTerm(node);
}

void TreeHost::AcceptUnaryMinus(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitUnaryMinus(node);
}

void TreeHost::AcceptNot(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitNot(node);
}

void TreeHost::AcceptPlusPlusBefore(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitPlusPlusBefore(node);
}

void TreeHost::AcceptPlusPlusAfter(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitPlusPlusAfter(node);
}

void TreeHost::AcceptMinusMinusBefore(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitMinusMinusBefore(node);
}

void TreeHost::AcceptMinusMinusAfter(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitMinusMinusAfter(node);
}

void TreeHost::AcceptPrimary(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitPrimary(node);
}

void TreeHost::AcceptObjectDefList(const Object& node){
    for (int i = 0; i < node.GetTotal() - 1; i++)
        Accept(*node[i]->ToObject());
    visitor->VisitObjectDefList(node);
}

void TreeHost::AcceptObjectDef(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitObjectDef(node);
}

void TreeHost::AcceptLValue(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitLValue(node);
}

void TreeHost::AcceptConst(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitConst(node);
}

void TreeHost::AcceptNumber(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitNumber(node);
}

void TreeHost::AcceptString(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitString(node);
}

void TreeHost::AcceptNill(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitNill(node);
}

void TreeHost::AcceptTrue(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitTrue(node);
}

void TreeHost::AcceptFalse(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitFalse(node);
}

void TreeHost::AcceptId(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitId(node);
}

void TreeHost::AcceptLocal(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitLocal(node);
}

void TreeHost::AcceptDoubleColon(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitDoubleColon(node);
}

void TreeHost::AcceptMember(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitMember(node);
}

void TreeHost::AcceptDot(const Object& node){
    Accept(*node[AST_TAG_LVALUE]->ToObject());
    Accept(*node[AST_TAG_ID]->ToObject());
    visitor->VisitDot(node);
}

void TreeHost::AcceptBracket(const Object& node){
    Accept(*node[AST_TAG_LVALUE]->ToObject());
    Accept(*node[AST_TAG_EXPR]->ToObject());
    visitor->VisitBracket(node);
}

void TreeHost::AcceptCallSuffix(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitCallSuffix(node);
}

void TreeHost::AcceptNormalCall(const Object& node){
    Accept(*node[AST_TAG_ARGUMENTS]->ToObject());
    visitor->VisitNormalCall(node);
}

void TreeHost::AcceptMethodCall(const Object& node){
    Accept(*node[AST_TAG_FUNCTION]->ToObject());
    Accept(*node[AST_TAG_ARGUMENTS]->ToObject());
    visitor->VisitMethodCall(node);
}

void TreeHost::AcceptCall(const Object& node){
    Accept(*node[AST_TAG_FUNCTION]->ToObject());
    if (node.ElementExists(AST_TAG_SUFFIX))
        Accept(*node[AST_TAG_SUFFIX]->ToObject());
    else
        Accept(*node[AST_TAG_ARGUMENTS]->ToObject());
    visitor->VisitCall(node);
}

void TreeHost::AcceptExpressionList(const Object& node){
    for (int i = 0; i < node.GetTotal() - 1; i++)
        Accept(*node[i]->ToObject());
    visitor->VisitExpressionList(node);    
}

void TreeHost::AcceptCommaExpressions(const Object& node){
    for (int i = 0; i < node.GetTotal() - 1; i++)
        Accept(*node[i]->ToObject());
    visitor->VisitCommaExpressions(node);    
}

void TreeHost::AcceptObjectDef(const Object& node){
    Accept(*node[AST_TAG_OBJECT_DEF_VAL]->ToObject());
    visitor->VisitObjectDef(node);
}

void TreeHost::AcceptIndexed(const Object& node){
    Accept(*node[AST_TAG_INDEXED_ELEM]->ToObject());
    Accept(*node[AST_TAG_COMMA_INDEXED_ELEMS]->ToObject());
    visitor->VisitIndexed(node);
}

void TreeHost::AcceptCommaIndexedElems(const Object& node){
    for (int i = 0; i < node.GetTotal() - 1; i++)
        Accept(*node[i]->ToObject());
    visitor->VisitCommaIndexedElems(node);
}

void TreeHost::AcceptIndexedElem(const Object& node){
    Accept(*node[AST_TAG_OBJECT_KEY]->ToObject());
    Accept(*node[AST_TAG_OBJECT_VALUE]->ToObject());
    visitor->VisitCommaIndexedElems(node);
}

void TreeHost::AcceptIdList(const Object& node){
    for (int i = 0; i < node.GetTotal() - 1; i++)
        Accept(*node[i]->ToObject());
    visitor->VisitIdList(node);
}