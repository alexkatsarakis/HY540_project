#include "VisualizeVisitor.h"
#include "TreeTags.h"

#include <iostream>

void VisualizeVisitor::CreateNewNode(const std::string & label) {
    output << "node" << ++lastNode <<  " [label=\"" << label << "\"]" << std::endl;
}

void VisualizeVisitor::LinkToPreviousNode(void) {
    if (lastNode != 1)
        output << "node" << lastNode << " -> node" << (lastNode - 1) << std::endl;
}

void VisualizeVisitor::LinkToOrphan() {
    if (orphans.size() != 0) {
        LinkToNode(orphans.top());
        orphans.pop();
    }
}

void VisualizeVisitor::LinkToNode(unsigned node) {
    output << "node" << lastNode << " -> node" << node << std::endl;
}

void VisualizeVisitor::SaveOrphan(void) {
    if (lastNode != 0) orphans.push(lastNode);
}

void VisualizeVisitor::DumpToFile(void) {
    std::ofstream file;
    file.open(outputFile, std::ios::out | std::ios::trunc);
    if (!file.is_open()) throw::std::runtime_error("Could not open file \"" + outputFile + "\".");
    file << output.rdbuf();
}

void VisualizeVisitor::VisitProgram (const Object& node) {
    CreateNewNode(AST_TAG_PROGRAM);
    LinkToPreviousNode();
    output << "}" << std::endl;
    DumpToFile();
}

void VisualizeVisitor::VisitEmptyStatement (const Object& node) {
    /* Save the previous sub-tree because a new terminal is created */
    SaveOrphan();
    CreateNewNode(AST_TAG_STMTS);
}

void VisualizeVisitor::VisitNormalStatemens(const Object& node) {
    CreateNewNode(AST_TAG_STMTS);
    LinkToPreviousNode();

    for(register unsigned i = 1; i < node.GetNumericSize(); ++i) {
        output << "node" << lastNode << " -> node" << orphans.top() << std::endl;
        orphans.pop();
    }
}

void VisualizeVisitor::VisitStatements (const Object& node) {
    if (node.GetTotal() == 1) VisitEmptyStatement(node);
    else VisitNormalStatemens(node);
}

