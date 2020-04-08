#include "EvalDispatcher.h"
#include "TreeTags.h"

#include <cassert>

bool EvalDispatcher::IsValid(void) const {
    for(auto pair : evals) if (pair.first.empty() || !pair.second) return false;
    return true;
}

void EvalDispatcher::Install(const std::string & tag, const Func & f) {
    assert(!tag.empty());
    assert(f);

    evals[tag] = f;
}

void EvalDispatcher::Clear(void) {
    evals.clear();
}

const Value EvalDispatcher::Eval(Object & node) {
    assert(node.IsValid());

    return evals[node[AST_TAG_TYPE_KEY]->ToString()](node);
}

EvalDispatcher::~EvalDispatcher() { }