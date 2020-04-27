#ifndef _WRITE_FIELD_H_
#define _WRITE_FIELD_H_

#include "Object.h"

#include <string>
#include <cassert>

#define SYMBOL_IS_TABLE true
#define SYMBOL_IS_VAR false
#define SYMBOL_IS_CLOSURE_CHANGE true

class Symbol {
private:
    Object * context = nullptr;
    std::string strVal;
    double numVal;

    bool isIndexNumber = false;
    bool isIndexString = false;
    bool isContextTable = false;
    bool isClosureChange = false;

public:

    Symbol(Object * _context, const std::string & str, bool isTable = false, bool closureChange = false) : strVal(str) {
        assert(_context);
        assert(!str.empty());

        context = _context;
        isIndexString = true;
        isIndexNumber = false;
        isContextTable = isTable;
        isClosureChange = closureChange;
    }

    Symbol(Object * _context, double num, bool isTable = false, bool closureChange = false) : numVal(num) {
        assert(_context);

        context = _context;
        isIndexString = false;
        isIndexNumber = true;
        isContextTable = isTable;
        isClosureChange = closureChange;
    }

    bool IsValid(void) const {
        return (context &&
                (!isIndexString || !strVal.empty()) &&
                (!isIndexString || !isIndexNumber) &&
                (isIndexString || isIndexNumber));
    }

    bool IsIndexString(void)   const { return isIndexString; }
    bool IsIndexNumber(void)   const { return isIndexNumber; }
    bool IsContextTable(void)  const { return isContextTable; }
    bool IsClosureChange(void) const { return isClosureChange; }

    double ToNumber(void) const {
        assert(IsIndexNumber());
        return numVal;
    }

    std::string ToString(void) const {
        assert(IsIndexString());
        return strVal;
    }

    Object * GetContext(void) const {
        return context;
    }

    void SetIsClosureChange(bool value) { isClosureChange = value; }
};

#endif