#ifndef _VALUE_H_
#define _VALUE_H_

#include <string>
#include <functional>

class Object;

/* Used only to indicate that we want to create a Value objec with Nil type */
enum class NilTypeValue { Nil };

/* TODISCUSS: This could be an std::function but we should use new and delete
 * all the time */
typedef void (*LibraryFunc)(Object &);

class Value {

public:
    enum class Type {
        UndefType,
        NumberType,
        BooleanType,
        StringType,
        ObjectType,
        ProgramFunctionType,
        LibraryFunctionType,
        NativePtrType,
        NilType
    };

    /****** Constructors ******/

    Value();

    Value(const Value & val);

    Value(double num);

    Value(bool val);

    Value(const char * str);

    Value(const std::string & str);

    Value(Object * obj);

    Value(Object * ast, Object * closure); // 2 Object ptrs mean program func

    Value(LibraryFunc func, const std::string & str = "");

    Value(void * ptr, const std::string & type);

    Value(NilTypeValue);

    /****** Operators ******/

    const Value & operator = (const Value & val);

    /****** Verifier ******/

    bool IsValid(void) const;

    /****** Observers ******/

    bool IsUndef(void) const;

    bool IsNumber(void) const;

    bool IsBoolean(void) const;

    bool IsString(void) const;

    bool IsObject(void) const;

    bool IsProgramFunction(void) const;

    bool IsLibraryFunction(void) const;

    bool IsNativePtr(void) const;

    bool IsNil(void) const;

    /****** Setters ******/

    void FromUndef(void);

    void FromNumber(double num);

    void FromBoolean(bool val);

    void FromString(const std::string & str);

    void FromObject(Object * obj);

    void FromProgramFunction(Object * ast, Object * closure);

    void FromLibraryFunction(LibraryFunc func, const std::string & str);

    void FromNativePointer(void * ptr, const std::string & str);

    void FromNil(void);

    /****** Getters ******/

    Type GetType(void) const;

    double ToNumber(void) const;

    bool ToBoolean(void) const;

    std::string ToString(void) const;

    const Object * ToObject(void) const;

    const Object * ToProgramFunctionAST(void) const;

    const Object * ToProgramFunctionClosure(void) const;

    LibraryFunc ToLibraryFunction(void) const;

    std::string ToLibraryFunctionId(void) const;

    void * ToNativePtr(void) const;

    std::string ToNativeTypeId(void) const;

    /****** Replicator ******/

    Value * Clone(void) const;

    /****** Destructor ******/

    virtual ~Value();

private:

    union Data {
        double numVal;
        bool boolVal;
        char * stringVal;
        Object * objectVal;

        struct {
            Object * ast;
            Object * closure;
        } programFunctionVal;

        struct {
            LibraryFunc function;
            char * id;
        } libraryFunctionVal;

        struct {
            void * ptr;
            char * typeId;
        } nativePtrVal;

        Data();

        void Clear(void);

        ~Data();
    };

    Type type;
    Data data;
};

#endif