#include "SetParentTreeVisitor.h"

#include <memory>    //addressof is C-equivalent '&' operator, returns a pointer

#define PARENT_FIELD "$$Parent"
using namespace std;

void SetParentTreeVisitor::SetParent(const Object &parent) {
    Object &parentNoConst = const_cast<Object &>(parent);
    if (isRemoval) {
        parentNoConst.Remove(PARENT_FIELD);
        return;
    }
    while (!valueStack.IsEmpty())
        valueStack.GetTopAndPop().ToObject_NoConst()->Set(PARENT_FIELD, Value(addressof(parentNoConst)));
    valueStack.Push(Value(addressof(parentNoConst)));
}

TreeVisitor *SetParentTreeVisitor::Clone(void) const {
    SetParentTreeVisitor *clone = new SetParentTreeVisitor(this->isRemoval);
    return clone;
}
