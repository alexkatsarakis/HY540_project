#ifndef _SEMANTIC_ACTIONS_H_
#define _SEMANTIC_ACTIONS_H_

#include "Object.h"

Object *ParseProgram(Object *stmts);

Object *ParseEmptyStmts(void);

Object *ParseStmts(Object *stmts, Object *stmt);

Object *ParseStmt(Object *stmt);

Object *ParseSemicolon(void);

Object *ParseExpr(Object *expr);

Object *ParseAssign(Object *lvalue, Object *rvalue);

Object *ParsePlus(Object *expr1, Object *expr2);

Object *ParseMinus(Object *expr1, Object *expr2);

Object *ParseMul(Object *expr1, Object *expr2);

Object *ParseDiv(Object *expr1, Object *expr2);

Object *ParseModulo(Object *expr1, Object *expr2);

Object *ParseGreater(Object *expr1, Object *expr2);

Object *ParseLess(Object *expr1, Object *expr2);

Object *ParseGreaterEqual(Object *expr1, Object *expr2);

Object *ParseLessEqual(Object *expr1, Object *expr2);

Object *ParseEqual(Object *expr1, Object *expr2);

Object *ParseNotEqual(Object *expr1, Object *expr2);

Object *ParseAnd(Object *expr1, Object *expr2);

Object *ParseOr(Object *expr1, Object *expr2);

Object *ParseTerm(Object *term);

Object *ParseUminus(Object *expr);

Object *ParseNot(Object *expr);

Object *ParsePlusPlusLvalue(Object *expr);

Object *ParseLvaluePlusPlus(Object *expr);

Object *ParseMinusMinusLvalue(Object *expr);

Object *ParseLvalueMinusMinus(Object *expr);

Object *ParsePrimary(Object *primary);

Object *ParseLvalue(Object *lvalue);

Object *ParseSimpleID(char *value);

Object *ParseLocalID(char *value);

Object *ParseDoubleColonID(char *value);

Object *ParseDollarID(char *value);

Object *ParseMember(Object *member);

Object *ParseMemberDot(Object *lvalue, Object *id);

Object *ParseMemberBracket(Object *lvalue, Object *id);

Object *ParseCallCall(Object *call, Object *elist);

Object *ParseLvalueCall(Object *lvalue, Object *suffix);

Object *ParseFuncdefCall(Object *funcdef, Object *elist);

Object *ParseCallSuffix(Object *call);

Object *ParseNormCall(Object *elist);

Object *ParseMethodCall(Object *id, Object *elist);

Object *ParseEmptyElist(void);

Object *ParseCommaExprs(Object *parsed, Object *expr);

Object *ParseElist(Object *expr, Object *rest);

Object *ParseObjectDef(Object *child);

Object *ParseEmptyIndexed(void);

Object *ParseCommaIndexed(Object *rest, Object *elem);

Object *ParseIndexed(Object *indexedelem, Object *rest);

Object *ParseIndexedElem(Object *key, Object *value);

Object *ParseIndexedElem(Object *key, Object *value);    //redefinition of top, why g++ allowed this?

Object *ParseBlock(Object *stmts);

Object *ParseFuncDef(Object *id, Object *idlist, Object *block);

Object *ParseConst(Object *child);

Object *ParseNumber(double value);

Object *ParseString(char *value);

Object *ParseNil(void);

Object *ParseTrue(void);

Object *ParseFalse(void);

Object *ParseEmptyIdlist(void);

Object *ParseCommaIds(Object *rest, Object *id);

Object *ParseIdList(Object *id, Object *rest);

Object *ParseIfStmt(Object *cond, Object *stmt, Object *elseStmt = nullptr);

Object *ParseWhileStmt(Object *cond, Object *stmt);

Object *ParseForStmt(Object *elist1, Object *expr, Object *elist2, Object *stmt);

Object *ParseReturnStmt(Object *expr = nullptr);

Object *ParseBreakStmt(void);

Object *ParseContinueStmt(void);

#endif
