#include "TreeTags.h"
#include "Value.h"
#include "Object.h"
#include "AST.h"

#include <iostream>

int main(void) {
    std::cout << "Hello world!" << std::endl;

    Object numconst;
    numconst.SetValue("type", TYPE_NUMCONST);
    numconst.SetValue("value", 23.0f);

    Object term;
    term.SetValue("type", TYPE_TERM);
    term.SetValue("value", &numconst);

    Object rvalue;
    rvalue.SetValue("type", TYPE_RVALUE);
    rvalue.SetValue("value", &term);

    //std::cout << rvalue["value"].GetObject()["value"].GetObject()["value"].ToString() << std::endl;;

    AST leftSubTree;
    leftSubTree.CreateEmptyNode();
    leftSubTree.GetRoot()->SetValue("type", "var");
    leftSubTree.GetRoot()->SetValue("id", "x");

    AST ast;
    ast.CreateNode(TYPE_NUMCONST, 23.0f);
    ast.PrependNode(TYPE_TERM, "child");
    ast.PrependNode(TYPE_ASSIGNEXPR, "rvalue");
    ast.MergeTrees("lvalue", leftSubTree);
    ast.PrependNode(TYPE_STMT, "child");

    std::cout << ast.GetNodesCount() << std::endl;

    ast.AcceptVisitor([](Object * obj) {
        std::cout << "I am traversing the tree and I found a node of type " << obj->operator[]("type").GetString() << std::endl;
    });

    return EXIT_SUCCESS;
}