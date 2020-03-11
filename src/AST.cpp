#include "AST.h"

#include <cassert>

AST::AST(void) {
    root = nullptr;
    nodesCount = 0;
}

bool AST::IsValid(void) const {
    return true;
}

unsigned AST::GetNodesCount(void) const {
    assert(IsValid());
    return nodesCount;
}

Object * AST::GetRoot(void) const {
    return root;
}

void AST::CreateEmptyNode(void) {
    Object * ptr = new Object();
    root = ptr;
    nodesCount = 1;
}

void AST::CreateNode(const std::string & type, const Value & val) {
    Object * ptr = new Object();
    ptr->SetValue("type", type);
    ptr->SetValue("value", val);

    if (!root) root = ptr;

    nodesCount = 1;
}

void AST::ExtendNode(const std::string & rel, Object * ptr) {
    assert(!rel.empty());
    assert(ptr);
    assert(root);

    root->SetValue(rel, ptr);
    nodesCount += 1;
}

void AST::PrependNode(const std::string & type, const std::string & rel) {
    assert(!type.empty());
    assert(!rel.empty());
    assert(root);

    Object * ptr = new Object();
    ptr->SetValue("type", type);
    ptr->SetValue(rel, root);

    root = ptr;
    nodesCount += 1;
}

void AST::MergeTrees(const std::string & rel, const AST & ast) {
    assert(!rel.empty());

    if (!root) {
        root = ast.root;
        nodesCount = ast.nodesCount;
    }
    else {
        ExtendNode(rel, ast.root);
        nodesCount += ast.nodesCount - 1;
    }
}

void TraverseTree(Object * object, VisitorFunction func) {
    if (!object) return;

    /* TODO: Please change this. It is a really bad implementation AND idea */
    if(object->ElementExists("child") && object->operator[]("child").IsObject() ) TraverseTree((*object)["child"].GetObject(), func);
    if(object->ElementExists("lvalue") && object->operator[]("lvalue").IsObject() ) TraverseTree((*object)["lvalue"].GetObject(), func);
    if(object->ElementExists("rvalue") && object->operator[]("rvalue").IsObject() ) TraverseTree((*object)["rvalue"].GetObject(), func);

    func(object);
}

void AST::AcceptVisitor(VisitorFunction func) {
    TraverseTree(root, func);
}