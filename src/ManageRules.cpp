#include "ManageRules.h"
#include "AST.h"
#include "TreeTags.h"

#include <iostream>

AST * ManageRules::ManageConstNumber(double value) {
    AST * ast = new AST();
    ast->CreateNode(TYPE_NUMCONST, value);
    return ast;
}

AST * ManageRules::ManagePrimary(AST * ast) {
    ast->PrependNode(TYPE_PRIMARY, "child");
    return ast;
}

AST * ManageRules::ManageTerm(AST * ast) {
    ast->PrependNode(TYPE_TERM, "child");
    return ast;
}

AST * ManageRules::ManageExpr(AST * ast) {
    ast->PrependNode(TYPE_EXPR, "child");
    return ast;
}

AST * ManageRules::ManageExprPlusExpr(AST * left, AST * right) {
    right->PrependNode(TYPE_EXPR, "right");
    right->MergeTrees("left", *left);
    return right;
}

AST * ManageRules::ManageStmt(AST * ast) {
    ast->PrependNode(TYPE_STMT, "child");
    return ast;
}