void VisualizeVisitor::VisitStatement (const Object& node) {
    CreateNewNode(AST_TAG_STMT);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitExpression (const Object& node) {
    CreateNewNode(AST_TAG_EXPR);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitAssign (const Object& node) {
    CreateNewNode(AST_TAG_ASSIGN);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitPlus (const Object& node) {
    CreateNewNode(AST_TAG_PLUS);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitMinus (const Object& node) {
    CreateNewNode(AST_TAG_MINUS);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitMul (const Object& node) {
    CreateNewNode(AST_TAG_MUL);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitDiv (const Object& node) {
    CreateNewNode(AST_TAG_DIV);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitModulo (const Object& node) {
    CreateNewNode(AST_TAG_MODULO);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitGreater (const Object& node) {
    CreateNewNode(AST_TAG_GREATER);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitLess (const Object& node) {
    CreateNewNode(AST_TAG_LESS);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitGreaterEqual (const Object& node) {
    CreateNewNode(AST_TAG_GEQUAL);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitLessEqual (const Object& node) {
    CreateNewNode(AST_TAG_LEQUAL);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitEqual (const Object& node) {
    CreateNewNode(AST_TAG_EQUAL);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitNotEqual (const Object& node) {
    CreateNewNode(AST_TAG_EQUAL);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitAnd (const Object& node) {
    CreateNewNode(AST_TAG_AND);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitOr (const Object& node) {
    CreateNewNode(AST_TAG_OR);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitTerm (const Object& node) {
    CreateNewNode(AST_TAG_TERM);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitUnaryMinus (const Object& node) {
    CreateNewNode(AST_TAG_UMINUS);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitNot (const Object& node) {
    CreateNewNode(AST_TAG_NOT);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitPlusPlusBefore (const Object& node) {
    CreateNewNode(AST_TAG_BPLUSPLUS);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitPlusPlusAfter (const Object& node) {
    CreateNewNode(AST_TAG_APLUSPLUS);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitMinusMinusBefore (const Object& node) {
    CreateNewNode(AST_TAG_BMINUSMINUS);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitMinusMinusAfter (const Object& node) {
    CreateNewNode(AST_TAG_AMINUSMINUS);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitPrimary (const Object& node) {
    CreateNewNode(AST_TAG_PRIMARY);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitLValue (const Object& node) {
    CreateNewNode(AST_TAG_LVALUE);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitId (const Object& node) {
    SaveOrphan();
    CreateNewNode(node[AST_TAG_ID]->ToString());
}

void VisualizeVisitor::VisitLocal (const Object& node) {
    SaveOrphan();
    CreateNewNode("local " + node[AST_TAG_ID]->ToString());
}

void VisualizeVisitor::VisitDoubleColon (const Object& node) {
    SaveOrphan();
    CreateNewNode("::" + node[AST_TAG_ID]->ToString());
}

void VisualizeVisitor::VisitMember (const Object& node) {
    CreateNewNode(AST_TAG_MEMBER);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitDot (const Object& node) {
    CreateNewNode(AST_TAG_DOT);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitBracket (const Object& node) {
    CreateNewNode(AST_TAG_BRACKET);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitCall (const Object& node) {
    CreateNewNode(AST_TAG_CALL);
}

void VisualizeVisitor::VisitCallSuffix (const Object& node) {
    CreateNewNode(AST_TAG_CALL_SUFFIX);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitNormalCall (const Object& node) {
    CreateNewNode(AST_TAG_NORMAL_CALL);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitMethodCall (const Object& node) {
    CreateNewNode(AST_TAG_METHOD_CALL);
}

void VisualizeVisitor::VisitExpressionList (const Object& node) {

    if (node.GetTotal() == 1) {
        SaveOrphan();
        CreateNewNode(AST_TAG_ELIST);
        return;
    }

    CreateNewNode(AST_TAG_ELIST);
    LinkToPreviousNode();

    for(register unsigned i = 1; i < node.GetNumericSize(); ++i) {
        output << "node" << lastNode << " -> node" << orphans.top() << std::endl;
        orphans.pop();
    }
}

void VisualizeVisitor::VisitObjectDef (const Object& node) {
    CreateNewNode(AST_TAG_OBJECT_DEF);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitIndexed (const Object& node) {
    if (node.GetTotal() == 1) {
        SaveOrphan();
        CreateNewNode(AST_TAG_INDEXED);
        return;
    }

    CreateNewNode(AST_TAG_INDEXED);
    LinkToPreviousNode();

    for(register unsigned i = 1; i < node.GetNumericSize(); ++i) {
        output << "node" << lastNode << " -> node" << orphans.top() << std::endl;
        orphans.pop();
    }
}

void VisualizeVisitor::VisitIndexedElem (const Object& node) {
    CreateNewNode(AST_TAG_INDEXED_ELEM);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitBlock (const Object& node) {
    CreateNewNode(AST_TAG_BLOCK);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitFunctionDef (const Object& node) {
    CreateNewNode(AST_TAG_FUNCTION);
}

void VisualizeVisitor::VisitConst (const Object& node) {
    CreateNewNode(AST_TAG_CONST);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitNumber (const Object& node) {
    SaveOrphan();
    CreateNewNode(std::to_string(node[AST_TAG_VALUE]->ToNumber()));
}

void VisualizeVisitor::VisitString (const Object& node) {
    SaveOrphan();
    CreateNewNode("\\\"" + node[AST_TAG_VALUE]->ToString() + "\\\"");
}

void VisualizeVisitor::VisitNill (const Object& node) {
    SaveOrphan();
    CreateNewNode(AST_TAG_NILL);
}

void VisualizeVisitor::VisitTrue (const Object& node) {
    SaveOrphan();
    CreateNewNode(AST_TAG_TRUE);
}

void VisualizeVisitor::VisitFalse (const Object& node) {
    SaveOrphan();
    CreateNewNode(AST_TAG_FALSE);
}

void VisualizeVisitor::VisitIdList (const Object& node) {
    CreateNewNode(AST_TAG_ID_LIST);
}

void VisualizeVisitor::VisitIf (const Object& node) {
    CreateNewNode(AST_TAG_IF);
    LinkToPreviousNode();
    LinkToOrphan();
    if (node.ElementExists(AST_TAG_ELSE_STMT)) LinkToOrphan();
}

void VisualizeVisitor::VisitWhile (const Object& node) {
    CreateNewNode(AST_TAG_WHILE);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitFor (const Object& node) {
    CreateNewNode(AST_TAG_FOR);
    LinkToPreviousNode();  // Stmt
    LinkToOrphan();        // Elist2
    LinkToOrphan();        // Cond
    LinkToOrphan();        // Elist1
}

void VisualizeVisitor::VisitReturn (const Object& node) {
    if (node.ElementExists(AST_TAG_CHILD)) {
        CreateNewNode(AST_TAG_RETURN);
        LinkToPreviousNode();
    } else {
        SaveOrphan();
        CreateNewNode(AST_TAG_RETURN);
    }
}

void VisualizeVisitor::VisitBreak (const Object& node) {
    SaveOrphan();
    CreateNewNode(AST_TAG_BREAK);
}

void VisualizeVisitor::VisitContinue (const Object& node) {
    SaveOrphan();
    CreateNewNode(AST_TAG_CONTINUE);
}

TreeVisitor * VisualizeVisitor::Clone (void) const {
    auto visitor = new VisualizeVisitor();
    visitor->lastNode = lastNode;
    visitor->output << output.rdbuf();
    visitor->outputFile = outputFile;
    visitor->orphans = orphans;

    return visitor;
}

VisualizeVisitor::VisualizeVisitor(const std::string & filename) {
    lastNode = 0;
    outputFile = filename;
    output << "digraph G {" << std::endl;
}

VisualizeVisitor::VisualizeVisitor(void) : VisualizeVisitor("alpha_AST.dot") { }