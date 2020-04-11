#ifndef _VALUE_STACK_H_
#define _VALUE_STACK_H_

#include "Value.h"

#include <list>

class ValueStack {

protected:
    std::list<Value> stack;

public:
    bool IsValid(void) const;

    bool IsEmpty(void) const;

    void Clear(void);

    void Push(const Value & v);

    void Pop(void);

    const Value & Top(void) const;

    const Value GetTopAndPop(void);

    ValueStack(void)=default;

    ValueStack(const ValueStack &)=delete;

    ValueStack(const ValueStack &&)=delete;
};

#endif