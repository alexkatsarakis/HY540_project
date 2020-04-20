#include "UnparseVisitor.h"

#include "Object.h"
#include "TreeTags.h"
#include "Utilities.h"

#include <cassert>
#include <fstream>

#define UNPARSE_VALUE "$$UnparseValue"
using namespace std;

void UnparseVisitor::WriteFile(const std::string &program) {
    ofstream f(fileName.c_str(), ios_base::out);
    f << program;
    f.close();
}
const std::string UnparseVisitor::GetUnparsed(const Value *val) const {
    const Value *unparseVal = val->ToObject_NoConst()->GetAndRemove(UNPARSE_VALUE);
    string str = unparseVal->ToString();
    // delete unparseVal; //bug on that
    return str;
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
const std::string UnparseVisitor::UnparseStatementNOSEMICOLON(const std::string &stmt) {
    return string(stmt);
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
const std::string UnparseVisitor::UnparseCallPARENTHESIS(const std::string &call, const std::string &arglist) {
    return string(call + "(" + arglist + ")");
}
const std::string UnparseVisitor::UnparseCall(const std::string &lvalue, const std::string &callsuffix) {
    return string(lvalue + callsuffix);
}
const std::string UnparseVisitor::UnparseCallPARENTHESISTWIN(const std::string &funcdef, const std::string &arglist) {
    return string("(" + funcdef + ")" + "(" + arglist + ")");
}
const std::string UnparseVisitor::UnparseCallSuffix(const std::string &call) {
    return call;
}
const std::string UnparseVisitor::UnparseNormalCall(const std::string &arglist) {
    return string("(" + arglist + ")");
}
const std::string UnparseVisitor::UnparseMethodCall(const std::string &id, const std::string &arglist) {
    return string(".." + id + "(" + arglist + ")");
}
const std::string UnparseVisitor::UnparseArgumentList(const std::vector<std::string> &posArguments, const std::vector<std::string> &namedArguments) {
    string argumentListStr;
    //positional
    for (auto argument : posArguments)
        argumentListStr.append(argument).append(", ");
    //named
    for (unsigned i = 0; i < namedArguments.size(); i += 2) {
        argumentListStr.append(namedArguments.at(i)).append(" : ");
        argumentListStr.append(namedArguments.at(i + 1)).append(", ");
    }
    if (argumentListStr.at(argumentListStr.size() - 2) == ',') argumentListStr.erase(argumentListStr.size() - 2);
    return argumentListStr;
}
const std::string UnparseVisitor::UnparseExpressionList(const std::vector<std::string> &expressions) {
    string expressionListStr;
    for (auto expression : expressions)
        expressionListStr.append(expression).append(", ");
    if (expressionListStr.at(expressionListStr.size() - 2) == ',') expressionListStr.erase(expressionListStr.size() - 2);
    return expressionListStr;
}
const std::string UnparseVisitor::UnparseObjectDef(const std::string &child) {
    return string("[" + child + "]");
}
const std::string UnparseVisitor::UnparseIndexed(const std::vector<std::string> &indexedElements) {
    string indexedListStr;
    for (auto indexedElement : indexedElements)
        indexedListStr.append(indexedElement).append(", ");
    if (indexedListStr.at(indexedListStr.size() - 2) == ',') indexedListStr.erase(indexedListStr.size() - 2);
    return indexedListStr;
}
const std::string UnparseVisitor::UnparseIndexedElem(const std::string &key, const std::string &value) {
    return string("{" + key + ":" + value + "}");
}
const std::string UnparseVisitor::UnparseBlock(const std::string &stmts) {
    return string("{\n" + stmts + "}");
}
const std::string UnparseVisitor::UnparseFunctionDef(const std::string &id, const std::string &idlist, const std::string &block) {
    return string("function " + id + "(" + idlist + ") " + block + "\n");
}
const std::string UnparseVisitor::UnparseConst(const std::string &child) {
    return child;
}
const std::string UnparseVisitor::UnparseNumber(const double &value) {
    return Utilities::IsInt(value) ? to_string(static_cast<int>(value)) : to_string(value);
}
const std::string UnparseVisitor::UnparseString(const std::string &value) {
    string valStr = Utilities::UnparserFormatEscChars(value);
    return string("\"" + valStr + "\"");
}
const std::string UnparseVisitor::UnparseNil() {
    return "nil";
}
const std::string UnparseVisitor::UnparseTrue() {
    return "true";
}
const std::string UnparseVisitor::UnparseFalse() {
    return "false";
}
const std::string UnparseVisitor::UnparseIdList(const std::vector<std::string> &ids) {
    string idListStr;
    for (auto id : ids)
        idListStr.append(id).append(", ");
    if (idListStr.at(idListStr.size() - 2) == ',') idListStr.erase(idListStr.size() - 2);
    return idListStr;
}
const std::string UnparseVisitor::UnparseIf(const std::string &cond, const std::string &stmt, const std::string &elseStmt) {
    string elseStr = elseStmt.empty() ? "" : " else " + elseStmt;
    return string("if (" + cond + ") " + stmt + elseStr + "\n");
}
const std::string UnparseVisitor::UnparseWhile(const std::string &expr, const std::string &stmt) {
    return string("while (" + expr + ") " + stmt + "\n");
}
const std::string UnparseVisitor::UnparseFor(const std::string &elist1, const std::string &expr, const std::string &elist2, const std::string &stmt) {
    return string("for (" + elist1 + "; " + expr + "; " + elist2 + ") " + stmt + "\n");
}
const std::string UnparseVisitor::UnparseReturn(const std::string &expr) {
    return string("return " + expr + ";\n");
}
const std::string UnparseVisitor::UnparseBreak(void) {
    return string("break;\n");
}
const std::string UnparseVisitor::UnparseContinue(void) {
    return string("continue;\n");
}

UnparseVisitor::UnparseVisitor(const std::string &_fileName) : fileName(_fileName) {}
TreeVisitor *UnparseVisitor::Clone(void) const {
    UnparseVisitor *clone = new UnparseVisitor(this->fileName);
    return clone;
}

void UnparseVisitor::VisitProgram(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseProgram(GetUnparsed(node[AST_TAG_CHILD]))));
}
void UnparseVisitor::VisitStatements(const Object &node) {
    vector<string> statements;
    for (unsigned i = 0; i < node.GetNumericSize(); ++i)
        statements.push_back(GetUnparsed(node[i]));
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseStatements(statements)));
}
void UnparseVisitor::VisitStatement(const Object &node) {
    if (!node.ElementExists(AST_TAG_CHILD))
        const_cast<Object &>(node).Set(
            UNPARSE_VALUE,
            Value(UnparseStatement("")));
    else {
        Object child = *(node[AST_TAG_CHILD]->ToObject());
        if (child[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_EXPR)
            const_cast<Object &>(node).Set(
                UNPARSE_VALUE,
                Value(UnparseStatement(GetUnparsed(node[AST_TAG_CHILD]))));
        else
            const_cast<Object &>(node).Set(
                UNPARSE_VALUE,
                Value(UnparseStatementNOSEMICOLON(GetUnparsed(node[AST_TAG_CHILD]))));
    }
}
void UnparseVisitor::VisitExpression(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseExpression(GetUnparsed(node[AST_TAG_CHILD]))));
}
void UnparseVisitor::VisitAssign(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseAssign(GetUnparsed(node[AST_TAG_LVALUE]),
                            GetUnparsed(node[AST_TAG_RVALUE]))));
}
void UnparseVisitor::VisitPlus(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparsePlus(GetUnparsed(node[AST_TAG_FIRST_EXPR]),
                          GetUnparsed(node[AST_TAG_SECOND_EXPR]))));
}
void UnparseVisitor::VisitMinus(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseMinus(GetUnparsed(node[AST_TAG_FIRST_EXPR]),
                           GetUnparsed(node[AST_TAG_SECOND_EXPR]))));
}
void UnparseVisitor::VisitMul(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseMul(GetUnparsed(node[AST_TAG_FIRST_EXPR]),
                         GetUnparsed(node[AST_TAG_SECOND_EXPR]))));
}
void UnparseVisitor::VisitDiv(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseDiv(GetUnparsed(node[AST_TAG_FIRST_EXPR]),
                         GetUnparsed(node[AST_TAG_SECOND_EXPR]))));
}
void UnparseVisitor::VisitModulo(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseModulo(GetUnparsed(node[AST_TAG_FIRST_EXPR]),
                            GetUnparsed(node[AST_TAG_SECOND_EXPR]))));
}
void UnparseVisitor::VisitGreater(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseGreater(GetUnparsed(node[AST_TAG_FIRST_EXPR]),
                             GetUnparsed(node[AST_TAG_SECOND_EXPR]))));
}
void UnparseVisitor::VisitLess(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseLess(GetUnparsed(node[AST_TAG_FIRST_EXPR]),
                          GetUnparsed(node[AST_TAG_SECOND_EXPR]))));
}
void UnparseVisitor::VisitGreaterEqual(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseGreaterEqual(GetUnparsed(node[AST_TAG_FIRST_EXPR]),
                                  GetUnparsed(node[AST_TAG_SECOND_EXPR]))));
}
void UnparseVisitor::VisitLessEqual(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseLessEqual(GetUnparsed(node[AST_TAG_FIRST_EXPR]),
                               GetUnparsed(node[AST_TAG_SECOND_EXPR]))));
}
void UnparseVisitor::VisitEqual(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseEqual(GetUnparsed(node[AST_TAG_FIRST_EXPR]),
                           GetUnparsed(node[AST_TAG_SECOND_EXPR]))));
}
void UnparseVisitor::VisitNotEqual(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseNotEqual(GetUnparsed(node[AST_TAG_FIRST_EXPR]),
                              GetUnparsed(node[AST_TAG_SECOND_EXPR]))));
}
void UnparseVisitor::VisitAnd(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseAnd(GetUnparsed(node[AST_TAG_FIRST_EXPR]),
                         GetUnparsed(node[AST_TAG_SECOND_EXPR]))));
}
void UnparseVisitor::VisitOr(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseOr(GetUnparsed(node[AST_TAG_FIRST_EXPR]),
                        GetUnparsed(node[AST_TAG_SECOND_EXPR]))));
}
void UnparseVisitor::VisitTerm(const Object &node) {
    auto child = *(node[AST_TAG_CHILD]->ToObject());
    if (child[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_EXPR)
        const_cast<Object &>(node).Set(
            UNPARSE_VALUE,
            Value(UnparseTermPARENTHESIS(GetUnparsed(node[AST_TAG_CHILD]))));
    else
        const_cast<Object &>(node).Set(
            UNPARSE_VALUE,
            Value(UnparseTerm(GetUnparsed(node[AST_TAG_CHILD]))));
}
void UnparseVisitor::VisitUnaryMinus(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseUnaryMinus(GetUnparsed(node[AST_TAG_CHILD]))));
}
void UnparseVisitor::VisitNot(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseNot(GetUnparsed(node[AST_TAG_CHILD]))));
}
void UnparseVisitor::VisitPlusPlusBefore(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparsePlusPlusBefore(GetUnparsed(node[AST_TAG_CHILD]))));
}
void UnparseVisitor::VisitPlusPlusAfter(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparsePlusPlusAfter(GetUnparsed(node[AST_TAG_CHILD]))));
}
void UnparseVisitor::VisitMinusMinusBefore(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseMinusMinusBefore(GetUnparsed(node[AST_TAG_CHILD]))));
}
void UnparseVisitor::VisitMinusMinusAfter(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseMinusMinusAfter(GetUnparsed(node[AST_TAG_CHILD]))));
}
void UnparseVisitor::VisitPrimary(const Object &node) {
    auto child = *(node[AST_TAG_CHILD]->ToObject());
    if (child[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_FUNCTION_DEF)
        const_cast<Object &>(node).Set(
            UNPARSE_VALUE,
            Value(UnparsePrimaryPARENTHESIS(GetUnparsed(node[AST_TAG_CHILD]))));
    else
        const_cast<Object &>(node).Set(
            UNPARSE_VALUE,
            Value(UnparsePrimary(GetUnparsed(node[AST_TAG_CHILD]))));
}
void UnparseVisitor::VisitLValue(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseLValue(GetUnparsed(node[AST_TAG_CHILD]))));
}
void UnparseVisitor::VisitId(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseId(node[AST_TAG_ID]->ToString())));
}
void UnparseVisitor::VisitLocal(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseLocal(node[AST_TAG_ID]->ToString())));
}
void UnparseVisitor::VisitDoubleColon(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseDoubleColon(node[AST_TAG_ID]->ToString())));
}
void UnparseVisitor::VisitDollar(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseDollar(node[AST_TAG_ID]->ToString())));
}
void UnparseVisitor::VisitMember(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseMember(GetUnparsed(node[AST_TAG_CHILD]))));
}
void UnparseVisitor::VisitDot(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseDot(GetUnparsed(node[AST_TAG_LVALUE]),
                         GetUnparsed(node[AST_TAG_ID]))));
}
void UnparseVisitor::VisitBracket(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseBracket(GetUnparsed(node[AST_TAG_LVALUE]),
                             GetUnparsed(node[AST_TAG_EXPR]))));
}
void UnparseVisitor::VisitCall(const Object &node) {
    const Object value = *(node[AST_TAG_FUNCTION]->ToObject());
    const string childType = value[AST_TAG_TYPE_KEY]->ToString();
    if (childType == AST_TAG_FUNCTION_DEF)
        const_cast<Object &>(node).Set(
            UNPARSE_VALUE,
            Value(UnparseCallPARENTHESISTWIN(GetUnparsed(node[AST_TAG_FUNCTION]),
                                             GetUnparsed(node[AST_TAG_ARGUMENTS]))));
    else if (childType == AST_TAG_CALL)
        const_cast<Object &>(node).Set(
            UNPARSE_VALUE,
            Value(UnparseCallPARENTHESIS(GetUnparsed(node[AST_TAG_FUNCTION]),
                                         GetUnparsed(node[AST_TAG_ARGUMENTS]))));
    else
        const_cast<Object &>(node).Set(
            UNPARSE_VALUE,
            Value(UnparseCall(GetUnparsed(node[AST_TAG_FUNCTION]),
                              GetUnparsed(node[AST_TAG_SUFFIX]))));
}
void UnparseVisitor::VisitCallSuffix(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseCallSuffix(GetUnparsed(node[AST_TAG_CHILD]))));
}
void UnparseVisitor::VisitNormalCall(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseNormalCall(GetUnparsed(node[AST_TAG_CHILD]))));
}
void UnparseVisitor::VisitMethodCall(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseMethodCall(GetUnparsed(node[AST_TAG_FUNCTION]),
                                GetUnparsed(node[AST_TAG_ARGUMENTS]))));
}
void UnparseVisitor::VisitArgumentList(const Object &node) {
    vector<string> posArguments;
    vector<string> namedArguments;
    for (unsigned i = 0; i < node.GetNumericSize(); ++i)
        posArguments.push_back(GetUnparsed(node[i]));
    for (const auto &key : node.GetStringKeys()) {
        namedArguments.push_back(key);
        namedArguments.push_back(GetUnparsed(node[key]));
    }
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseArgumentList(posArguments, namedArguments)));
}
void UnparseVisitor::VisitExpressionList(const Object &node) {
    vector<string> expressions;
    for (unsigned i = 0; i < node.GetNumericSize(); ++i)
        expressions.push_back(GetUnparsed(node[i]));
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseExpressionList(expressions)));
}
void UnparseVisitor::VisitObjectDef(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseObjectDef(GetUnparsed(node[AST_TAG_CHILD]))));
}
void UnparseVisitor::VisitIndexed(const Object &node) {
    vector<string> indexedElements;
    for (unsigned i = 0; i < node.GetNumericSize(); ++i)
        indexedElements.push_back(GetUnparsed(node[i]));
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseIndexed(indexedElements)));
}
void UnparseVisitor::VisitIndexedElem(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseIndexedElem(GetUnparsed(node[AST_TAG_OBJECT_KEY]),
                                 GetUnparsed(node[AST_TAG_OBJECT_VALUE]))));
}
void UnparseVisitor::VisitBlock(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseBlock(GetUnparsed(node[AST_TAG_CHILD]))));
}
void UnparseVisitor::VisitFunctionDef(const Object &node) {
    const string funcId = GetUnparsed(node[AST_TAG_FUNCTION_ID]);
    if (funcId.find("$") != std::string::npos)
        const_cast<Object &>(node).Set(
            UNPARSE_VALUE,
            Value(UnparseFunctionDef("",
                                     GetUnparsed(node[AST_TAG_FUNCTION_FORMALS]),
                                     GetUnparsed(node[AST_TAG_STMT]))));
    else
        const_cast<Object &>(node).Set(
            UNPARSE_VALUE,
            Value(UnparseFunctionDef(funcId,
                                     GetUnparsed(node[AST_TAG_FUNCTION_FORMALS]),
                                     GetUnparsed(node[AST_TAG_STMT]))));
}
void UnparseVisitor::VisitConst(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseConst(GetUnparsed(node[AST_TAG_CHILD]))));
}
void UnparseVisitor::VisitNumber(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseNumber(node[AST_TAG_VALUE]->ToNumber())));
}
void UnparseVisitor::VisitString(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseString(node[AST_TAG_VALUE]->ToString())));
}
void UnparseVisitor::VisitNil(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseNil()));
}
void UnparseVisitor::VisitTrue(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseTrue()));
}
void UnparseVisitor::VisitFalse(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseFalse()));
}
void UnparseVisitor::VisitIdList(const Object &node) {
    vector<string> ids;
    for (unsigned i = 0; i < node.GetNumericSize(); ++i)
        ids.push_back(GetUnparsed(node[i]));
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseIdList(ids)));
}
void UnparseVisitor::VisitFormal(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseId(node[AST_TAG_ID]->ToString())));
}
void UnparseVisitor::VisitIf(const Object &node) {
    if (node.ElementExists(AST_TAG_ELSE_STMT))
        const_cast<Object &>(node).Set(
            UNPARSE_VALUE,
            Value(UnparseIf(GetUnparsed(node[AST_TAG_CONDITION]),
                            GetUnparsed(node[AST_TAG_STMT]),
                            GetUnparsed(node[AST_TAG_ELSE_STMT]))));
    else
        const_cast<Object &>(node).Set(
            UNPARSE_VALUE,
            Value(UnparseIf(GetUnparsed(node[AST_TAG_CONDITION]),
                            GetUnparsed(node[AST_TAG_STMT]))));
}
void UnparseVisitor::VisitWhile(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseWhile(GetUnparsed(node[AST_TAG_CONDITION]),
                           GetUnparsed(node[AST_TAG_STMT]))));
}
void UnparseVisitor::VisitFor(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseFor(GetUnparsed(node[AST_TAG_FOR_PRE_ELIST]),
                         GetUnparsed(node[AST_TAG_CONDITION]),
                         GetUnparsed(node[AST_TAG_FOR_POST_ELIST]),
                         GetUnparsed(node[AST_TAG_STMT]))));
}
void UnparseVisitor::VisitReturn(const Object &node) {
    if (node.ElementExists(AST_TAG_CHILD))
        const_cast<Object &>(node).Set(
            UNPARSE_VALUE,
            Value(UnparseReturn(GetUnparsed(node[AST_TAG_CHILD]))));
    else
        const_cast<Object &>(node).Set(
            UNPARSE_VALUE,
            Value(UnparseReturn()));
}
void UnparseVisitor::VisitBreak(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseBreak()));
}
void UnparseVisitor::VisitContinue(const Object &node) {
    const_cast<Object &>(node).Set(
        UNPARSE_VALUE,
        Value(UnparseContinue()));
}
