#include "VisualizeVisitor.h"
#include "TreeTags.h"
#include "Utilities.h"

#include <cassert>
#include <string>

#define DEFAULT_DOT_FILE "alpha_AST.dot"

#define EXTREME_ASSERT
#ifdef EXTREME_ASSERT
#define eassert(x) assert(x)
#else
#define eassert(x) ;
#endif

#ifdef MACROS
#define NODE_PREFIX "node"
#define TO_NODE_PREFIX (" -> " NODE_PREFIX)
#else
std::string NODE_PREFIX("node");
std::string TO_NODE_PREFIX(" -> " + NODE_PREFIX);
#endif

void findAndReplaceAll(std::string &data, std::string toSearch, std::string replaceStr) {
    // Get the first occurrence
    size_t pos = data.find(toSearch);

    // Repeat till end is reached
    while (pos != std::string::npos) {
        // Replace this occurrence of Sub String
        data.replace(pos, toSearch.size(), replaceStr);
        // Get the next occurrence from the current position
        pos = data.find(toSearch, pos + replaceStr.size());
    }
}

void VisualizeVisitor::CreateNewNode(const std::string &label) {
    output << NODE_PREFIX << ++lastNode << " [label=\"" << label << "\"]" << std::endl;
}

void VisualizeVisitor::LinkToPreviousNode(void) {
    if (lastNode != 1)
        output << NODE_PREFIX << lastNode << TO_NODE_PREFIX << (lastNode - 1) << std::endl;
}

void VisualizeVisitor::LinkToOrphan() {
    assert(!orphans.empty());
    LinkToNode(orphans.top());
    orphans.pop();
}

void VisualizeVisitor::LinkToNode(unsigned node) {
    output << NODE_PREFIX << lastNode << TO_NODE_PREFIX << node << std::endl;
}

void VisualizeVisitor::LinkToNodeEdgeLabel(unsigned node, const std::string &str) {
    assert(!str.empty());
    output << NODE_PREFIX << lastNode << TO_NODE_PREFIX << node << "[ label = \"" << str << "\"]" << std::endl;
}

void VisualizeVisitor::SaveOrphan(void) {
    if (lastNode != 0) orphans.push(lastNode);
}

void VisualizeVisitor::DumpToFile(void) {
    std::ofstream file;
    file.open(outputFile, std::ios::out | std::ios::trunc);
    if (!file.is_open()) throw ::std::runtime_error("Could not open file \"" + outputFile + "\".");

    file << output.rdbuf();

    file.close();
}

void VisualizeVisitor::VisitProgram(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_PROGRAM);

    CreateNewNode(AST_TAG_PROGRAM);
    LinkToPreviousNode();

    output << "}" << std::endl;

    DumpToFile();
}

void VisualizeVisitor::VisitEmptyStatements(const Object &node) {
    /* Save the previous sub-tree because a new terminal is created */
    SaveOrphan();
    CreateNewNode(AST_TAG_STMTS);
}

void VisualizeVisitor::VisitNormalStatements(const Object &node) {
    CreateNewNode(AST_TAG_STMTS);
    LinkToPreviousNode();

    for (register unsigned i = 1; i < node.GetNumericSize(); ++i) LinkToOrphan();
}

void VisualizeVisitor::VisitStatements(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_STMTS);
    if (node.GetTotal() == 1)
        VisitEmptyStatements(node);
    else
        VisitNormalStatements(node);
}

void VisualizeVisitor::VisitEmptyStatement(const Object &node) {
    SaveOrphan();
    CreateNewNode(AST_TAG_STMT);
}

void VisualizeVisitor::VisitNormalStatement(const Object &node) {
    CreateNewNode(AST_TAG_STMT);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitStatement(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_STMT);
    if (node.GetTotal() == 1)
        VisitEmptyStatement(node);
    else
        VisitNormalStatement(node);
}

void VisualizeVisitor::VisitExpression(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_EXPR);
    CreateNewNode(AST_TAG_EXPR);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitAssign(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_ASSIGN);
    CreateNewNode(AST_TAG_ASSIGN);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitPlus(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_PLUS);
    CreateNewNode(AST_TAG_PLUS);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitMinus(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_MINUS);
    CreateNewNode(AST_TAG_MINUS);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitMul(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_MUL);
    CreateNewNode(AST_TAG_MUL);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitDiv(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_DIV);
    CreateNewNode(AST_TAG_DIV);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitModulo(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_MODULO);
    CreateNewNode(AST_TAG_MODULO);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitGreater(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_GREATER);
    CreateNewNode(AST_TAG_GREATER);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitLess(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_LESS);
    CreateNewNode(AST_TAG_LESS);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitGreaterEqual(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_GEQUAL);
    CreateNewNode(AST_TAG_GEQUAL);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitLessEqual(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_LEQUAL);
    CreateNewNode(AST_TAG_LEQUAL);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitEqual(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_EQUAL);
    CreateNewNode(AST_TAG_EQUAL);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitNotEqual(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_NEQUAL);
    CreateNewNode(AST_TAG_NEQUAL);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitAnd(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_AND);
    CreateNewNode(AST_TAG_AND);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitOr(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_OR);
    CreateNewNode(AST_TAG_OR);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitTerm(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_TERM);
    CreateNewNode(AST_TAG_TERM);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitUnaryMinus(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_UMINUS);
    CreateNewNode(AST_TAG_UMINUS);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitNot(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_NOT);
    CreateNewNode(AST_TAG_NOT);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitPlusPlusBefore(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_BPLUSPLUS);
    CreateNewNode(AST_TAG_BPLUSPLUS);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitPlusPlusAfter(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_APLUSPLUS);
    CreateNewNode(AST_TAG_APLUSPLUS);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitMinusMinusBefore(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_BMINUSMINUS);
    CreateNewNode(AST_TAG_BMINUSMINUS);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitMinusMinusAfter(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_AMINUSMINUS);
    CreateNewNode(AST_TAG_AMINUSMINUS);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitPrimary(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_PRIMARY);
    CreateNewNode(AST_TAG_PRIMARY);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitLValue(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_LVALUE);
    CreateNewNode(AST_TAG_LVALUE);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitId(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_ID);
    SaveOrphan();
    CreateNewNode(node[AST_TAG_ID]->ToString());
}

