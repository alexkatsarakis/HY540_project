#include "TreeHost.h"
#include "Object.h"
#include "TreeTags.h"

#define LAMBDA(f) [this](const Object& node) { f(node); }

void TreeHost::Accept (const Object& node){ 
    acceptors[node[AST_TAG_TYPE_KEY]->ToString()](node); 
}

void TreeHost::InstallAcceptor (const std::string& tag, const Acceptor& f){ 
    acceptors[tag] = f ;
}

void TreeHost::InstallAllAcceptors (void){
    InstallAcceptor(AST_TAG_PROGRAM,  LAMBDA(AcceptProgram));
    InstallAcceptor(AST_TAG_STMTS, LAMBDA(AcceptStatements));
    InstallAcceptor(AST_TAG_STMT, LAMBDA(AcceptStatement));
    InstallAcceptor(AST_TAG_EXPR, LAMBDA(AcceptExpression));
    InstallAcceptor(AST_TAG_ASSIGN, LAMBDA(AcceptAssign));
    InstallAcceptor(AST_TAG_PLUS, LAMBDA(AcceptPlus));
    InstallAcceptor(AST_TAG_MINUS, LAMBDA(AcceptMinus));
    InstallAcceptor(AST_TAG_MUL, LAMBDA(AcceptMul));
    InstallAcceptor(AST_TAG_DIV, LAMBDA(AcceptDiv));
    InstallAcceptor(AST_TAG_MODULO, LAMBDA(AcceptModulo));
    InstallAcceptor(AST_TAG_GREATER, LAMBDA(AcceptGreater));
    InstallAcceptor(AST_TAG_LESS, LAMBDA(AcceptGreaterEqual));
    InstallAcceptor(AST_TAG_GEQUAL, LAMBDA(AcceptLess));
    InstallAcceptor(AST_TAG_LEQUAL, LAMBDA(AcceptLessEqual));
    InstallAcceptor(AST_TAG_EQUAL, LAMBDA(AcceptEqual));
    InstallAcceptor(AST_TAG_NEQUAL, LAMBDA(AcceptNotEqual));
    InstallAcceptor(AST_TAG_AND, LAMBDA(AcceptAnd));
    InstallAcceptor(AST_TAG_OR, LAMBDA(AcceptOr));
    InstallAcceptor(AST_TAG_TERM, LAMBDA(AcceptTerm));
    InstallAcceptor(AST_TAG_UMINUS, LAMBDA(AcceptUnaryMinus));
    InstallAcceptor(AST_TAG_NOT, LAMBDA(AcceptNot));
    InstallAcceptor(AST_TAG_BPLUSPLUS, LAMBDA(AcceptPlusPlusBefore));
    InstallAcceptor(AST_TAG_APLUSPLUS, LAMBDA(AcceptPlusPlusAfter));
    InstallAcceptor(AST_TAG_BMINUSMINUS, LAMBDA(AcceptMinusMinusBefore));
    InstallAcceptor(AST_TAG_AMINUSMINUS, LAMBDA(AcceptMinusMinusAfter));
    InstallAcceptor(AST_TAG_PRIMARY, LAMBDA(AcceptPrimary));
    InstallAcceptor(AST_TAG_LVALUE, LAMBDA(AcceptLValue));
    InstallAcceptor(AST_TAG_ID, LAMBDA(AcceptId));
    InstallAcceptor(AST_TAG_LOCAL_ID, LAMBDA(AcceptLocal));
    InstallAcceptor(AST_TAG_DOUBLECOLON_ID, LAMBDA(AcceptDoubleColon));
    InstallAcceptor(AST_TAG_MEMBER, LAMBDA(AcceptMember));
    InstallAcceptor(AST_TAG_DOT, LAMBDA(AcceptDot));
    InstallAcceptor(AST_TAG_BRACKET, LAMBDA(AcceptBracket));
    InstallAcceptor(AST_TAG_CALL, LAMBDA(AcceptCall));
    InstallAcceptor(AST_TAG_SUFFIX, LAMBDA(AcceptCallSuffix));
    InstallAcceptor(AST_TAG_NORMAL_CALL, LAMBDA(AcceptNormalCall));
    InstallAcceptor(AST_TAG_METHOD_CALL, LAMBDA(AcceptMethodCall));
    InstallAcceptor(AST_TAG_ELIST, LAMBDA(AcceptExpressionList));
    InstallAcceptor(AST_TAG_OBJECT_DEF, LAMBDA(AcceptObjectDef));
    InstallAcceptor(AST_TAG_INDEXED, LAMBDA(AcceptIndexed));
    InstallAcceptor(AST_TAG_INDEXED_ELEM, LAMBDA(AcceptIndexedElem));
    InstallAcceptor(AST_TAG_BLOCK, LAMBDA(AcceptBlock));
    InstallAcceptor(AST_TAG_FUNCTION_DEF, LAMBDA(AcceptFunctionDef));
    InstallAcceptor(AST_TAG_CONST, LAMBDA(AcceptConst));
    InstallAcceptor(AST_TAG_NUMBER, LAMBDA(AcceptNumber));
    InstallAcceptor(AST_TAG_STRING, LAMBDA(AcceptString));
    InstallAcceptor(AST_TAG_NILL, LAMBDA(AcceptNill));
    InstallAcceptor(AST_TAG_TRUE, LAMBDA(AcceptTrue));
    InstallAcceptor(AST_TAG_FALSE, LAMBDA(AcceptFalse));
    InstallAcceptor(AST_TAG_ID_LIST, LAMBDA(AcceptIdList));
    InstallAcceptor(AST_TAG_IF, LAMBDA(AcceptIf));
    InstallAcceptor(AST_TAG_WHILE, LAMBDA(AcceptWhile));
    InstallAcceptor(AST_TAG_FOR, LAMBDA(AcceptFor));
    InstallAcceptor(AST_TAG_RETURN, LAMBDA(AcceptReturn));
    InstallAcceptor(AST_TAG_BREAK, LAMBDA(AcceptBreak));
    InstallAcceptor(AST_TAG_CONTINUE, LAMBDA(AcceptContinue));
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

void TreeHost::AcceptLValue(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitLValue(node);
}

void TreeHost::AcceptId(const Object& node){
    Accept(*node[AST_TAG_ID]->ToObject());
    visitor->VisitId(node);
}

void TreeHost::AcceptLocal(const Object& node){
    Accept(*node[AST_TAG_LOCAL_ID]->ToObject());
    visitor->VisitLocal(node);
}

void TreeHost::AcceptDoubleColon(const Object& node){
    Accept(*node[AST_TAG_DOUBLECOLON_ID]->ToObject());
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

void TreeHost::AcceptCall(const Object& node){
    Accept(*node[AST_TAG_FUNCTION]->ToObject());
    if (node.ElementExists(AST_TAG_SUFFIX))
        Accept(*node[AST_TAG_SUFFIX]->ToObject());
    else
        Accept(*node[AST_TAG_ARGUMENTS]->ToObject());
    visitor->VisitCall(node);
}

void TreeHost::AcceptCallSuffix(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitCallSuffix(node);
}

void TreeHost::AcceptNormalCall(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitNormalCall(node);
}

void TreeHost::AcceptMethodCall(const Object& node){
    Accept(*node[AST_TAG_FUNCTION]->ToObject());
    Accept(*node[AST_TAG_ARGUMENTS]->ToObject());
    visitor->VisitMethodCall(node);
}

void TreeHost::AcceptExpressionList(const Object& node){
    for (int i = 0; i < node.GetTotal() - 1; i++)
        Accept(*node[i]->ToObject());
    visitor->VisitExpressionList(node);    
}

void TreeHost::AcceptObjectDef(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitObjectDef(node);
}

void TreeHost::AcceptIndexed(const Object& node){
    for (int i = 0; i < node.GetTotal() - 1; i++)
        Accept(*node[i]->ToObject());
    visitor->VisitIndexed(node);
}

void TreeHost::AcceptIndexedElem(const Object& node){
    Accept(*node[AST_TAG_OBJECT_KEY]->ToObject());
    Accept(*node[AST_TAG_OBJECT_VALUE]->ToObject());
    visitor->VisitIndexedElem(node);
}

void TreeHost::AcceptBlock(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitBlock(node);
}

void TreeHost::AcceptFunctionDef(const Object& node){
    Accept(*node[AST_TAG_FUNCTION_ID]->ToObject());
    Accept(*node[AST_TAG_FUNCTION_FORMALS]->ToObject());
    Accept(*node[AST_TAG_STMT]->ToObject());
    visitor->VisitFunctionDef(node);
}

void TreeHost::AcceptConst(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitConst(node);
}

void TreeHost::AcceptNumber(const Object& node){
    Accept(*node[AST_TAG_VALUE]->ToObject());
    visitor->VisitNumber(node);
}

void TreeHost::AcceptString(const Object& node){
    Accept(*node[AST_TAG_VALUE]->ToObject());
    visitor->VisitString(node);
}

void TreeHost::AcceptNill(const Object& node){
    Accept(*node[AST_TAG_VALUE]->ToObject());
    visitor->VisitNill(node);
}

void TreeHost::AcceptTrue(const Object& node){
    Accept(*node[AST_TAG_VALUE]->ToObject());
    visitor->VisitTrue(node);
}

void TreeHost::AcceptFalse(const Object& node){
    Accept(*node[AST_TAG_VALUE]->ToObject());
    visitor->VisitFalse(node);
}

void TreeHost::AcceptIdList(const Object& node){
    for (int i = 0; i < node.GetTotal() - 1; i++)
        Accept(*node[i]->ToObject());
    visitor->VisitIdList(node);
}

void TreeHost::AcceptIf(const Object& node){
    Accept(*node[AST_TAG_CONDITION]->ToObject());
    Accept(*node[AST_TAG_STMT]->ToObject());
    if (node.ElementExists(AST_TAG_ELSE_STMT))
        Accept(*node[AST_TAG_ELSE_STMT]->ToObject());
    visitor->VisitIf(node);
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

void TreeHost::AcceptReturn(const Object& node){
    Accept(*node[AST_TAG_CHILD]->ToObject());
    visitor->VisitReturn(node);
}

void TreeHost::AcceptBreak(const Object& node){
    visitor->VisitBreak(node);
}

void TreeHost::AcceptContinue(const Object& node){
    visitor->VisitContinue(node);
}