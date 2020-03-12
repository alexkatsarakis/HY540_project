#ifndef _MANAGE_RULES_H_
#define _MANAGE_RULES_H_

#include "AST.h"

class ManageRules {
private:
    AST ast;

public:
    AST * ManageConstNumber(double value);

    AST * ManagePrimary(AST * ast);

    AST * ManageTerm(AST * ast);

    AST * ManageExpr(AST * ast);

    AST * ManageExprPlusExpr(AST * left, AST * right);

    AST * ManageStmt(AST * ast);
};

#endif