void VisualizeVisitor::VisitLocal(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_LOCAL_ID);
    SaveOrphan();
    CreateNewNode("local " + node[AST_TAG_ID]->ToString());
}

void VisualizeVisitor::VisitDoubleColon(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_DOUBLECOLON_ID);
    SaveOrphan();
    CreateNewNode("::" + node[AST_TAG_ID]->ToString());
}

void VisualizeVisitor::VisitDollar(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_DOLLAR_ID);
    SaveOrphan();
    CreateNewNode(node[AST_TAG_ID]->ToString());
}

void VisualizeVisitor::VisitMember(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_MEMBER);
    CreateNewNode(AST_TAG_MEMBER);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitDot(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_DOT);
    CreateNewNode(AST_TAG_DOT);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitBracket(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_BRACKET);
    CreateNewNode(AST_TAG_BRACKET);
    LinkToPreviousNode();
    LinkToOrphan();
}

void VisualizeVisitor::VisitCall(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_CALL);
    CreateNewNode(AST_TAG_CALL);
    LinkToPreviousNode();
    LinkToOrphan();
    if (node.ElementExists(AST_TAG_LVALUE)) LinkToOrphan();
}

void VisualizeVisitor::VisitEmptyArgumentList(const Object &node) {
    SaveOrphan();
    CreateNewNode(AST_TAG_ARGLIST);
}

void VisualizeVisitor::VisitNormalArgumentList(const Object &node) {
    CreateNewNode(AST_TAG_ARGLIST);
    /*
    auto named = node.GetUserKeys();
    auto it = named.rbegin();
    unsigned i = 0;

    if (named.empty()) {
        LinkToPreviousNode();
        i = 1;
    }
    else LinkToNodeEdgeLabel(lastNode - 1, *(it++));

    for(; it != named.rend(); ++it) {
        assert(!orphans.empty());
        LinkToNodeEdgeLabel(orphans.top(), *it);
        orphans.pop();
    }

    for (; i < node.GetNumericSize(); ++i) LinkToOrphan();
 */
    LinkToPreviousNode();
    for (register unsigned i = 1; i < node.GetNumericSize(); ++i) LinkToOrphan();
}

void VisualizeVisitor::VisitArgumentList(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_ARGLIST);
    if (node.GetNumericSize() == 0)    //change from GetTotal
        VisitEmptyArgumentList(node);
    else
        VisitNormalArgumentList(node);
}

void VisualizeVisitor::VisitNamedArgument(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_NAMED);
    CreateNewNode(AST_TAG_NAMED);
    LinkToPreviousNode();    // Expr
    LinkToOrphan();          // id
}

void VisualizeVisitor::VisitEmptyExpressionList(const Object &node) {
    SaveOrphan();
    CreateNewNode(AST_TAG_ELIST);
}

void VisualizeVisitor::VisitNormalExpressionList(const Object &node) {
    CreateNewNode(AST_TAG_ELIST);
    LinkToPreviousNode();

    for (register unsigned i = 1; i < node.GetNumericSize(); ++i) LinkToOrphan();
}

void VisualizeVisitor::VisitExpressionList(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_ELIST);
    if (node.GetTotal() == 1)
        VisitEmptyExpressionList(node);
    else
        VisitNormalExpressionList(node);
}

void VisualizeVisitor::VisitObjectDef(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_OBJECT_DEF);
    CreateNewNode(AST_TAG_OBJECT_DEF);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitEmptyIndexed(const Object &node) {
    SaveOrphan();
    CreateNewNode(AST_TAG_INDEXED);
}

void VisualizeVisitor::VisitNormalIndexed(const Object &node) {
    CreateNewNode(AST_TAG_INDEXED);
    LinkToPreviousNode();

    for (register unsigned i = 1; i < node.GetNumericSize(); ++i) LinkToOrphan();
}

void VisualizeVisitor::VisitIndexed(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_INDEXED);
    if (node.GetTotal() == 1)
        VisitEmptyIndexed(node);
    else
        VisitNormalIndexed(node);
}

