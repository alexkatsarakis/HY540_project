#include "SemanticActions.h"
#include "Object.h"
#include "TreeTags.h"
#include "Value.h"

#include <cassert>

#define LINE_NUMBER_RESERVED_FIELD "$line_number"

#define SET_LINE(n) n->Set(LINE_NUMBER_RESERVED_FIELD, double(yylineno));

/*********** Helper Functions ***********/

Object *ParseEmptyNode(const char *type) {
    assert(type);

    auto node = new Object();
    node->Set(AST_TAG_TYPE_KEY, type);
    SET_LINE(node);

    assert(node->IsValid() && node->GetTotal() == 2);
    return node;
}

Object *ParseSingleChild(const char *type, Object *child) {
    assert(type);
    assert(child && child->IsValid());

    auto node = new Object();
    node->Set(AST_TAG_TYPE_KEY, type);
    node->Set(AST_TAG_CHILD, Value(child));
    SET_LINE(node);

    assert(node->IsValid() && node->GetTotal() == 3);
    return node;
}

Object *ParseTwoChildren(const char *type, const char *type1, Object *child1, const char *type2, Object *child2) {
    assert(type);
    assert(type1);
    assert(type2);
    assert(child1 && child1->IsValid());
    assert(child2 && child2->IsValid());

    auto node = new Object();
    node->Set(AST_TAG_TYPE_KEY, type);
    node->Set(type1, Value(child1));
    node->Set(type2, Value(child2));
    SET_LINE(node);

    assert(node->IsValid() && node->GetTotal() == 4);
    return node;
}

Object *ParseID(const char *type, char *value) {
    assert(type);
    assert(value);

    auto node = new Object();
    node->Set(AST_TAG_TYPE_KEY, type);
    node->Set(AST_TAG_ID, Value(std::string(value)));
    SET_LINE(node);

    /* When lexxer identifies an ID it always uses strdup */
    free(value);

    assert(node->IsValid());
    return node;
}

Object *ParseRecursion(Object *parsed, Object *current) {
    assert(parsed && parsed->IsValid());
    assert(current && current->IsValid());
    assert(parsed->GetTotal() >= 1);

    parsed->Set(parsed->GetNumericSize(), Value(current));    //TODO: Convert GetTotal to GetNumeric ?

    assert(parsed->IsValid());
    return parsed;
}

Object *ParseCompleteRecursion(const char *type, Object *current, Object *rest) {
    assert(type);
    assert(current && current->IsValid());
    assert(rest && rest->IsValid());

    Object *table = new Object();
    table->Set(AST_TAG_TYPE_KEY, type);

    table->Set(double(0), Value(current));

    for (register unsigned i = 0; i < rest->GetNumericSize(); ++i) {
        Value v = *(*rest)[double(i)];
        table->Set(double(i + 1), v);
    }
    SET_LINE(table);

    /* TODO: Should we delete rest? */
    rest->Clear();
    delete rest;

    assert(table->IsValid());
    return table;
}

Object *MergeLists(const char *type, Object *table1, Object *table2) {
    assert(type);
    assert(table1 && table1->IsValid());
    assert(table2 && table2->IsValid());

    Object *table = new Object();
    table->Set(AST_TAG_TYPE_KEY, type);
    for (register unsigned i = 0; i < table1->GetNumericSize(); ++i) {
        Value v = *(*table1)[double(i)];
        table->Set(double(i), v);
    }
    for (register unsigned i = 0; i < table2->GetNumericSize(); ++i) {
        Value v = *(*table2)[double(i)];
        table->Set(double(table1->GetNumericSize() + i), v);
    }
    assert(table->GetNumericSize() == table1->GetNumericSize() + table2->GetNumericSize());
    SET_LINE(table);

    /* TODO: Should we delete rest? */
    table1->Clear(), delete table1;
    table2->Clear(), delete table2;

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

    stmts->Set(stmts->GetNumericSize(), Value(stmt));
    SET_LINE(stmts);

    assert(stmts->IsValid());
    return stmts;
}

Object *ParseStmt(Object *stmt) {
    return ParseSingleChild(AST_TAG_STMT, stmt);
}

