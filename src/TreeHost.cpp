#include "TreeHost.h"
#include "Object.h"
#include "AstTags.h"

void TreeHost::Accept (const Object& node){ 
    acceptors[node[AST_TAG_TYPE_KEY]->ToString()](node); 
}

void TreeHost::InstallAcceptor (const std::string& tag, const Acceptor& f){ 
    acceptors[tag] = f ;
}

void TreeHost::InstallAllAcceptors (void){
    /* todo when I finish the rest of the accept functions */
}

void TreeHost::AcceptStatement(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitStmt(node);
}

void TreeHost::AcceptExpression(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitStmt(node);
}

/* maybe not intuitive order? */
void TreeHost::AcceptIf(const Object& node){
    Accept(*node[AST_TAG_IF_COND]->ToObject());
    Accept(*node[AST_TAG_IF_STMT]->ToObject());
    if (node.ElementExists(AST_TAG_ELSE_STMT))
        Accept(*node[AST_TAG_ELSE_STMT]->ToObject());    
    visitor->VisitIf(node);
}

void TreeHost::AcceptWhile(const Object& node){
    Accept(*node[AST_TAG_WHILE_COND]->ToObject());
    Accept(*node[AST_TAG_WHILE_STMT]->ToObject());
    visitor->VisitWhile(node);
}

void TreeHost::AcceptFor(const Object& node){
    Accept(*node[AST_TAG_FOR_INIT]->ToObject());
    Accept(*node[AST_TAG_FOR_COND]->ToObject());
    Accept(*node[AST_TAG_FOR_STEP]->ToObject());
    Accept(*node[AST_TAG_FOR_STMT]->ToObject());
    visitor->VisitFor(node);
}

/*  
    maybe the parse function for returnstmt can create an 
    empty statement node so AST_TAG_RET_VAL always exists?
*/
void TreeHost::AcceptReturn(const Object& node){
    if (node.ElementExists(AST_TAG_RET_VAL))
        Accept(*node[AST_TAG_RET_VAL]->ToObject());
    visitor->VisitReturn(node);
}

void TreeHost::AcceptBreak(const Object& node){
    visitor->VisitBreak(node);
}

void TreeHost::AcceptContinue(const Object& node){
    visitor->VisitContinue(node);
}

void TreeHost::AcceptFunctionDef(const Object& node){
    Accept(*node[AST_TAG_FUNC_ID]->ToObject());
    Accept(*node[AST_TAG_FUNC_ARGS]->ToObject());
    Accept(*node[AST_TAG_FUNC_CODE]->ToObject());
    visitor->VisitFunctionDef(node);
}

void TreeHost::AcceptBlock(const Object& node){
    Accept(*node[AST_TAG_BLOCK_STMTS]->ToObject());
    visitor->VisitBlock(node);
}

/* can be skipped? */
void TreeHost::AcceptEmptyStatement(const Object& node){
    visitor->VisitEmptyStatement(node);
}

void TreeHost::AcceptAssign(const Object& node){
    Accept(*node[AST_TAG_LVALUE]->ToObject());
    Accept(*node[AST_TAG_RVALUE]->ToObject());
    visitor->VisitAssign(node);
}

void TreeHost::AcceptPlus(const Object& node){
    Accept(*node[AST_TAG_LEFT_OPERAND]->ToObject());
    Accept(*node[AST_TAG_RIGHT_OPERAND]->ToObject());
    visitor->VisitPlus(node);
}

void TreeHost::AcceptMinus(const Object& node){
    Accept(*node[AST_TAG_LEFT_OPERAND]->ToObject());
    Accept(*node[AST_TAG_RIGHT_OPERAND]->ToObject());
    visitor->VisitMinus(node);
}

void TreeHost::AcceptMul(const Object& node){
    Accept(*node[AST_TAG_LEFT_OPERAND]->ToObject());
    Accept(*node[AST_TAG_RIGHT_OPERAND]->ToObject());
    visitor->VisitMul(node);
}
void TreeHost::AcceptDiv(const Object& node){
    Accept(*node[AST_TAG_LEFT_OPERAND]->ToObject());
    Accept(*node[AST_TAG_RIGHT_OPERAND]->ToObject());
    visitor->VisitDiv(node);
}

void TreeHost::AcceptModulo(const Object& node){
    Accept(*node[AST_TAG_LEFT_OPERAND]->ToObject());
    Accept(*node[AST_TAG_RIGHT_OPERAND]->ToObject());
    visitor->VisitModulo(node);
}

