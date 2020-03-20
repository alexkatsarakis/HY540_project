#include "UnparseVisitor.h"

#include "Object.h"
#include "TreeTags.h"

#include <cassert>
#include <fstream>
#include <list>
#include <sstream>

using namespace std;

UnparseVisitor::UnparseVisitor(const std::string &_fileName) : fileName(_fileName) {}
void UnparseVisitor::WriteFile(void) {
    ofstream f(fileName.c_str(), ios_base::out);
    assert(stack.size() == 1);
    f << stack.top();
    f.close();
}
TreeVisitor *UnparseVisitor::Clone(void) const {
    UnparseVisitor *clone = new UnparseVisitor(this->fileName);
    clone->stack = this->stack;    // copy assignment
    return clone;
}
void UnparseVisitor::VisitProgram(const Object &node) {
    WriteFile();
}
void UnparseVisitor::VisitStatements(const Object &node) {
    ostringstream code;
    for (unsigned int i = 0; i < node.GetNumericSize(); ++i) {
        string stmt = stack.top();
        stack.pop();
        code << stmt;
    }
    stack.push(code.str());
}
void UnparseVisitor::VisitStatement(const Object &node) {
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
void UnparseVisitor::VisitExpression(const Object &node) {}
void UnparseVisitor::VisitAssign(const Object &node) {
    ostringstream code;
    string expr = stack.top();
    stack.pop();
    string lvalue = stack.top();
    stack.pop();
    code << lvalue << "=" << expr;
    stack.push(code.str());
}
void UnparseVisitor::VisitPlus(const Object &node) {
    ostringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << "+" << expr2;
    stack.push(code.str());
}
void UnparseVisitor::VisitMinus(const Object &node) {
    ostringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << "-" << expr2;
    stack.push(code.str());
}
void UnparseVisitor::VisitMul(const Object &node) {
    ostringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << "*" << expr2;
    stack.push(code.str());
}
void UnparseVisitor::VisitDiv(const Object &node) {
    ostringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << "/" << expr2;
    stack.push(code.str());
}
void UnparseVisitor::VisitModulo(const Object &node) {
    ostringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << "%" << expr2;
    stack.push(code.str());
}
void UnparseVisitor::VisitGreater(const Object &node) {
    ostringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << ">" << expr2;
    stack.push(code.str());
}
void UnparseVisitor::VisitLess(const Object &node) {
    ostringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << "<" << expr2;
    stack.push(code.str());
}
void UnparseVisitor::VisitGreaterEqual(const Object &node) {
    ostringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << ">=" << expr2;
    stack.push(code.str());
}
void UnparseVisitor::VisitLessEqual(const Object &node) {
    ostringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << "<=" << expr2;
    stack.push(code.str());
}
void UnparseVisitor::VisitEqual(const Object &node) {
    ostringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << "==" << expr2;
    stack.push(code.str());
}
void UnparseVisitor::VisitNotEqual(const Object &node) {
    ostringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << "!=" << expr2;
    stack.push(code.str());
}
void UnparseVisitor::VisitAnd(const Object &node) {
    ostringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << "&&" << expr2;
    stack.push(code.str());
}
void UnparseVisitor::VisitOr(const Object &node) {
    ostringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << "||" << expr2;
    stack.push(code.str());
}
void UnparseVisitor::VisitTerm(const Object &node) {}
void UnparseVisitor::VisitUnaryMinus(const Object &node) {
    ostringstream code;
    string expr = stack.top();
    stack.pop();
    code << "-" << expr;
    stack.push(code.str());
}
void UnparseVisitor::VisitNot(const Object &node) {
    ostringstream code;
    string expr = stack.top();
    stack.pop();
    code << "!" << expr;
    stack.push(code.str());
}
void UnparseVisitor::VisitPlusPlusBefore(const Object &node) {
    ostringstream code;
    string lvalue = stack.top();
    stack.pop();
    code << "++" << lvalue;
    stack.push(code.str());
}
void UnparseVisitor::VisitPlusPlusAfter(const Object &node) {
    ostringstream code;
    string lvalue = stack.top();
    stack.pop();
    code << lvalue << "++";
    stack.push(code.str());
}
void UnparseVisitor::VisitMinusMinusBefore(const Object &node) {
    ostringstream code;
    string lvalue = stack.top();
    stack.pop();
    code << "--" << lvalue;
    stack.push(code.str());
}
void UnparseVisitor::VisitMinusMinusAfter(const Object &node) {
    ostringstream code;
    string lvalue = stack.top();
    stack.pop();
    code << lvalue << "--";
    stack.push(code.str());
}
void UnparseVisitor::VisitPrimary(const Object &node) {
    auto child = *(node[AST_TAG_CHILD]->ToObject());
    if (child[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_FUNCTION_DEF) {
        ostringstream code;
        string funcdef = stack.top();
        stack.pop();
        code << "(" << funcdef << ")";
        stack.push(code.str());
    }
}
void UnparseVisitor::VisitLValue(const Object &node) {}
void UnparseVisitor::VisitId(const Object &node) {
    ostringstream code;
    code << node[AST_TAG_ID]->ToString();
    stack.push(code.str());
}
void UnparseVisitor::VisitLocal(const Object &node) {
    ostringstream code;
    string id = node[AST_TAG_LOCAL_ID]->ToString();
    code << "local " << id;
    stack.push(code.str());
}
void UnparseVisitor::VisitDoubleColon(const Object &node) {
    ostringstream code;
    string id = node[AST_TAG_DOUBLECOLON_ID]->ToString();
    code << "::" << id;
    stack.push(code.str());
}
void UnparseVisitor::VisitMember(const Object &node) {}
void UnparseVisitor::VisitDot(const Object &node) {
    ostringstream code;
    string id = stack.top();
    stack.pop();
    string dot = stack.top();
    stack.pop();
    code << dot << id;
    stack.push(code.str());
}
void UnparseVisitor::VisitBracket(const Object &node) {
    ostringstream code;
    string expr = stack.top();
    stack.pop();
    code << "{" << expr << "}";
    stack.push(code.str());
}
void UnparseVisitor::VisitCall(const Object &node) {
    ostringstream code;
    const Object value = *(node[AST_TAG_FUNCTION]->ToObject());
    const string childType = value[AST_TAG_TYPE_KEY]->ToString();
    if (childType == AST_TAG_FUNCTION_DEF) {
        string elist = stack.top();
        stack.pop();
        string funcdef = stack.top();
        stack.pop();
        code << "(" << funcdef << ")"
             << "(" << elist << ")";
    } else if (childType == AST_TAG_CALL) {
        string elist = stack.top();
        stack.pop();
        string calll = stack.top();
        stack.pop();
        code << calll << "(" << elist << ")";
    } else {
        string callSuffix = stack.top();
        stack.pop();
        string lvalue = stack.top();
        stack.pop();
        code << lvalue << callSuffix;
    }
    stack.push(code.str());
}
void UnparseVisitor::VisitCallSuffix(const Object &node) {}
void UnparseVisitor::VisitNormalCall(const Object &node) {
    ostringstream code;
    string elist = stack.top();
    stack.pop();
    code << "(" << elist << ")";
    stack.push(code.str());
}
void UnparseVisitor::VisitMethodCall(const Object &node) {
    ostringstream code;
    string elist = stack.top();
    stack.pop();
    string id = stack.top();
    stack.pop();
    code << ".." << id << "(" << elist << ")";
    stack.push(code.str());
}
void UnparseVisitor::VisitExpressionList(const Object &node) {
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
void UnparseVisitor::VisitObjectDef(const Object &node) {
    ostringstream code;
    string child = stack.top();
    stack.pop();
    code << "[" << child << "]";
    stack.push(code.str());
}
void UnparseVisitor::VisitIndexed(const Object &node) {
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
void UnparseVisitor::VisitIndexedElem(const Object &node) {
    ostringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << "{" << expr1 << ":" << expr2 << "}";
    stack.push(code.str());
}
void UnparseVisitor::VisitBlock(const Object &node) {
    ostringstream code;
    string stmt = stack.top();
    stack.pop();
    code << "{" << stmt << "}";
    stack.push(code.str());
}
void UnparseVisitor::VisitFunctionDef(const Object &node) {
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
void UnparseVisitor::VisitConst(const Object &node) {}
void UnparseVisitor::VisitNumber(const Object &node) {
    ostringstream code;
    code << node[AST_TAG_VALUE]->ToNumber();
    stack.push(code.str());
}
void UnparseVisitor::VisitString(const Object &node) {
    ostringstream code;
    code << node[AST_TAG_VALUE]->ToString();
    stack.push(code.str());
}
void UnparseVisitor::VisitNill(const Object &node) {
    stack.push("nil");
}
void UnparseVisitor::VisitTrue(const Object &node) {
    stack.push("true");
}
void UnparseVisitor::VisitFalse(const Object &node) {
    stack.push("false");
}
void UnparseVisitor::VisitIdList(const Object &node) {
    ostringstream code;
    for (int i = 0; i < node.GetNumericSize(); ++i) {
        string id = stack.top();
        stack.pop();
        code << id << ", ";
    }
    stack.push(code.str());
}
void UnparseVisitor::VisitIf(const Object &node) {
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
void UnparseVisitor::VisitWhile(const Object &node) {
    ostringstream code;
    string stmt = stack.top();
    stack.pop();
    string expr = stack.top();
    stack.pop();
    code << "while (" << expr << ")" << stmt;
    stack.push(code.str());
}
void UnparseVisitor::VisitFor(const Object &node) {
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
void UnparseVisitor::VisitReturn(const Object &node) {
    ostringstream code;
    code << "continue ";
    if (node.ElementExists(AST_TAG_CHILD) == false) {
        code << ";";
        stack.push(code.str());
        return;
    }
    auto child = *(node[AST_TAG_CHILD]->ToObject());
    string expr = child[AST_TAG_EXPR]->ToString();    //stack?
    code << expr << ";";
    stack.push(code.str());
}
void UnparseVisitor::VisitBreak(const Object &node) {
    stack.push("break;");
}
void UnparseVisitor::VisitContinue(const Object &node) {
    stack.push("continue;");
}
