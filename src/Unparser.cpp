#include "Unparser.h"
#include "Object.h"
#include "TreeTags.h"

#include <list>
#include <sstream>

using namespace std;

TreeVisitor *Unparser::Clone(void) const {
}

void Unparser::VisitProgram(const Object &node) {}
void Unparser::VisitStatements(const Object &node) {
    ostringstream code;
    for (unsigned int i = 0; node.GetNumericSize(); ++i) {
        string stmt = stack.top();
        stack.pop();
        code << stmt;
    }
    stack.push(code.str());
}
void Unparser::VisitStatement(const Object &node) {
    ostringstream code;
    if (node.ElementExists(AST_TAG_CHILD) == false) {
        //don't pop stack?
        code << ";";
        stack.push(code.str());
        return;
    }
    //else has child
    auto child = *(node[AST_TAG_CHILD]->ToObject());
    if (child[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_EXPR) {
        string expr = stack.top();
        stack.pop();
        code << expr << ";";
        stack.push(code.str());
    }
}
void Unparser::VisitExpression(const Object &node) {}
void Unparser::VisitAssign(const Object &node) {
    ostringstream code;
    string expr = stack.top();
    stack.pop();
    string lvalue = stack.top();
    stack.pop();
    code << lvalue << "=" << expr;
    stack.push(code.str());
}
void Unparser::VisitPlus(const Object &node) {
    ostringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << "+" << expr2;
    stack.push(code.str());
}
void Unparser::VisitMinus(const Object &node) {
    ostringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << "-" << expr2;
    stack.push(code.str());
}
void Unparser::VisitMul(const Object &node) {
    ostringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << "*" << expr2;
    stack.push(code.str());
}
void Unparser::VisitDiv(const Object &node) {
    ostringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << "/" << expr2;
    stack.push(code.str());
}
void Unparser::VisitModulo(const Object &node) {
    ostringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << "%" << expr2;
    stack.push(code.str());
}
void Unparser::VisitGreater(const Object &node) {
    ostringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << ">" << expr2;
    stack.push(code.str());
}
void Unparser::VisitLess(const Object &node) {
    ostringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << "<" << expr2;
    stack.push(code.str());
}
void Unparser::VisitGreaterEqual(const Object &node) {
    ostringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << ">=" << expr2;
    stack.push(code.str());
}
void Unparser::VisitLessEqual(const Object &node) {
    ostringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << "<=" << expr2;
    stack.push(code.str());
}
void Unparser::VisitEqual(const Object &node) {
    ostringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << "==" << expr2;
    stack.push(code.str());
}
void Unparser::VisitNotEqual(const Object &node) {
    ostringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << "!=" << expr2;
    stack.push(code.str());
}
void Unparser::VisitAnd(const Object &node) {
    ostringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << "&&" << expr2;
    stack.push(code.str());
}
void Unparser::VisitOr(const Object &node) {
    ostringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << "||" << expr2;
    stack.push(code.str());
}
void Unparser::VisitTerm(const Object &node) {}
void Unparser::VisitUnaryMinus(const Object &node) {
    ostringstream code;
    string expr = stack.top();
    stack.pop();
    code << "-" << expr;
    stack.push(code.str());
}
void Unparser::VisitNot(const Object &node) {
    ostringstream code;
    string expr = stack.top();
    stack.pop();
    code << "!" << expr;
    stack.push(code.str());
}
void Unparser::VisitPlusPlusBefore(const Object &node) {
    ostringstream code;
    string lvalue = stack.top();
    stack.pop();
    code << "++" << lvalue;
    stack.push(code.str());
}
void Unparser::VisitPlusPlusAfter(const Object &node) {
    ostringstream code;
    string lvalue = stack.top();
    stack.pop();
    code << lvalue << "++";
    stack.push(code.str());
}
void Unparser::VisitMinusMinusBefore(const Object &node) {
    ostringstream code;
    string lvalue = stack.top();
    stack.pop();
    code << "--" << lvalue;
    stack.push(code.str());
}
void Unparser::VisitMinusMinusAfter(const Object &node) {
    ostringstream code;
    string lvalue = stack.top();
    stack.pop();
    code << lvalue << "--";
    stack.push(code.str());
}
void Unparser::VisitPrimary(const Object &node) {
    auto child = *(node[AST_TAG_CHILD]->ToObject());
    if (child[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_FUNCTION_DEF) {
        ostringstream code;
        string funcdef = stack.top();
        stack.pop();
        code << "(" << funcdef << ")";
        stack.push(code.str());
    }
}
void Unparser::VisitLValue(const Object &node) {}
void Unparser::VisitId(const Object &node) {
    ostringstream code;
    string id = stack.top();
    stack.pop();
    code << id;
    stack.push(code.str());
}
void Unparser::VisitLocal(const Object &node) {
    ostringstream code;
    string id = stack.top();
    stack.pop();
    code << "local " << id;
    stack.push(code.str());
}
void Unparser::VisitDoubleColon(const Object &node) {
    ostringstream code;
    string id = stack.top();
    stack.pop();
    code << "::" << id;
    stack.push(code.str());
}
void Unparser::VisitMember(const Object &node) {}
void Unparser::VisitDot(const Object &node) {
    ostringstream code;
    string id = stack.top();
    stack.pop();
    string dot = stack.top();
    stack.pop();
    code << dot << id;
    stack.push(code.str());
}
void Unparser::VisitBracket(const Object &node) {
    ostringstream code;
    string expr = stack.top();
    stack.pop();
    code << "{" << expr << "}";
    stack.push(code.str());
}
void Unparser::VisitCall(const Object &node) {    //can't distinguish rvalues, resolve later
    ostringstream code;
    /* string elist = stack.top();
	stack.pop();
	code << "(" << elist << ")"; */
    stack.push(code.str());
}
void Unparser::VisitCallSuffix(const Object &node) {}
void Unparser::VisitNormalCall(const Object &node) {
    ostringstream code;
    string elist = stack.top();
    stack.pop();
    code << "(" << elist << ")";
    stack.push(code.str());
}
void Unparser::VisitMethodCall(const Object &node) {
    ostringstream code;
    string elist = stack.top();
    stack.pop();
    string id = stack.top();
    stack.pop();
    code << ".." << id << "(" << elist << ")";
    stack.push(code.str());
}
void Unparser::VisitExpressionList(const Object &node) {
    ostringstream code;
    list<string> exprElements;
    for (unsigned i = 0; i < node.GetNumericSize(); ++i) {
        string exprElem = stack.top();
        stack.pop();
        exprElements.push_front(exprElem);
    }
    for (const auto &elem : exprElements)
        code << elem << ", ";
    stack.push(code.str());
}
void Unparser::VisitObjectDef(const Object &node) {
    ostringstream code;
    string child = stack.top();
    stack.pop();
    code << "[" << child << "]";
    stack.push(code.str());
}
void Unparser::VisitIndexed(const Object &node) {
    ostringstream code;
    list<string> indexedElements;
    for (unsigned i = 0; i < node.GetNumericSize(); ++i) {
        string indexedElem = stack.top();
        stack.pop();
        indexedElements.push_front(indexedElem);
    }
    for (const auto &elem : indexedElements)
        code << elem << ", ";
    stack.push(code.str());
}
void Unparser::VisitIndexedElem(const Object &node) {
    ostringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << "{" << expr1 << ":" << expr2 << "}";
    stack.push(code.str());
}
void Unparser::VisitBlock(const Object &node) {
    ostringstream code;
    string stmt = stack.top();
    stack.pop();
    code << "{" << stmt << "}";
    stack.push(code.str());
}
void Unparser::VisitFunctionDef(const Object &node) {
    ostringstream code;
    string stmt = stack.top();
    stack.pop();
    string formals = stack.top();
    stack.pop();
    string id = stack.top();
    stack.pop();
    code << "function " << id << "(" << formals << ")" << stmt;
    stack.push(code.str());
}
void Unparser::VisitConst(const Object &node) {}
void Unparser::VisitNumber(const Object &node) {
    ostringstream code;
    code << node[AST_TAG_VALUE]->ToNumber();
    stack.push(code.str());
}
void Unparser::VisitString(const Object &node) {
    ostringstream code;
    code << node[AST_TAG_VALUE]->ToString();
    stack.push(code.str());
}
void Unparser::VisitNill(const Object &node) {
    stack.push("nil");
}
void Unparser::VisitTrue(const Object &node) {
    stack.push("true");
}
void Unparser::VisitFalse(const Object &node) {
    stack.push("false");
}
void Unparser::VisitIdList(const Object &node) {
    ostringstream code;
    for (int i = 0; i < node.GetNumericSize(); ++i) {
        string id = stack.top();
        stack.pop();
        code << id << ", ";
    }
    stack.push(code.str());
}
void Unparser::VisitIf(const Object &node) {
    ostringstream code;
    string elseAddition;
    if (node.ElementExists(AST_TAG_ELSE_STMT)) {
        string stmt2 = stack.top();
        stack.pop();
        elseAddition = "else " + stmt2;
    }
    string stmt1 = stack.top();
    stack.pop();
    string expr = stack.top();
    stack.pop();
    code << "if (" << expr << ")" << stmt1 << elseAddition;
    stack.push(code.str());
}
void Unparser::VisitWhile(const Object &node) {
    ostringstream code;
    string stmt = stack.top();
    stack.pop();
    string expr = stack.top();
    stack.pop();
    code << "while (" << expr << ")" << stmt;
    stack.push(code.str());
}
void Unparser::VisitFor(const Object &node) {
    ostringstream code;
    string stmt = stack.top();
    stack.pop();
    string elist2 = stack.top();
    stack.pop();
    string expr = stack.top();
    stack.pop();
    string elist1 = stack.top();
    stack.pop();
    code << "for (" << elist1 << ";" << expr << ";" << elist2 << ")" << stmt;
    stack.push(code.str());
}
void Unparser::VisitReturn(const Object &node) {
    ostringstream code;
    code << "continue ";
    if (node.ElementExists(AST_TAG_CHILD) == false) {
        code << ";";
        stack.push(code.str());
        return;
    }
    auto child = *(node[AST_TAG_CHILD]->ToObject());
    string expr = child[AST_TAG_EXPR]->ToString();
    code << expr << ";";
    stack.push(code.str());
}
void Unparser::VisitBreak(const Object &node) {
    ostringstream code;
    code << "break;";
    stack.push(code.str());
}
void Unparser::VisitContinue(const Object &node) {
    ostringstream code;
    code << "continue;";
    stack.push(code.str());
}
