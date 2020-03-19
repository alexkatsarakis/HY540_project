#include "SemanticActions.h"
#include "Object.h"
#include "TreeTags.h"
#include "Value.h"

#include <cassert>

/*********** Helper Functions ***********/

Object *ParseEmptyNode(const char *type) {
	assert(type);

	auto node = new Object();
	node->Set(AST_TAG_TYPE_KEY, type);

	assert(node->IsValid() && node->GetTotal() == 1);
	return node;
}

Object *ParseSingleChild(const char *type, Object *child) {
	assert(type);
	assert(child && child->IsValid());

	auto node = new Object();
	node->Set(AST_TAG_TYPE_KEY, type);
	node->Set(AST_TAG_CHILD, Value(child));

	assert(node->IsValid() && node->GetTotal() == 2);
	return node;
}

Object *ParseTwoChildren(const char *type, const char *type1, Object *child1, const char *type2, Object *child2) {
	assert(child1 && child1->IsValid());
	assert(child2 && child2->IsValid());

	auto node = new Object();
	node->Set(AST_TAG_TYPE_KEY, type);
	node->Set(type1, Value(child1));
	node->Set(type2, Value(child2));

	assert(node->IsValid() && node->GetTotal() == 3);
	return node;
}

Object *ParseID(const char *type, const char *value) {
	assert(type);
	assert(value);

	auto node = new Object();
	node->Set(AST_TAG_TYPE_KEY, type);
	node->Set(AST_TAG_ID, Value(std::string(value)));

    /* TODO: Values pointers should be freed. Change definitions to:
     * (const char * type, char * value) */
    free((char *) value);

	assert(node->IsValid());
	return node;
}

Object *ParseRecursion(Object *rest, Object *current) {
	assert(rest && rest->IsValid());
	assert(current && current->IsValid());

	rest->Set(rest->GetTotal() - 1, Value(current));

	assert(rest->IsValid());
	return rest;
}

Object *ParseCompleteRecursion(const char *type, Object *current, Object *rest) {
	assert(current && current->IsValid());
	assert(rest && rest->IsValid());

	auto table = new Object();
	table->Set(AST_TAG_TYPE_KEY, type);

	table->Set(double(0), Value(current));

	for (register unsigned i = 0; i < rest->GetNumericSize(); ++i) {
		Value v = *(*rest)[double(i)];
		table->Set(double(i + 1), v);
	}

	assert(table->IsValid());
	return table;
}

/*********** Parse Functions ***********/

Object *ParseProgram(Object *stmts) {
	return ParseSingleChild(AST_TAG_PROGRAM, stmts);
}

Object *ParseEmptyStmts(void) {
	return ParseEmptyNode(AST_TAG_STMTS);
}

Object *ParseStmts(Object *stmts, Object *stmt) {
	assert(stmts && stmts->IsValid());
	assert(stmt && stmt->IsValid());

	stmts->Set(stmts->GetTotal() - 1, Value(stmt));

	assert(stmts->IsValid());
	return stmts;
}

Object *ParseStmt(Object *stmt) {
	return ParseSingleChild(AST_TAG_STMT, stmt);
}

Object *ParseSemicolon(void) {
	auto node = new Object();
	node->Set(AST_TAG_TYPE_KEY, AST_TAG_STMT);

	assert(node->IsValid());
	return node;
}

Object *ParseExpr(Object *expr) {
	return ParseSingleChild(AST_TAG_EXPR, expr);
}

Object *ParseAssign(Object *lvalue, Object *rvalue) {
	return ParseTwoChildren(AST_TAG_ASSIGN, AST_TAG_LVALUE, lvalue, AST_TAG_RVALUE, rvalue);
}

Object *ParsePlus(Object *expr1, Object *expr2) {
	return ParseTwoChildren(AST_TAG_PLUS, AST_TAG_FIRST_EXPR, expr1, AST_TAG_SECOND_EXPR, expr2);
}