void VisualizeVisitor::VisitIndexedElem(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_INDEXED_ELEM);
    CreateNewNode(AST_TAG_INDEXED_ELEM);
    LinkToPreviousNode();    // Expr
    LinkToOrphan();          // Expr
}

void VisualizeVisitor::VisitBlock(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_BLOCK);
    CreateNewNode(AST_TAG_BLOCK);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitFunctionDef(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_FUNCTION_DEF);
    CreateNewNode(AST_TAG_FUNCTION_DEF);

    LinkToPreviousNode();    // Stmt
    LinkToOrphan();          // Idlist
    LinkToOrphan();          // Id
}

void VisualizeVisitor::VisitConst(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_CONST);
    CreateNewNode(AST_TAG_CONST);
    LinkToPreviousNode();
}

void VisualizeVisitor::VisitNumber(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_NUMBER);
    SaveOrphan();
    double value = node[AST_TAG_VALUE]->ToNumber();
    std::string str = (Utilities::IsInt(value)) ? std::to_string(static_cast<int>(value)) : std::to_string(value);
    CreateNewNode(str);
}

void VisualizeVisitor::VisitString(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_STRING);
    SaveOrphan();

    std::string label(node[AST_TAG_VALUE]->ToString());
    findAndReplaceAll(label, "\"", "\\\"");
    CreateNewNode("\\\"" + label + "\\\"");
}

void VisualizeVisitor::VisitNil(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_NIL);
    SaveOrphan();
    CreateNewNode(AST_TAG_NIL);
}

void VisualizeVisitor::VisitTrue(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_TRUE);
    SaveOrphan();
    CreateNewNode(AST_TAG_TRUE);
}

void VisualizeVisitor::VisitFalse(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_FALSE);
    SaveOrphan();
    CreateNewNode(AST_TAG_FALSE);
}

void VisualizeVisitor::VisitEmptyIdlist(const Object &node) {
    SaveOrphan();
    CreateNewNode(AST_TAG_ID_LIST);
}

void VisualizeVisitor::VisitNormalIdlist(const Object &node) {
    CreateNewNode(AST_TAG_ID_LIST);
    LinkToPreviousNode();

    for (register unsigned i = 1; i < node.GetNumericSize(); ++i) LinkToOrphan();
}

void VisualizeVisitor::VisitIdList(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_ID_LIST);
    if (node.GetTotal() == 1)
        VisitEmptyIdlist(node);
    else
        VisitNormalIdlist(node);
}

void VisualizeVisitor::VisitFormal(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_FORMAL);
    SaveOrphan();
    CreateNewNode(node[AST_TAG_ID]->ToString());
}

void VisualizeVisitor::VisitIf(const Object &node) {
    CreateNewNode(AST_TAG_IF);
    LinkToPreviousNode();    // Stmt (either normal or else stmt)
    LinkToOrphan();          // Either expr or stmt
    if (node.ElementExists(AST_TAG_ELSE_STMT)) LinkToOrphan();
}

void VisualizeVisitor::VisitWhile(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_WHILE);
    CreateNewNode(AST_TAG_WHILE);
    LinkToPreviousNode();    // Stmt
    LinkToOrphan();          // Cond
}

void VisualizeVisitor::VisitFor(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_FOR);
    CreateNewNode(AST_TAG_FOR);
    LinkToPreviousNode();    // Stmt
    LinkToOrphan();          // Elist2
    LinkToOrphan();          // Cond
    LinkToOrphan();          // Elist1
}

void VisualizeVisitor::VisitReturn(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_RETURN);
    if (node.ElementExists(AST_TAG_CHILD)) {
        CreateNewNode(AST_TAG_RETURN);
        LinkToPreviousNode();
    } else {
        SaveOrphan();
        CreateNewNode(AST_TAG_RETURN);
    }
}

void VisualizeVisitor::VisitBreak(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_BREAK);
    SaveOrphan();
    CreateNewNode(AST_TAG_BREAK);
}

void VisualizeVisitor::VisitContinue(const Object &node) {
    eassert(node[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_CONTINUE);
    SaveOrphan();
    CreateNewNode(AST_TAG_CONTINUE);
}

TreeVisitor *VisualizeVisitor::Clone(void) const {
    auto visitor = new VisualizeVisitor();
    visitor->lastNode = lastNode;
    visitor->output << output.rdbuf();
    visitor->outputFile = outputFile;
    visitor->orphans = orphans;

    return visitor;
}

VisualizeVisitor::VisualizeVisitor(const std::string &filename) {
    lastNode = 0;
    outputFile = filename;
    output << "digraph AST { "
           << "graph [ "
           << "label = \"\n\n\n\nAlpha Abstract Syntax Tree\""
           << "];"
           << std::endl;
}

VisualizeVisitor::VisualizeVisitor(void) : VisualizeVisitor(DEFAULT_DOT_FILE) {}

VisualizeVisitor::~VisualizeVisitor() {
    lastNode = 0;
    output.clear();
    outputFile.clear();
    while (!orphans.empty()) orphans.pop();
}
