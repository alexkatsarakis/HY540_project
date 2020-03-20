#include "UnparseVisitor.h"

#include "Object.h"
#include "TreeTags.h"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <list>
#include <sstream>

using namespace std;

string formatEscChars(const string &str) {
    // size_t index = 0;
    // index = str.find("\n");
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

UnparseVisitor::UnparseVisitor(const std::string &_fileName) : fileName(_fileName) {}
void UnparseVisitor::WriteFile(void) {
    ofstream f(fileName.c_str(), ios_base::out);
    assert(stack.size() == 1);
    f << stack.top();
    stack.pop();
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
    if (node.GetNumericSize() == 0) {
        stack.push("");
        return;
    }
    stringstream code;
    list<string> stmts;
    for (unsigned int i = 0; i < node.GetNumericSize(); ++i) {
        string stmt = stack.top();
        stack.pop();
        stmts.push_front(stmt);
    }
    for (const auto &stmt : stmts)
        code << stmt;
    stack.push(code.str());
}
void UnparseVisitor::VisitStatement(const Object &node) {
    stringstream code;
    if (node.ElementExists(AST_TAG_CHILD) == false) {
        code << ";" << endl;
        stack.push(code.str());
        return;
    }
    auto child = *(node[AST_TAG_CHILD]->ToObject());
    if (child[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_EXPR) {
        string expr = stack.top();
        stack.pop();
        code << expr << ";" << endl;
        stack.push(code.str());
    }
}
void UnparseVisitor::VisitExpression(const Object &node) {}
void UnparseVisitor::VisitAssign(const Object &node) {
    stringstream code;
    string expr = stack.top();
    stack.pop();
    string lvalue = stack.top();
    stack.pop();
    code << lvalue << " = " << expr;
    stack.push(code.str());
}
void UnparseVisitor::VisitPlus(const Object &node) {
    stringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << " + " << expr2;
    stack.push(code.str());
}
void UnparseVisitor::VisitMinus(const Object &node) {
    stringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << " - " << expr2;
    stack.push(code.str());
}
void UnparseVisitor::VisitMul(const Object &node) {
    stringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << " * " << expr2;
    stack.push(code.str());
}
void UnparseVisitor::VisitDiv(const Object &node) {
    stringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << " / " << expr2;
    stack.push(code.str());
}
void UnparseVisitor::VisitModulo(const Object &node) {
    stringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << " % " << expr2;
    stack.push(code.str());
}
void UnparseVisitor::VisitGreater(const Object &node) {
    stringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << " > " << expr2;
    stack.push(code.str());
}
void UnparseVisitor::VisitLess(const Object &node) {
    stringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << " < " << expr2;
    stack.push(code.str());
}
void UnparseVisitor::VisitGreaterEqual(const Object &node) {
    stringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << " >= " << expr2;
    stack.push(code.str());
}
void UnparseVisitor::VisitLessEqual(const Object &node) {
    stringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << " <= " << expr2;
    stack.push(code.str());
}
void UnparseVisitor::VisitEqual(const Object &node) {
    stringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << " == " << expr2;
    stack.push(code.str());
}
void UnparseVisitor::VisitNotEqual(const Object &node) {
    stringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << " != " << expr2;
    stack.push(code.str());
}
void UnparseVisitor::VisitAnd(const Object &node) {
    stringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << " and " << expr2;
    stack.push(code.str());
}
void UnparseVisitor::VisitOr(const Object &node) {
    stringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << expr1 << " or " << expr2;
    stack.push(code.str());
}
void UnparseVisitor::VisitTerm(const Object &node) {
    auto child = *(node[AST_TAG_CHILD]->ToObject());
    if (child[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_EXPR) {
        stringstream code;
        string expr = stack.top();
        stack.pop();
        code << "(" << expr << ")";
        stack.push(code.str());
    }
}
void UnparseVisitor::VisitUnaryMinus(const Object &node) {
    stringstream code;
    string expr = stack.top();
    stack.pop();
    code << "-" << expr;
    stack.push(code.str());
}
void UnparseVisitor::VisitNot(const Object &node) {
    stringstream code;
    string expr = stack.top();
    stack.pop();
    code << "not " << expr;
    stack.push(code.str());
}
void UnparseVisitor::VisitPlusPlusBefore(const Object &node) {
    stringstream code;
    string lvalue = stack.top();
    stack.pop();
    code << "++" << lvalue;
    stack.push(code.str());
}
void UnparseVisitor::VisitPlusPlusAfter(const Object &node) {
    stringstream code;
    string lvalue = stack.top();
    stack.pop();
    code << lvalue << "++";
    stack.push(code.str());
}
void UnparseVisitor::VisitMinusMinusBefore(const Object &node) {
    stringstream code;
    string lvalue = stack.top();
    stack.pop();
    code << "--" << lvalue;
    stack.push(code.str());
}
void UnparseVisitor::VisitMinusMinusAfter(const Object &node) {
    stringstream code;
    string lvalue = stack.top();
    stack.pop();
    code << lvalue << "--";
    stack.push(code.str());
}
void UnparseVisitor::VisitPrimary(const Object &node) {
    auto child = *(node[AST_TAG_CHILD]->ToObject());
    if (child[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_FUNCTION_DEF) {
        stringstream code;
        string funcdef = stack.top();
        stack.pop();
        code << "(" << funcdef << ")";
        stack.push(code.str());
    }
}
void UnparseVisitor::VisitLValue(const Object &node) {}
void UnparseVisitor::VisitId(const Object &node) {
    stringstream code;
    code << node[AST_TAG_ID]->ToString();
    stack.push(code.str());
}
void UnparseVisitor::VisitLocal(const Object &node) {
    stringstream code;
    string id = node[AST_TAG_ID]->ToString();
    code << "local " << id;
    stack.push(code.str());
}
void UnparseVisitor::VisitDoubleColon(const Object &node) {
    stringstream code;
    string id = node[AST_TAG_ID]->ToString();
    code << "::" << id;
    stack.push(code.str());
}
void UnparseVisitor::VisitMember(const Object &node) {}
void UnparseVisitor::VisitDot(const Object &node) {
    stringstream code;
    string id = stack.top();
    stack.pop();
    string lvalueCall = stack.top();
    stack.pop();
    code << lvalueCall << "." << id;
    stack.push(code.str());
}
void UnparseVisitor::VisitBracket(const Object &node) {
    stringstream code;
    string expr = stack.top();
    stack.pop();
    string lvalueCall = stack.top();
    stack.pop();
    code << lvalueCall << "[" << expr << "]";
    stack.push(code.str());
}
void UnparseVisitor::VisitCall(const Object &node) {
    stringstream code;
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
    stringstream code;
    string elist = stack.top();
    stack.pop();
    code << "(" << elist << ")";
    stack.push(code.str());
}
void UnparseVisitor::VisitMethodCall(const Object &node) {
    stringstream code;
    string elist = stack.top();
    stack.pop();
    string id = stack.top();
    stack.pop();
    code << ".." << id << "(" << elist << ")";
    stack.push(code.str());
}
void UnparseVisitor::VisitExpressionList(const Object &node) {
    if (node.GetNumericSize() == 0) {
        stack.push("");
        return;
    }
    stringstream code;
    list<string> exprElements;
    string last = stack.top();
    stack.pop();
    for (unsigned i = 0; i < node.GetNumericSize() - 1; ++i) {
        string exprElem = stack.top();
        stack.pop();
        exprElements.push_front(exprElem);
    }
    for (const auto &elem : exprElements)
        code << elem << ", ";
    code << last;
    stack.push(code.str());
}
void UnparseVisitor::VisitObjectDef(const Object &node) {
    stringstream code;
    string child = stack.top();
    stack.pop();
    code << "[" << child << "]";
    stack.push(code.str());
}
void UnparseVisitor::VisitIndexed(const Object &node) {
    assert(node.GetNumericSize() > 0);
    stringstream code;
    list<string> indexedElements;
    string last = stack.top();
    stack.pop();
    for (unsigned i = 0; i < node.GetNumericSize() - 1; ++i) {
        string indexedElem = stack.top();
        stack.pop();
        indexedElements.push_front(indexedElem);
    }
    for (const auto &elem : indexedElements)
        code << elem << ", ";
    code << last;
    stack.push(code.str());
}
void UnparseVisitor::VisitIndexedElem(const Object &node) {
    stringstream code;
    string expr2 = stack.top();
    stack.pop();
    string expr1 = stack.top();
    stack.pop();
    code << "{" << expr1 << ":" << expr2 << "}";
    stack.push(code.str());
}
void UnparseVisitor::VisitBlock(const Object &node) {
    stringstream code;
    string stmt = stack.top();
    stack.pop();
    code << "{" << endl
         << stmt << "}";
    stack.push(code.str());
}
void UnparseVisitor::VisitFunctionDef(const Object &node) {
    stringstream code;
    string stmt = stack.top();
    stack.pop();
    string formals = stack.top();
    stack.pop();
    string id = stack.top();
    stack.pop();
    if (id.find("$") != std::string::npos) id = "";
    code << "function " << id << "(" << formals << ") " << stmt;
    stack.push(code.str());
}
void UnparseVisitor::VisitConst(const Object &node) {}
void UnparseVisitor::VisitNumber(const Object &node) {
    stringstream code;
    code << node[AST_TAG_VALUE]->ToNumber();
    stack.push(code.str());
}
void UnparseVisitor::VisitString(const Object &node) {
    stringstream code;
    string inStr = node[AST_TAG_VALUE]->ToString();
    string outStr = formatEscChars(inStr);
    code << "\"";
    code << outStr;
    code << "\"";
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
    if (node.GetNumericSize() == 0) {
        stack.push("");
        return;
    };
    stringstream code;
    list<string> ids;
    string last = stack.top();
    stack.pop();
    for (int i = 0; i < node.GetNumericSize() - 1; ++i) {
        string id = stack.top();
        stack.pop();
        ids.push_front(id);
    }
    for (const auto &id : ids)
        code << id << ", ";
    code << last;
    stack.push(code.str());
}
void UnparseVisitor::VisitIf(const Object &node) {
    stringstream code;
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
    code << "if (" << expr << ") " << stmt1 << elseAddition;
    stack.push(code.str());
}
void UnparseVisitor::VisitWhile(const Object &node) {
    stringstream code;
    string stmt = stack.top();
    stack.pop();
    string expr = stack.top();
    stack.pop();
    code << "while (" << expr << ") " << stmt;
    stack.push(code.str());
}
void UnparseVisitor::VisitFor(const Object &node) {
    stringstream code;
    string stmt = stack.top();
    stack.pop();
    string elist2 = stack.top();
    stack.pop();
    string expr = stack.top();
    stack.pop();
    string elist1 = stack.top();
    stack.pop();
    code << "for (" << elist1 << "; " << expr << "; " << elist2 << ") " << stmt;
    stack.push(code.str());
}
void UnparseVisitor::VisitReturn(const Object &node) {
    stringstream code;
    code << "return ";
    if (node.ElementExists(AST_TAG_CHILD) == false) {
        code << ";" << endl;
        stack.push(code.str());
        return;
    }
    auto child = *(node[AST_TAG_CHILD]->ToObject());
    string expr = stack.top();
    stack.pop();
    code << expr << ";" << endl;
    stack.push(code.str());
}
void UnparseVisitor::VisitBreak(const Object &node) {
    stack.push("break;");
}
void UnparseVisitor::VisitContinue(const Object &node) {
    stack.push("continue;");
}
