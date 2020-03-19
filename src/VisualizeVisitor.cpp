#include "VisualizeVisitor.h"
#include "TreeTags.h"

#include <iostream>

#define CREATE_NEW_NODE(label) outputFile << "node" << ++lastNode <<  " [label=\"" << label << "\"]" << std::endl;
#define LINK_TO_PREVIOUS_NODE() outputFile << "node" << lastNode << " -> node" << (lastNode - 1) << std::endl;
#define LINK_TO_CHILDREN(stack) for(register unsigned i = 0; i < node.GetNumericSize(); ++i) {\
        outputFile << "node" << lastNode << " -> node" << stack.top() << std::endl;\
        stack.pop();\
    }
#define LINK_TO_2_CHILDREN(stack) outputFile << "node" << lastNode << " -> node" << stack.top() << std::endl;\
    stack.pop();\
    outputFile << "node" << lastNode << " -> node" << stack.top() << std::endl;\
    stack.pop();

void VisualizeVisitor::VisitProgram (const Object& node) {
    CREATE_NEW_NODE(AST_TAG_PROGRAM);
    LINK_TO_PREVIOUS_NODE();
    outputFile << "}" << std::endl;
    outputFile.close();
}

void VisualizeVisitor::VisitStatements (const Object& node) {
    CREATE_NEW_NODE(AST_TAG_STMTS);
    LINK_TO_CHILDREN(statements);
}

void VisualizeVisitor::VisitStatement (const Object& node) {
    CREATE_NEW_NODE(AST_TAG_STMT);
    LINK_TO_PREVIOUS_NODE();
    statements.push(lastNode);
}

void VisualizeVisitor::VisitExpression (const Object& node) {
    CREATE_NEW_NODE(AST_TAG_EXPR);
    LINK_TO_PREVIOUS_NODE();
    expressions.push(lastNode);
}

void VisualizeVisitor::VisitAssign (const Object& node) {
    CREATE_NEW_NODE(AST_TAG_ASSIGN);
    LINK_TO_2_CHILDREN(expressions);
}

void VisualizeVisitor::VisitPlus (const Object& node) {
    CREATE_NEW_NODE(AST_TAG_PLUS);
    LINK_TO_2_CHILDREN(expressions);
}

void VisualizeVisitor::VisitMinus (const Object& node) {
    CREATE_NEW_NODE(AST_TAG_MINUS);
    LINK_TO_2_CHILDREN(expressions);
}

void VisualizeVisitor::VisitMul (const Object& node) {
    CREATE_NEW_NODE(AST_TAG_MUL);
    LINK_TO_2_CHILDREN(expressions);
}

void VisualizeVisitor::VisitDiv (const Object& node) {
    CREATE_NEW_NODE(AST_TAG_DIV);
    LINK_TO_2_CHILDREN(expressions);
}

void VisualizeVisitor::VisitModulo (const Object& node) {
    CREATE_NEW_NODE(AST_TAG_MODULO);
    LINK_TO_2_CHILDREN(expressions);
}

void VisualizeVisitor::VisitGreater (const Object& node) {
    CREATE_NEW_NODE(AST_TAG_GREATER);
    LINK_TO_2_CHILDREN(expressions);
}

void VisualizeVisitor::VisitLess (const Object& node) {
    CREATE_NEW_NODE(AST_TAG_LESS);
    LINK_TO_2_CHILDREN(expressions);
}

void VisualizeVisitor::VisitGreaterEqual (const Object& node) {
    CREATE_NEW_NODE(AST_TAG_GEQUAL);
    LINK_TO_2_CHILDREN(expressions);
}

void VisualizeVisitor::VisitLessEqual (const Object& node) {
    CREATE_NEW_NODE(AST_TAG_LEQUAL);
    LINK_TO_2_CHILDREN(expressions);
}

void VisualizeVisitor::VisitEqual (const Object& node) {
    CREATE_NEW_NODE(AST_TAG_EQUAL);
    LINK_TO_2_CHILDREN(expressions);
}

void VisualizeVisitor::VisitNotEqual (const Object& node) {
    CREATE_NEW_NODE(AST_TAG_EQUAL);
    LINK_TO_2_CHILDREN(expressions);
}

void VisualizeVisitor::VisitAnd (const Object& node) {
    CREATE_NEW_NODE(AST_TAG_AND);
    LINK_TO_2_CHILDREN(expressions);
}

void VisualizeVisitor::VisitOr (const Object& node) {
    CREATE_NEW_NODE(AST_TAG_OR);
    LINK_TO_2_CHILDREN(expressions);
}

void VisualizeVisitor::VisitTerm (const Object& node) {
    CREATE_NEW_NODE(AST_TAG_TERM);
    LINK_TO_PREVIOUS_NODE();
}

