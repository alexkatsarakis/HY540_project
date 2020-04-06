#include "UnparseVisitor.h"

#include "Object.h"
#include "TreeTags.h"

#include <fstream>

#define UNPARSE_VALUE "$$UnparseValue"
using namespace std;

string formatEscChars(const string &str) {
    string out;
    for (const auto &c : str) {
        if (c == '\n')
            out += "\\n";
        else if (c == '\t')
            out += "\\t";
        else if (c == '\"')
            out += "\\\"";
        else if (c == '\\')
            out += "\\\\";
        else
            out += c;
    }
    return out;
}

void UnparseVisitor::WriteFile(const std::string &program) {
    ofstream f(fileName.c_str(), ios_base::out);
    f << program;
    f.close();
}
const std::string UnparseVisitor::GetUnparsed(const Value *val) const {
    return val->ToObject_NoConst()->GetAndRemove(UNPARSE_VALUE).ToString();
}

const std::string UnparseVisitor::UnparseProgram(const std::string &stmts) {
    WriteFile(stmts);
    return stmts;
}
const std::string UnparseVisitor::UnparseStatements(const std::vector<std::string> &statements) {
    string statementListStr;
    for (auto statement : statements)
        statementListStr.append(statement);
    return statementListStr;
}
const std::string UnparseVisitor::UnparseStatement(const std::string &stmt) {
    return string(stmt + ";\n");
}
const std::string UnparseVisitor::UnparseExpression(const std::string &expr) {
    return expr;
}
const std::string UnparseVisitor::UnparseAssign(const std::string &lvalue, const std::string &rvalue) {
    return string(lvalue + " = " + rvalue);
}
const std::string UnparseVisitor::UnparsePlus(const std::string &expr1, const std::string &expr2) {
    return string(expr1 + " + " + expr2);
}
const std::string UnparseVisitor::UnparseMinus(const std::string &expr1, const std::string &expr2) {
    return string(expr1 + " - " + expr2);
}
const std::string UnparseVisitor::UnparseMul(const std::string &expr1, const std::string &expr2) {
    return string(expr1 + " * " + expr2);
}
const std::string UnparseVisitor::UnparseDiv(const std::string &expr1, const std::string &expr2) {
    return string(expr1 + " / " + expr2);
}
const std::string UnparseVisitor::UnparseModulo(const std::string &expr1, const std::string &expr2) {
    return string(expr1 + " % " + expr2);
}
const std::string UnparseVisitor::UnparseGreater(const std::string &expr1, const std::string &expr2) {
    return string(expr1 + " > " + expr2);
}
const std::string UnparseVisitor::UnparseLess(const std::string &expr1, const std::string &expr2) {
    return string(expr1 + " < " + expr2);
}
const std::string UnparseVisitor::UnparseGreaterEqual(const std::string &expr1, const std::string &expr2) {
    return string(expr1 + " >= " + expr2);
}
const std::string UnparseVisitor::UnparseLessEqual(const std::string &expr1, const std::string &expr2) {
    return string(expr1 + " <= " + expr2);
}
const std::string UnparseVisitor::UnparseEqual(const std::string &expr1, const std::string &expr2) {
    return string(expr1 + " == " + expr2);
}
const std::string UnparseVisitor::UnparseNotEqual(const std::string &expr1, const std::string &expr2) {
    return string(expr1 + " != " + expr2);
}
const std::string UnparseVisitor::UnparseAnd(const std::string &expr1, const std::string &expr2) {
    return string(expr1 + " and " + expr2);
}
const std::string UnparseVisitor::UnparseOr(const std::string &expr1, const std::string &expr2) {
    return string(expr1 + " or " + expr2);
}
const std::string UnparseVisitor::UnparseTerm(const std::string &expr) {
    return expr;
}
const std::string UnparseVisitor::UnparseTermPARENTHESIS(const std::string &expr) {
    return string("(" + expr + ")");
}
const std::string UnparseVisitor::UnparseUnaryMinus(const std::string &expr) {
    return string("-" + expr);
}
const std::string UnparseVisitor::UnparseNot(const std::string &expr) {
    return string("not " + expr);
}
const std::string UnparseVisitor::UnparsePlusPlusBefore(const std::string &expr) {
    return string("++" + expr);
}
const std::string UnparseVisitor::UnparsePlusPlusAfter(const std::string &expr) {
    return string(expr + "++");
}
const std::string UnparseVisitor::UnparseMinusMinusBefore(const std::string &expr) {
    return string("--" + expr);
}
const std::string UnparseVisitor::UnparseMinusMinusAfter(const std::string &expr) {
    return string(expr + "--");
}
const std::string UnparseVisitor::UnparsePrimary(const std::string &primary) {
    return primary;
}
const std::string UnparseVisitor::UnparsePrimaryPARENTHESIS(const std::string &funcdef) {
    return string("(" + funcdef + ")");
}
const std::string UnparseVisitor::UnparseLValue(const std::string &lvalue) {
    return lvalue;
}
const std::string UnparseVisitor::UnparseId(const std::string &value) {
    return value;
}
const std::string UnparseVisitor::UnparseLocal(const std::string &value) {
    return string("local " + value);
}
const std::string UnparseVisitor::UnparseDoubleColon(const std::string &value) {
    return string("::" + value);
}
const std::string UnparseVisitor::UnparseDollar(const std::string &value) {
    return string("$" + value);
}
const std::string UnparseVisitor::UnparseMember(const std::string &member) {
    return member;
}
const std::string UnparseVisitor::UnparseDot(const std::string &lvalue, const std::string &id) {
    return string(lvalue + "." + id);
}
const std::string UnparseVisitor::UnparseBracket(const std::string &lvalue, const std::string &id) {
    return string(lvalue + "[" + id + "]");
}
const std::string UnparseVisitor::UnparseCallPARENTHESIS(const std::string &call, const std::string &elist) {
    return string(call + "(" + elist + ")");
}
const std::string UnparseVisitor::UnparseCall(const std::string &lvalue, const std::string &callsuffix) {
    return string(lvalue + callsuffix);
}
const std::string UnparseVisitor::UnparseCallPARENTHESISTWIN(const std::string &funcdef, const std::string &elist) {
    return string("(" + funcdef + ")" + "(" + elist + ")");
}
const std::string UnparseVisitor::UnparseCallSuffix(const std::string &call) {
    return call;
}
const std::string UnparseVisitor::UnparseNormalCall(const std::string &elist) {
    return string("(" + elist + ")");
}
const std::string UnparseVisitor::UnparseMethodCall(const std::string &id, const std::string &elist) {
    return string(".." + id + "(" + elist + ")");
}
const std::string UnparseVisitor::UnparseExpressionList(const std::vector<std::string> &expressions) {
    string expressionListStr;
    for (auto expression : expressions)
        expressionListStr.append(expression).append(", ");
    expressionListStr.erase(expressionListStr.size() - 2);
    return expressionListStr;
}
const std::string UnparseVisitor::UnparseObjectDef(const std::string &child) {
    return string("[" + child + "]");
}
const std::string UnparseVisitor::UnparseIndexed(const std::vector<std::string> &indexedElements) {
    string indexedListStr;
    for (auto indexedElement : indexedElements)
        indexedListStr.append(indexedElement).append(", ");
    indexedListStr.erase(indexedListStr.size() - 2);
    return indexedListStr;
}
const std::string UnparseVisitor::UnparseIndexedElem(const std::string &key, const std::string &value) {
    return string("{" + key + ":" + value + "}");
}
const std::string UnparseVisitor::UnparseBlock(const std::string &stmts) {
    return string("{\n" + stmts + "}");
}
const std::string UnparseVisitor::UnparseFunctionDef(const std::string &id, const std::string &idlist, const std::string &block) {
    return string("function " + id + "(" + idlist + ") " + block);
}
const std::string UnparseVisitor::UnparseConst(const std::string &child) {
    return child;
}
const std::string UnparseVisitor::UnparseNumber(const std::string &value) {
    return value;
}
const std::string UnparseVisitor::UnparseString(const std::string &value) {
    string valStr = formatEscChars(value);
    return string("\"" + valStr + "\"");
}
const std::string UnparseVisitor::UnparseNill(const std::string &value) {
    return value;
}
const std::string UnparseVisitor::UnparseTrue(const std::string &value) {
    return value;
}
const std::string UnparseVisitor::UnparseFalse(const std::string &value) {
    return value;
}
const std::string UnparseVisitor::UnparseIdList(const std::vector<std::string> &ids) {
    string idListStr;
    for (auto id : ids)
        idListStr.append(id).append(", ");
    idListStr.erase(idListStr.size() - 2);
    return idListStr;
}
const std::string UnparseVisitor::UnparseIf(const std::string &cond, const std::string &stmt, const std::string &elseStmt) {
    string elseStr = elseStmt.empty() ? "" : "else " + elseStmt;
    return string("if (" + cond + ") " + stmt + elseStr);
}
const std::string UnparseVisitor::UnparseWhile(const std::string &expr, const std::string &stmt) {
    return string("while (" + expr + ") " + stmt);
}
const std::string UnparseVisitor::UnparseFor(const std::string &elist1, const std::string &expr, const std::string &elist2, const std::string &stmt) {
    return string("for (" + elist1 + "; " + expr + "; " + elist2 + ") " + stmt);
}
const std::string UnparseVisitor::UnparseReturn(const std::string &expr) {
    return string("return" + expr + ";");
}
const std::string UnparseVisitor::UnparseBreak(void) {
    return string("break;");
}
const std::string UnparseVisitor::UnparseContinue(void) {
    return string("continue;");
}