Object *ParseSemicolon(void) {
    auto node = new Object();
    node->Set(AST_TAG_TYPE_KEY, AST_TAG_STMT);
    SET_LINE(node);

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

Object *ParseSimpleID(char *value) {
    return ParseID(AST_TAG_ID, value);
}

Object *ParseLocalID(char *value) {
    return ParseID(AST_TAG_LOCAL_ID, value);
}

Object *ParseDoubleColonID(char *value) {
    return ParseID(AST_TAG_DOUBLECOLON_ID, value);
}

Object *ParseDollarID(char *value) {
    return ParseID(AST_TAG_DOLLAR_ID, value);
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

Object *ParseLvalueCall(Object *lvalue, Object *elist) {
    return ParseTwoChildren(AST_TAG_CALL, AST_TAG_FUNCTION, lvalue, AST_TAG_ARGUMENTS, elist);
}

Object *ParseLvalueMethodCall(Object *lvalue, Object *id, Object *arguments) {
    Object *node = ParseTwoChildren(AST_TAG_CALL, AST_TAG_FUNCTION, id, AST_TAG_ARGUMENTS, arguments);    //Line set in here
    node->Set(AST_TAG_LVALUE, lvalue);
    return node;
}

Object *ParseFuncdefCall(Object *funcdef, Object *elist) {
    return ParseTwoChildren(AST_TAG_CALL, AST_TAG_FUNCTION, funcdef, AST_TAG_ARGUMENTS, elist);
}

Object *ParseArgList(Object *arg, Object *rest) {
    return ParseCompleteRecursion(AST_TAG_ARGLIST, arg, rest);
}

Object *ParseMixedArgList(Object *positional, Object *named) {
    return MergeLists(AST_TAG_ARGLIST, positional, named);
}

Object *ParseEmptyArgList(void) {
    return ParseEmptyNode(AST_TAG_ARGLIST);
}

Object *ParseCommaNamedArgs(Object *parsed, Object *named) {
    return ParseRecursion(parsed, named);
}

Object *ParseNamedArg(Object *id, Object *expr) {
    assert(id && id->IsValid());
    assert(expr && expr->IsValid());

    Object *node = new Object();
    node->Set(AST_TAG_TYPE_KEY, AST_TAG_NAMED);
    node->Set(AST_TAG_NAMED_KEY, Value(id));
    node->Set(AST_TAG_NAMED_VALUE, Value(expr));
    SET_LINE(node);

    assert(node->IsValid());
    return node;
}

Object *ParseEmptyElist(void) {
    return ParseEmptyNode(AST_TAG_ELIST);
}

Object *ParseCommaExprs(Object *parsed, Object *expr) {
    return ParseRecursion(parsed, expr);
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

Object *ParseIndexedElem(Object *key, Object *value) {    //Delete duplicate signature in header file
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
    SET_LINE(node);

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
    SET_LINE(node);

    assert(node->IsValid());
    return node;
}

Object *ParseString(char *value) {
    assert(value);

    auto node = new Object();
    node->Set(AST_TAG_TYPE_KEY, AST_TAG_STRING);
    node->Set(AST_TAG_VALUE, Value(std::string(value)));
    SET_LINE(node);

    /* When lexxer identifies an ID it always uses strdup */
    free(value);

    assert(node->IsValid());
    return node;
}

Object *ParseNil(void) {
    auto node = new Object();
    node->Set(AST_TAG_TYPE_KEY, AST_TAG_NIL);
    node->Set(AST_TAG_VALUE, Value(NilTypeValue::Nil));
    SET_LINE(node);

    assert(node->IsValid());
    return node;
}

Object *ParseTrue(void) {
    auto node = new Object();
    node->Set(AST_TAG_TYPE_KEY, AST_TAG_TRUE);
    node->Set(AST_TAG_VALUE, Value(true));
    SET_LINE(node);

    assert(node->IsValid());
    return node;
}

Object *ParseFalse(void) {
    auto node = new Object();
    node->Set(AST_TAG_TYPE_KEY, AST_TAG_FALSE);
    node->Set(AST_TAG_VALUE, Value(false));
    SET_LINE(node);

    assert(node->IsValid());
    return node;
}

Object *ParseEmptyIdlist(void) {
    return ParseEmptyNode(AST_TAG_ID_LIST);
}

Object *ParseCommaIds(Object *rest, Object *id) {
    return ParseRecursion(rest, id);
}

Object *ParseOptionals(Object *rest, Object *assignment) {
    return ParseRecursion(rest, assignment);
}

Object *ParseIdList(Object *id, Object *rest) {
    return ParseCompleteRecursion(AST_TAG_ID_LIST, id, rest);
}

Object *ParseMixedIdList(Object *required, Object *optionals) {
    return MergeLists(AST_TAG_ID_LIST, required, optionals);
}

// ParseOptionalIdList not used in parser.y, also not in header, delete?
Object *ParseOptionalIdList(Object *assignment, Object *rest) {
    return ParseCompleteRecursion(AST_TAG_ID_LIST, assignment, rest);
}

Object *ParseFormal(char *value) {
    return ParseID(AST_TAG_FORMAL, value);
}

Object *ParseIfStmt(Object *cond, Object *stmt, Object *elseStmt) {
    assert(cond && cond->IsValid());
    assert(stmt && stmt->IsValid());

    auto node = new Object();
    node->Set(AST_TAG_TYPE_KEY, AST_TAG_IF);
    node->Set(AST_TAG_CONDITION, Value(cond));
    node->Set(AST_TAG_STMT, Value(stmt));

    if (elseStmt) node->Set(AST_TAG_ELSE_STMT, Value(elseStmt));

    SET_LINE(node);

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
    SET_LINE(node);

    assert(node->IsValid());
    return node;
}

Object *ParseReturnStmt(Object *expr) {
    auto node = new Object();
    node->Set(AST_TAG_TYPE_KEY, AST_TAG_RETURN);

    if (expr) node->Set(AST_TAG_CHILD, Value(expr));

    SET_LINE(node);

    assert(node->IsValid());
    return node;
}

Object *ParseBreakStmt(void) {
    return ParseEmptyNode(AST_TAG_BREAK);
}

Object *ParseContinueStmt(void) {
    return ParseEmptyNode(AST_TAG_CONTINUE);
}