Object *ParseMinus(Object *expr1, Object *expr2) {
	return ParseTwoChildren(AST_TAG_MINUS, AST_TAG_FIRST_EXPR, expr1, AST_TAG_SECOND_EXPR, expr2);
}

Object *ParseMul(Object *expr1, Object *expr2) {
	return ParseTwoChildren(AST_TAG_MUL, AST_TAG_FIRST_EXPR, expr1, AST_TAG_SECOND_EXPR, expr2);
}

Object *ParseDiv(Object *expr1, Object *expr2) {
	return ParseTwoChildren(AST_TAG_DIV, AST_TAG_FIRST_EXPR, expr1, AST_TAG_SECOND_EXPR, expr2);
}

Object *ParseModulo(Object *expr1, Object *expr2) {
	return ParseTwoChildren(AST_TAG_MODULO, AST_TAG_FIRST_EXPR, expr1, AST_TAG_SECOND_EXPR, expr2);
}

Object *ParseGreater(Object *expr1, Object *expr2) {
	return ParseTwoChildren(AST_TAG_GREATER, AST_TAG_FIRST_EXPR, expr1, AST_TAG_SECOND_EXPR, expr2);
}

Object *ParseLess(Object *expr1, Object *expr2) {
	return ParseTwoChildren(AST_TAG_LESS, AST_TAG_FIRST_EXPR, expr1, AST_TAG_SECOND_EXPR, expr2);
}

Object *ParseGreaterEqual(Object *expr1, Object *expr2) {
	return ParseTwoChildren(AST_TAG_GEQUAL, AST_TAG_FIRST_EXPR, expr1, AST_TAG_SECOND_EXPR, expr2);
}

Object *ParseLessEqual(Object *expr1, Object *expr2) {
	return ParseTwoChildren(AST_TAG_LEQUAL, AST_TAG_FIRST_EXPR, expr1, AST_TAG_SECOND_EXPR, expr2);
}

Object *ParseEqual(Object *expr1, Object *expr2) {
	return ParseTwoChildren(AST_TAG_EQUAL, AST_TAG_FIRST_EXPR, expr1, AST_TAG_SECOND_EXPR, expr2);
}

Object *ParseNotEqual(Object *expr1, Object *expr2) {
	return ParseTwoChildren(AST_TAG_NEQUAL, AST_TAG_FIRST_EXPR, expr1, AST_TAG_SECOND_EXPR, expr2);
}

Object *ParseAnd(Object *expr1, Object *expr2) {
	return ParseTwoChildren(AST_TAG_AND, AST_TAG_FIRST_EXPR, expr1, AST_TAG_SECOND_EXPR, expr2);
}

Object *ParseOr(Object *expr1, Object *expr2) {
	return ParseTwoChildren(AST_TAG_OR, AST_TAG_FIRST_EXPR, expr1, AST_TAG_SECOND_EXPR, expr2);
}

Object *ParseTerm(Object *term) {
	return ParseSingleChild(AST_TAG_TERM, term);
}

Object *ParseUminus(Object *expr) {
	return ParseSingleChild(AST_TAG_UMINUS, expr);
}

Object *ParseNot(Object *expr) {
	return ParseSingleChild(AST_TAG_NOT, expr);
}

Object *ParsePlusPlusLvalue(Object *expr) {
	return ParseSingleChild(AST_TAG_BPLUSPLUS, expr);
}

Object *ParseLvaluePlusPlus(Object *expr) {
	return ParseSingleChild(AST_TAG_APLUSPLUS, expr);
}

Object *ParseMinusMinusLvalue(Object *expr) {
	return ParseSingleChild(AST_TAG_BMINUSMINUS, expr);
}

Object *ParseLvalueMinusMinus(Object *expr) {
	return ParseSingleChild(AST_TAG_AMINUSMINUS, expr);
}

Object *ParsePrimary(Object *primary) {
	return ParseSingleChild(AST_TAG_PRIMARY, primary);
}

Object *ParseLvalue(Object *lvalue) {
	return ParseSingleChild(AST_TAG_LVALUE, lvalue);
}