UnparseVisitor::UnparseVisitor(const std::string &_fileName) : fileName(_fileName) {}
UnparseVisitor::~UnparseVisitor() {}
TreeVisitor *UnparseVisitor::Clone(void) const {
    UnparseVisitor *clone = new UnparseVisitor(this->fileName);
    return clone;
}

void UnparseVisitor::VisitProgram(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseProgram(GetUnparsed(node[AST_TAG_CHILD])));
}
void UnparseVisitor::VisitStatements(const Object &node) {
    vector<string> statements;
    for (unsigned i = 0; i < node.GetNumericSize(); ++i)
        statements.push_back(GetUnparsed(node[i]));
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseStatements(statements));
}
void UnparseVisitor::VisitStatement(const Object &node) {
    if (!node.ElementExists(AST_TAG_CHILD))
        const_cast<Object &>(node).Set(
            UNPARSE_VALUE,
            UnparseStatement(""));
    else
        const_cast<Object &>(node).Set(
            UNPARSE_VALUE,
            UnparseStatement(GetUnparsed(node[AST_TAG_CHILD])));
}
void UnparseVisitor::VisitExpression(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseExpression(GetUnparsed(node[AST_TAG_CHILD])));
}
void UnparseVisitor::VisitAssign(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseAssign(GetUnparsed(node[AST_TAG_LVALUE]),
                      GetUnparsed(node[AST_TAG_RVALUE])));
}
void UnparseVisitor::VisitPlus(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparsePlus(GetUnparsed(node[AST_TAG_FIRST_EXPR]),
                    GetUnparsed(node[AST_TAG_SECOND_EXPR])));
}
void UnparseVisitor::VisitMinus(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseMinus(GetUnparsed(node[AST_TAG_FIRST_EXPR]),
                     GetUnparsed(node[AST_TAG_SECOND_EXPR])));
}
void UnparseVisitor::VisitMul(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseMul(GetUnparsed(node[AST_TAG_FIRST_EXPR]),
                   GetUnparsed(node[AST_TAG_SECOND_EXPR])));
}
void UnparseVisitor::VisitDiv(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseDiv(GetUnparsed(node[AST_TAG_FIRST_EXPR]),
                   GetUnparsed(node[AST_TAG_SECOND_EXPR])));
}
void UnparseVisitor::VisitModulo(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseModulo(GetUnparsed(node[AST_TAG_FIRST_EXPR]),
                      GetUnparsed(node[AST_TAG_SECOND_EXPR])));
}
void UnparseVisitor::VisitGreater(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseGreater(GetUnparsed(node[AST_TAG_FIRST_EXPR]),
                       GetUnparsed(node[AST_TAG_SECOND_EXPR])));
}
void UnparseVisitor::VisitLess(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseLess(GetUnparsed(node[AST_TAG_FIRST_EXPR]),
                    GetUnparsed(node[AST_TAG_SECOND_EXPR])));
}
void UnparseVisitor::VisitGreaterEqual(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseGreaterEqual(GetUnparsed(node[AST_TAG_FIRST_EXPR]),
                            GetUnparsed(node[AST_TAG_SECOND_EXPR])));
}
void UnparseVisitor::VisitLessEqual(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseLessEqual(GetUnparsed(node[AST_TAG_FIRST_EXPR]),
                         GetUnparsed(node[AST_TAG_SECOND_EXPR])));
}
void UnparseVisitor::VisitEqual(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseEqual(GetUnparsed(node[AST_TAG_FIRST_EXPR]),
                     GetUnparsed(node[AST_TAG_SECOND_EXPR])));
}
void UnparseVisitor::VisitNotEqual(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseNotEqual(GetUnparsed(node[AST_TAG_FIRST_EXPR]),
                        GetUnparsed(node[AST_TAG_SECOND_EXPR])));
}
void UnparseVisitor::VisitAnd(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseAnd(GetUnparsed(node[AST_TAG_FIRST_EXPR]),
                   GetUnparsed(node[AST_TAG_SECOND_EXPR])));
}
void UnparseVisitor::VisitOr(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseOr(GetUnparsed(node[AST_TAG_FIRST_EXPR]),
                  GetUnparsed(node[AST_TAG_SECOND_EXPR])));
}
void UnparseVisitor::VisitTerm(const Object &node) {
    auto child = *(node[AST_TAG_CHILD]->ToObject());
    if (child[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_EXPR)
        const_cast<Object &>(node).Set(
            UNPARSE_VALUE,
            UnparseTermPARENTHESIS(GetUnparsed(node[AST_TAG_CHILD])));
    else
        const_cast<Object &>(node).Set(
            UNPARSE_VALUE,
            UnparseTerm(GetUnparsed(node[AST_TAG_CHILD])));
}
void UnparseVisitor::VisitUnaryMinus(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseUnaryMinus(GetUnparsed(node[AST_TAG_CHILD])));
}
void UnparseVisitor::VisitNot(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseNot(GetUnparsed(node[AST_TAG_CHILD])));
}
void UnparseVisitor::VisitPlusPlusBefore(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparsePlusPlusBefore(GetUnparsed(node[AST_TAG_CHILD])));
}
void UnparseVisitor::VisitPlusPlusAfter(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparsePlusPlusAfter(GetUnparsed(node[AST_TAG_CHILD])));
}
void UnparseVisitor::VisitMinusMinusBefore(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseMinusMinusBefore(GetUnparsed(node[AST_TAG_CHILD])));
}
void UnparseVisitor::VisitMinusMinusAfter(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseMinusMinusAfter(GetUnparsed(node[AST_TAG_CHILD])));
}
void UnparseVisitor::VisitPrimary(const Object &node) {
    auto child = *(node[AST_TAG_CHILD]->ToObject());
    if (child[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_FUNCTION_DEF)
        const_cast<Object &>(node).Set(
            UNPARSE_VALUE,
            UnparsePrimaryPARENTHESIS(GetUnparsed(node[AST_TAG_CHILD])));
    else
        const_cast<Object &>(node).Set(
            UNPARSE_VALUE,
            UnparsePrimary(GetUnparsed(node[AST_TAG_CHILD])));
}
void UnparseVisitor::VisitLValue(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseLValue(GetUnparsed(node[AST_TAG_CHILD])));
}
void UnparseVisitor::VisitId(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseId(GetUnparsed(node[AST_TAG_ID])));
}
void UnparseVisitor::VisitLocal(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseLocal(GetUnparsed(node[AST_TAG_ID])));
}
void UnparseVisitor::VisitDoubleColon(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseDoubleColon(GetUnparsed(node[AST_TAG_ID])));
}
void UnparseVisitor::VisitDollar(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseDollar(GetUnparsed(node[AST_TAG_ID])));
}
void UnparseVisitor::VisitMember(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseMember(GetUnparsed(node[AST_TAG_CHILD])));
}
void UnparseVisitor::VisitDot(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseDot(GetUnparsed(node[AST_TAG_LVALUE]),
                   GetUnparsed(node[AST_TAG_ID])));
}
void UnparseVisitor::VisitBracket(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseBracket(GetUnparsed(node[AST_TAG_LVALUE]),
                       GetUnparsed(node[AST_TAG_EXPR])));
}
void UnparseVisitor::VisitCall(const Object &node) {
    const Object value = *(node[AST_TAG_FUNCTION]->ToObject());
    const string childType = value[AST_TAG_TYPE_KEY]->ToString();
    if (childType == AST_TAG_FUNCTION_DEF)
        const_cast<Object &>(node).Set(
            UNPARSE_VALUE,
            UnparseCallPARENTHESISTWIN(GetUnparsed(node[AST_TAG_FUNCTION]),
                                       GetUnparsed(node[AST_TAG_ARGUMENTS])));
    else if (childType == AST_TAG_CALL)
        const_cast<Object &>(node).Set(
            UNPARSE_VALUE,
            UnparseCallPARENTHESIS(GetUnparsed(node[AST_TAG_FUNCTION]),
                                   GetUnparsed(node[AST_TAG_ARGUMENTS])));
    else
        const_cast<Object &>(node).Set(
            UNPARSE_VALUE,
            UnparseCall(GetUnparsed(node[AST_TAG_FUNCTION]),
                        GetUnparsed(node[AST_TAG_SUFFIX])));
}
void UnparseVisitor::VisitCallSuffix(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseCallSuffix(GetUnparsed(node[AST_TAG_CHILD])));
}
void UnparseVisitor::VisitNormalCall(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseNormalCall(GetUnparsed(node[AST_TAG_CHILD])));
}
void UnparseVisitor::VisitMethodCall(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseMethodCall(GetUnparsed(node[AST_TAG_FUNCTION]),
                          GetUnparsed(node[AST_TAG_ARGUMENTS])));
}
void UnparseVisitor::VisitExpressionList(const Object &node) {
    vector<string> expressions;
    for (unsigned i = 0; i < node.GetNumericSize(); ++i)
        expressions.push_back(GetUnparsed(node[i]));
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseExpressionList(expressions));
}
void UnparseVisitor::VisitObjectDef(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseObjectDef(GetUnparsed(node[AST_TAG_CHILD])));
}
void UnparseVisitor::VisitIndexed(const Object &node) {
    vector<string> indexedElements;
    for (unsigned i = 0; i < node.GetNumericSize(); ++i)
        indexedElements.push_back(GetUnparsed(node[i]));
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseIndexed(indexedElements));
}
void UnparseVisitor::VisitIndexedElem(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseIndexedElem(GetUnparsed(node[AST_TAG_OBJECT_KEY]),
                           GetUnparsed(node[AST_TAG_OBJECT_VALUE])));
}
void UnparseVisitor::VisitBlock(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseBlock(GetUnparsed(node[AST_TAG_CHILD])));
}
void UnparseVisitor::VisitFunctionDef(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseFunctionDef(GetUnparsed(node[AST_TAG_FUNCTION_ID]),
                           GetUnparsed(node[AST_TAG_FUNCTION_FORMALS]),
                           GetUnparsed(node[AST_TAG_STMT])));
}
void UnparseVisitor::VisitConst(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseConst(GetUnparsed(node[AST_TAG_CHILD])));
}
void UnparseVisitor::VisitNumber(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseNumber(GetUnparsed(node[AST_TAG_VALUE])));
}
void UnparseVisitor::VisitString(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseString(GetUnparsed(node[AST_TAG_VALUE])));
}
void UnparseVisitor::VisitNill(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseNill(GetUnparsed(node[AST_TAG_VALUE])));
}
void UnparseVisitor::VisitTrue(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseTrue(GetUnparsed(node[AST_TAG_VALUE])));
}
void UnparseVisitor::VisitFalse(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseFalse(GetUnparsed(node[AST_TAG_VALUE])));
}
void UnparseVisitor::VisitIdList(const Object &node) {
    vector<string> ids;
    for (unsigned i = 0; i < node.GetNumericSize(); ++i)
        ids.push_back(GetUnparsed(node[i]));
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseIdList(ids));
}
void UnparseVisitor::VisitIf(const Object &node) {
    if (node.ElementExists(AST_TAG_ELSE_STMT))
        const_cast<Object &>(node).Set(
            UNPARSE_VALUE,
            UnparseIf(GetUnparsed(node[AST_TAG_CONDITION]),
                      GetUnparsed(node[AST_TAG_STMT]),
                      GetUnparsed(node[AST_TAG_ELSE_STMT])));
    else
        const_cast<Object &>(node).Set(
            UNPARSE_VALUE,
            UnparseIf(GetUnparsed(node[AST_TAG_CONDITION]),
                      GetUnparsed(node[AST_TAG_STMT]),
                      ""));
}
void UnparseVisitor::VisitWhile(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseWhile(GetUnparsed(node[AST_TAG_CONDITION]),
                     GetUnparsed(node[AST_TAG_STMT])));
}
void UnparseVisitor::VisitFor(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseFor(GetUnparsed(node[AST_TAG_FOR_PRE_ELIST]),
                   GetUnparsed(node[AST_TAG_CONDITION]),
                   GetUnparsed(node[AST_TAG_FOR_POST_ELIST]),
                   GetUnparsed(node[AST_TAG_STMT])));
}
void UnparseVisitor::VisitReturn(const Object &node) {
    if (node.ElementExists(AST_TAG_CHILD))
        const_cast<Object &>(node).Set(
            UNPARSE_VALUE,
            UnparseReturn(GetUnparsed(node[AST_TAG_CHILD])));
    else
        const_cast<Object &>(node).Set(
            UNPARSE_VALUE,
            UnparseReturn(""));
}
void UnparseVisitor::VisitBreak(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseBreak());
}
void UnparseVisitor::VisitContinue(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        UnparseContinue());
}