void TreeHost::AcceptGreater(const Object& node){
    Accept(*node[AST_TAG_LEFT_OPERAND]->ToObject());
    Accept(*node[AST_TAG_RIGHT_OPERAND]->ToObject());
    visitor->VisitGreater(node);
}

void TreeHost::AcceptLess(const Object& node){
    Accept(*node[AST_TAG_LEFT_OPERAND]->ToObject());
    Accept(*node[AST_TAG_RIGHT_OPERAND]->ToObject());
    visitor->VisitLess(node);
}

void TreeHost::AcceptGreaterEqual(const Object& node){
    Accept(*node[AST_TAG_LEFT_OPERAND]->ToObject());
    Accept(*node[AST_TAG_RIGHT_OPERAND]->ToObject());
    visitor->VisitGreaterEqual(node);
}

void TreeHost::AcceptLessEqual(const Object& node){
    Accept(*node[AST_TAG_LEFT_OPERAND]->ToObject());
    Accept(*node[AST_TAG_RIGHT_OPERAND]->ToObject());
    visitor->VisitLessEqual(node);
}

void TreeHost::AcceptEqual(const Object& node){
    Accept(*node[AST_TAG_LEFT_OPERAND]->ToObject());
    Accept(*node[AST_TAG_RIGHT_OPERAND]->ToObject());
    visitor->VisitEqual(node);
}

void TreeHost::AcceptNotEqual(const Object& node){
    Accept(*node[AST_TAG_LEFT_OPERAND]->ToObject());
    Accept(*node[AST_TAG_RIGHT_OPERAND]->ToObject());
    visitor->VisitNotEqual(node);
}

void TreeHost::AcceptAnd(const Object& node){
    Accept(*node[AST_TAG_LEFT_OPERAND]->ToObject());
    Accept(*node[AST_TAG_RIGHT_OPERAND]->ToObject());
    visitor->VisitAnd(node);
}

void TreeHost::AcceptOr(const Object& node){
    Accept(*node[AST_TAG_LEFT_OPERAND]->ToObject());
    Accept(*node[AST_TAG_RIGHT_OPERAND]->ToObject());
    visitor->VisitOr(node);
}

void TreeHost::AcceptTerm(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitTerm(node);
}

void TreeHost::AcceptUnaryMinus(const Object& node){
    Accept(*node[AST_TAG_OPERAND]->ToObject());
    visitor->VisitUnaryMinus(node);
}

void TreeHost::AcceptNot(const Object& node){
    Accept(*node[AST_TAG_OPERAND]->ToObject());
    visitor->VisitNot(node);
}

void TreeHost::AcceptPlusPlusBefore(const Object& node){
    Accept(*node[AST_TAG_OPERAND]->ToObject());
    visitor->VisitPlusPlusBefore(node);
}

void TreeHost::AcceptPlusPlusAfter(const Object& node){
    Accept(*node[AST_TAG_OPERAND]->ToObject());
    visitor->VisitPlusPlusAfter(node);
}

void TreeHost::AcceptMinusMinusBefore(const Object& node){
    Accept(*node[AST_TAG_OPERAND]->ToObject());
    visitor->VisitMinusMinusBefore(node);
}

void TreeHost::AcceptMinusMinusAfter(const Object& node){
    Accept(*node[AST_TAG_OPERAND]->ToObject());
    visitor->VisitMinusMinusAfter(node);
}

void TreeHost::AcceptPrimary(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitPrimary(node);
}

void TreeHost::AcceptLvalue(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitLvalue(node);
}

/* is there a better way ? grammar could be simpler, maybe eliminate call suffix ? */
void TreeHost::AcceptCall(const Object& node){
    Accept(*node[AST_TAG_FUNCTION_CALLED]->ToObject());
    if (node.ElementExists(AST_TAG_CALL_SUFFIX))
        Accept(*node[AST_TAG_CALL_SUFFIX]->ToObject());
    else
        Accept(*node[AST_TAG_FUNC_ARGS]->ToObject());
    visitor->VisitCall(node);
}

void TreeHost::AcceptObjectDef(const Object& node){

}

void TreeHost::AcceptConst(const Object& node){

}

void TreeHost::AcceptCallSuffix(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitCallSuffix(node);
}

void TreeHost::AcceptMethodCall(const Object& node){
    Accept(*node[AST_TAG_FUNC_ID]->ToObject());
    Accept(*node[AST_TAG_FUNC_ARGS]->ToObject());
    visitor->VisitMethodCall(node);
}

void TreeHost::AcceptNormalCall(const Object& node){
    Accept(*node[AST_TAG_FUNC_ARGS]->ToObject());
    visitor->VisitMethodCall(node);
}