Object *ParseSimpleID(const char *value) {
	return ParseID(AST_TAG_ID, value);
}

Object *ParseLocalID(const char *value) {
	return ParseID(AST_TAG_LOCAL_ID, value);
}

Object *ParseDoubleColonID(const char *value) {
	return ParseID(AST_TAG_DOUBLECOLON_ID, value);
}

Object *ParseMember(Object *member) {
	return ParseSingleChild(AST_TAG_MEMBER, member);
}

Object *ParseMemberDot(Object *lvalue, Object *id) {
	return ParseTwoChildren(AST_TAG_DOT, AST_TAG_LVALUE, lvalue, AST_TAG_ID, id);
}

Object *ParseMemberBracket(Object *lvalue, Object *expr) {
	return ParseTwoChildren(AST_TAG_BRACKET, AST_TAG_LVALUE, lvalue, AST_TAG_EXPR, expr);
}

Object *ParseCallCall(Object *call, Object *elist) {
	return ParseTwoChildren(AST_TAG_CALL, AST_TAG_FUNCTION, call, AST_TAG_ARGUMENTS, elist);
}

Object *ParseLvalueCall(Object *lvalue, Object *suffix) {
	return ParseTwoChildren(AST_TAG_CALL, AST_TAG_FUNCTION, lvalue, AST_TAG_SUFFIX, suffix);
}

Object *ParseFuncdefCall(Object *funcdef, Object *elist) {
	return ParseTwoChildren(AST_TAG_CALL, AST_TAG_FUNCTION, funcdef, AST_TAG_ARGUMENTS, elist);
}

Object *ParseCallSuffix(Object *call) {
	return ParseSingleChild(AST_TAG_CALL_SUFFIX, call);
}

Object *ParseNormCall(Object *elist) {
	return ParseSingleChild(AST_TAG_NORMAL_CALL, elist);
}

Object *ParseMethodCall(Object *id, Object *elist) {
	return ParseTwoChildren(AST_TAG_METHOD_CALL, AST_TAG_FUNCTION, id, AST_TAG_ARGUMENTS, elist);
}

Object *ParseEmptyElist(void) {
	return ParseEmptyNode(AST_TAG_ELIST);
}

Object *ParseCommaExprs(Object *rest, Object *expr) {
	return ParseRecursion(rest, expr);
}

Object *ParseElist(Object *expr, Object *rest) {
	return ParseCompleteRecursion(AST_TAG_ELIST, expr, rest);
}

Object *ParseObjectDef(Object *child) {
	return ParseSingleChild(AST_TAG_OBJECT_DEF, child);
}

Object *ParseEmptyIndexed(void) {
	return ParseEmptyNode(AST_TAG_INDEXED);
}

Object *ParseCommaIndexed(Object *rest, Object *elem) {
	return ParseRecursion(rest, elem);
}

Object *ParseIndexed(Object *indexedelem, Object *rest) {
	return ParseCompleteRecursion(AST_TAG_INDEXED, indexedelem, rest);
}

Object *ParseIndexedElem(Object *key, Object *value) {
	return ParseTwoChildren(AST_TAG_INDEXED_ELEM, AST_TAG_OBJECT_KEY, key, AST_TAG_OBJECT_VALUE, value);
}

Object *ParseBlock(Object *stmts) {
	return ParseSingleChild(AST_TAG_BLOCK, stmts);
}

Object *ParseFuncDef(Object *id, Object *idlist, Object *block) {
	assert(id && id->IsValid());
	assert(idlist && idlist->IsValid());
	assert(block && block->IsValid());

	auto node = new Object();
	node->Set(AST_TAG_TYPE_KEY, AST_TAG_FUNCTION_DEF);
	node->Set(AST_TAG_FUNCTION_ID, Value(id));
	node->Set(AST_TAG_FUNCTION_FORMALS, Value(idlist));
	node->Set(AST_TAG_STMT, Value(block));

	assert(node->IsValid());
	return node;
}

