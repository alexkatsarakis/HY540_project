#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "Value.h"

#include <string>
#include <map>
#include <functional>

typedef Value * ValuePtr; /* Use typedef so we can use shared_ptr in the future */
typedef std::map<double, ValuePtr>      NumbericValueMap;
typedef std::map<std::string, ValuePtr> StringValueMap;

class Object {
private:
    unsigned refCounter;
    NumbericValueMap numMap;
    StringValueMap   strMap;

public:

    using Visitor = std::function<void(const Value & key, const Value & val)>;
    using Applier = std::function<void(const Value & key, Value & val)>;

    /****** Constructors ******/

    Object();

    Object (const Object & obj);

    /****** Verifier ******/

    bool IsValid(void) const;

    /****** Observers ******/

    bool ElementExists(double key) const;

    bool ElementExists(const std::string & key) const;

    unsigned GetTotal(void) const;

    unsigned GetNumericSize(void) const;

    unsigned GetStringSize(void) const;

    /****** Modifiers ******/

    void IncreaseRefCounter(void);

    void DecreaseRefCounter(void);

    /****** Getters ******/

    const Value * operator[] (double key) const;

    const Value * operator[] (const std::string & key) const ;

    /****** Setters ******/

    void Set(double key, const Value & value);

    void Set(const std::string & key, const Value & value);

    void Remove(double key);

    void Remove(const std::string & key);

    /****** Visitors ******/

    void Visit(const Visitor & func) const;  /* Read-Only access */

    void Apply(const Applier & func); /* Read-Write access */

    /****** Destructor ******/

    virtual ~Object();
};

#endif