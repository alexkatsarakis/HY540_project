#include "ValueStack.h"

#include <cassert>
#include <utility>  /* For std::move */

bool ValueStack::IsValid(void) const {
    for(auto v : stack) if (!v.IsValid()) return false;
    return true;
}

bool ValueStack::IsEmpty(void) const {
    return stack.empty();
}

void ValueStack::Clear(void) {
    assert(IsValid());
    stack.clear();
}

void ValueStack::Push(const Value & v) {
    assert(v.IsValid());
    assert(IsValid());

    stack.push_front(v);
}

void ValueStack::Pop(void) {
    assert(IsValid());
    assert(!IsEmpty());
    stack.pop_front();
}

const Value & ValueStack::Top(void) const {
    assert(IsValid());
    assert(!IsEmpty());

    return stack.front();
}

const Value ValueStack::GetTopAndPop(void) {
    assert(IsValid());
    assert(!IsEmpty());

    Value v = stack.front();
    stack.pop_front();
    return std::move(v);
}