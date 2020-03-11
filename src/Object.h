#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "Value.h"

#include <string>
#include <vector>
#include <map>
#include <memory>

typedef std::shared_ptr<Value> ValuePtr;
typedef std::map<unsigned, ValuePtr> UnsignedValueMap;
typedef std::map<std::string, ValuePtr> StringValueMap;

class Object {
private:
    unsigned refCounter;
    UnsignedValueMap numHash;
    StringValueMap stringHash;

public:

    Object();

    bool IsValid(void) const;

    void IncreaseRefCounter(void);

    void DecreaseRefCounter(void);

    bool ElementExists(unsigned index) const;

    bool ElementExists(const std::string & str) const;

    const Value & operator[] (unsigned index) const;

    const Value & operator[] (const std::string & str) const ;

    void SetValue(unsigned index, const Value & val);

    void SetValue(const std::string & str, const Value & val);

    virtual ~Object();
};

#endif