#ifndef _EVAL_DISPATCHER_H_
#define _EVAL_DISPATCHER_H_

#include "Value.h"
#include "Object.h"

#include <string>
#include <functional>

class EvalDispatcher {

public:
    using Func = std::function<const Value(Object &)>;

    bool IsValid(void) const;

    void Install(const std::string & tag, const Func & f);

    void Clear(void);

    const Value Eval(Object & obj);

    EvalDispatcher(void)=default;
    EvalDispatcher(const EvalDispatcher &)=delete;
    EvalDispatcher(EvalDispatcher&&)=delete;

    virtual ~EvalDispatcher();

private:

    std::map<std::string, Func> evals;
};

#endif