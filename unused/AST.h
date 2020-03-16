#ifndef _AST_H_
#define _AST_H_

#include "Object.h"

#include <functional>

/* TODO: Is this a good idea? Perhaps an interface is more appropriate */
typedef std::function<void(Object *)> VisitorFunction;

class AST {
private:
    Object * root;

    unsigned nodesCount;

public:

    AST(void);

    bool IsValid(void) const;

    unsigned GetNodesCount(void) const;

    Object * GetRoot(void) const;

    void CreateEmptyNode(void);

    void CreateNode(const std::string & type, const Value & val);

    void ExtendNode(const std::string & rel, Object * ptr);

    void PrependNode(const std::string & type, const std::string & rel);

    void MergeTrees(const std::string & rel, const AST & ast);

    void AcceptVisitor(VisitorFunction);
};

#endif