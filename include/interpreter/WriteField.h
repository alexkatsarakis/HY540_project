#ifndef _WRITE_FIELD_H_
#define _WRITE_FIELD_H_

#include <string>
#include <cassert>

class WriteField {
private:
    std::string strVal;
    double numVal;

    bool isNumber = false;
    bool isString = false;

public:

    WriteField(const std::string & str) : strVal(str) {
        isString = true;
        isNumber = false;
    }

    WriteField(double num) : numVal(num) {
        isNumber = true;
        isString = false;
    }

    bool IsString(void) const { return isString; }
    bool IsNumber(void) const { return isNumber; }

    double ToNumber(void) const {
        assert(IsNumber());
        return numVal;
    }

    std::string ToString(void) const {
        assert(IsString());
        return strVal;
    }
};

using Symbol = std::pair<Object *, WriteField>;

#endif