void VisualizeVisitor::VisitUnaryMinus (const Object& node) {

}

void VisualizeVisitor::VisitNot (const Object& node) {

}

void VisualizeVisitor::VisitPlusPlusBefore (const Object& node) {

}

void VisualizeVisitor::VisitPlusPlusAfter (const Object& node) {

}

void VisualizeVisitor::VisitMinusMinusBefore (const Object& node) {

}

void VisualizeVisitor::VisitMinusMinusAfter (const Object& node) {

}

void VisualizeVisitor::VisitPrimary (const Object& node) {
    CREATE_NEW_NODE(AST_TAG_PRIMARY);
    LINK_TO_PREVIOUS_NODE();
}

void VisualizeVisitor::VisitLValue (const Object& node) {
    CREATE_NEW_NODE(AST_TAG_LVALUE);
    LINK_TO_PREVIOUS_NODE();
}

void VisualizeVisitor::VisitId (const Object& node) {
    CREATE_NEW_NODE(node[AST_TAG_ID]->ToString());
    ids.push(lastNode);
}

void VisualizeVisitor::VisitLocal (const Object& node) {

}

void VisualizeVisitor::VisitDoubleColon (const Object& node) {

}

void VisualizeVisitor::VisitMember (const Object& node) {

}

void VisualizeVisitor::VisitDot (const Object& node) {

}

void VisualizeVisitor::VisitBracket (const Object& node) {

}

void VisualizeVisitor::VisitCall (const Object& node) {

}

void VisualizeVisitor::VisitCallSuffix (const Object& node) {

}

void VisualizeVisitor::VisitNormalCall (const Object& node) {

}

void VisualizeVisitor::VisitMethodCall (const Object& node) {

}

void VisualizeVisitor::VisitExpressionList (const Object& node) {

}

void VisualizeVisitor::VisitObjectDef (const Object& node) {

}

void VisualizeVisitor::VisitIndexed (const Object& node) {

}

void VisualizeVisitor::VisitIndexedElem (const Object& node) {

}

void VisualizeVisitor::VisitBlock (const Object& node) {
    CREATE_NEW_NODE(AST_TAG_BLOCK);
    LINK_TO_PREVIOUS_NODE();
}

void VisualizeVisitor::VisitFunctionDef (const Object& node) {
    CREATE_NEW_NODE(AST_TAG_FUNCTION);
    LINK_TO_PREVIOUS_NODE();
    outputFile << "node" << lastNode << " -> node" << idlists.top() << std::endl;
    outputFile << "node" << lastNode << " -> node" << ids.top() << std::endl;
    idlists.pop();
    ids.pop();
}

void VisualizeVisitor::VisitConst (const Object& node) {
    CREATE_NEW_NODE(AST_TAG_CONST);
    LINK_TO_PREVIOUS_NODE();
}

void VisualizeVisitor::VisitNumber (const Object& node) {
    CREATE_NEW_NODE(node[AST_TAG_VALUE]->ToNumber());
}

void VisualizeVisitor::VisitString (const Object& node) {
    CREATE_NEW_NODE(node[AST_TAG_VALUE]->ToString());
}

void VisualizeVisitor::VisitNill (const Object& node) {
    CREATE_NEW_NODE(AST_TAG_NILL);
}

void VisualizeVisitor::VisitTrue (const Object& node) {
    CREATE_NEW_NODE(AST_TAG_TRUE);
}

void VisualizeVisitor::VisitFalse (const Object& node) {
    CREATE_NEW_NODE(AST_TAG_FALSE);
}

void VisualizeVisitor::VisitIdList (const Object& node) {
    CREATE_NEW_NODE(AST_TAG_ID_LIST);
    LINK_TO_CHILDREN(ids);
    idlists.push(lastNode);
}

void VisualizeVisitor::VisitIf (const Object& node) {

}

void VisualizeVisitor::VisitWhile (const Object& node) {

}

void VisualizeVisitor::VisitFor (const Object& node) {

}

void VisualizeVisitor::VisitReturn (const Object& node) {

}

void VisualizeVisitor::VisitBreak (const Object& node) {

}

void VisualizeVisitor::VisitContinue (const Object& node) {

}


TreeVisitor * VisualizeVisitor::Clone (void) const {

}

VisualizeVisitor::VisualizeVisitor(const std::string & filename) {
    lastNode = 0;

    outputFile.open(filename, std::ios::out | std::ios::trunc);

    if (!outputFile.is_open()) throw::std::runtime_error("Could not open file \"" + filename + "\".");

    outputFile << "digraph G {" << std::endl;
}

VisualizeVisitor::VisualizeVisitor(void) : VisualizeVisitor("alpha_AST.dot") {

}