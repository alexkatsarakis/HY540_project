#ifndef _VALUE_H_
#define _VALUE_H_

#include <string>

class Object;

class Value {

private:
    enum class ValueType {
        TypeNumber,
        TypeString,
        TypeBool,
        TypeObject,
        TypeNativePtr,
        TypeUserFunc,
        TypeLibFunc,
        TypeNil,
        TypeUndef
    };

    union ValueData {
        double numVal;
        std::string stringVal;
        bool boolVal;
        Object * objectVal;
        void * nativePointerVal;
        unsigned userFuncVal;
        std::string libFuncVal;

        ValueData();

        void Clear(void);

        ~ValueData();
    };

    ValueType type;
    ValueData data;

public:
    Value();

    Value(const Value & val);

    const Value & operator = (const Value & val);

    Value(double num);

    Value(const char * str);

    Value(const std::string & str);

    Value(bool val);

    Value(Object * obj);

    Value(void * ptr);

    bool IsValid(void) const;

    bool IsNumber(void) const;

    bool IsString(void) const;

    bool IsBool(void) const;

    bool IsObject(void) const;

    bool IsNativePointer(void) const;

    bool IsUserFunc(void) const;

    bool IsLibFunc(void) const;

    bool IsNil(void) const;

    bool IsUndef(void) const;

    void FromNumber(double num);

    void FromString(const std::string & str);

    void FromBool(bool val);

    void FromObject(Object * obj);

    void FromNativePointer(void * ptr);

    void FromUserFunc(unsigned index);

    void FromLibFunc(const std::string & str);

    void FromNil(void);

    void FromUndef(void);

    std::string ToString(void) const;

    double GetNumber(void) const;

    std::string GetString(void) const;

    bool GetBool(void) const;

    Object * GetObject(void) const;

    void * GetNativePointer(void) const;

    unsigned GetUserFunc(void) const;

    std::string GetLibFunc(void) const;

    virtual ~Value();
};

#endif