Object *ParseConst(Object *child) {
	return ParseSingleChild(AST_TAG_CONST, child);
}

Object *ParseNumber(double value) {
	auto node = new Object();
	node->Set(AST_TAG_TYPE_KEY, AST_TAG_NUMBER);
	node->Set(AST_TAG_VALUE, Value(value));

	assert(node->IsValid());
	return node;
}

Object *ParseString(char *value) {
	assert(value);

	auto node = new Object();
	node->Set(AST_TAG_TYPE_KEY, AST_TAG_STRING);
	node->Set(AST_TAG_VALUE, Value(std::string(value)));

	assert(node->IsValid());
	return node;
}

Object *ParseNill(void) {
	auto node = new Object();
	node->Set(AST_TAG_TYPE_KEY, AST_TAG_NILL);
	node->Set(AST_TAG_VALUE, Value(NilTypeValue::Nil));

	assert(node->IsValid());
	return node;
}

Object *ParseTrue(void) {
	auto node = new Object();
	node->Set(AST_TAG_TYPE_KEY, AST_TAG_TRUE);
	node->Set(AST_TAG_VALUE, Value(true));

	assert(node->IsValid());
	return node;
}

Object *ParseFalse(void) {
	auto node = new Object();
	node->Set(AST_TAG_TYPE_KEY, AST_TAG_FALSE);
	node->Set(AST_TAG_VALUE, Value(false));

	assert(node->IsValid());
	return node;
}

Object *ParseEmptyIdlist(void) {
	return ParseEmptyNode(AST_TAG_ID_LIST);
}

Object *ParseCommaIds(Object *rest, Object *id) {
	return ParseRecursion(rest, id);
}

Object *ParseIdList(Object *id, Object *rest) {
	return ParseCompleteRecursion(AST_TAG_ID_LIST, id, rest);
}

Object *ParseIfStmt(Object *cond, Object *stmt, Object *elseStmt) {
	assert(cond && cond->IsValid());
	assert(stmt && stmt->IsValid());

	auto node = new Object();
	node->Set(AST_TAG_TYPE_KEY, AST_TAG_IF);
	node->Set(AST_TAG_CONDITION, Value(cond));
	node->Set(AST_TAG_STMT, Value(stmt));

	if (elseStmt) node->Set(AST_TAG_ELSE_STMT, Value(elseStmt));

	assert(node->IsValid());
	return node;
}

Object *ParseWhileStmt(Object *cond, Object *stmt) {
	return ParseTwoChildren(AST_TAG_WHILE, AST_TAG_CONDITION, cond, AST_TAG_STMT, stmt);
}

Object *ParseForStmt(Object *elist1, Object *expr, Object *elist2, Object *stmt) {
	assert(elist1 && elist1->IsValid());
	assert(expr && expr->IsValid());
	assert(elist2 && elist2->IsValid());
	assert(stmt && stmt->IsValid());

	auto node = new Object();
	node->Set(AST_TAG_TYPE_KEY, AST_TAG_FOR);
	node->Set(AST_TAG_FOR_PRE_ELIST, Value(elist1));
	node->Set(AST_TAG_CONDITION, Value(expr));
	node->Set(AST_TAG_FOR_POST_ELIST, Value(elist2));
	node->Set(AST_TAG_STMT, Value(stmt));

	assert(node->IsValid());
	return node;
}

// Object * ParseReturnStmt(Object * expr = nullptr) {
Object *ParseReturnStmt(Object *expr) {
	auto node = new Object();
	node->Set(AST_TAG_TYPE_KEY, AST_TAG_RETURN);

	if (expr) node->Set(AST_TAG_CHILD, Value(expr));

	assert(node->IsValid());
	return node;
}

Object *ParseBreakStmt(void) {
	return ParseEmptyNode(AST_TAG_BREAK);
}

Object *ParseContinueStmt(void) {
	return ParseEmptyNode(AST_TAG_CONTINUE);
}
