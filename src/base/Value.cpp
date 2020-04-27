#include "Value.h"
#include "Utilities.h"

#include <cassert>
#include <cstring> /* For memset and strdup */
#include <string>

Value::Data::Data() { this->Clear(); }

void Value::Data::Clear(void) {
    std::memset(this, 0, sizeof(Data));
    assert(this->stringVal == nullptr);
}

Value::Data::~Data() { this->Clear(); }

/****** Constructors ******/

Value::Value() {
    type = Type::UndefType;
    data.Clear();
}

Value::Value(Value && val) {
    assert(val.IsValid());

    this->type = val.type;
    if(val.type == Type::StringType) {
        data.stringVal = val.data.stringVal;
        val.data.stringVal = nullptr;
    } else if (val.type == Type::LibraryFunctionType) {
        data.libraryFunctionVal.id = val.data.libraryFunctionVal.id;
        val.data.libraryFunctionVal.id = nullptr;
    } else if (val.type == Type::NativePtrType) {
        data.nativePtrVal.typeId = val.data.nativePtrVal.typeId;
        val.data.nativePtrVal.typeId = nullptr;
    } else std::memcpy(&data, &val.data, sizeof(Data));

    val.type = Type::UndefType;
    val.data.Clear();
}

Value::Value(const Value &val) {
    assert(val.IsValid());

    this->type = val.type;
    if (val.type == Type::StringType)
        data.stringVal = strdup(val.data.stringVal);
    else if (val.type == Type::LibraryFunctionType) {
        data.libraryFunctionVal.function = val.data.libraryFunctionVal.function;
        data.libraryFunctionVal.id = strdup(val.data.libraryFunctionVal.id);
    }
    else if (val.type == Type::NativePtrType) {
        data.nativePtrVal.ptr = val.data.nativePtrVal.ptr;
        data.nativePtrVal.typeId = strdup(val.data.nativePtrVal.typeId);
    }
    else
        std::memcpy(&data, &val.data, sizeof(Data));

    assert(IsValid());
}

Value::Value(double num) : Value() { FromNumber(num); }

Value::Value(bool val) : Value() { FromBoolean(val); }

Value::Value(const char *str) : Value() { FromString(str); }

Value::Value(const std::string &str) : Value() { FromString(str); }

Value::Value(Object *obj, bool closure) : Value() { FromObject(obj, closure); }

Value::Value(Object *ast, Object *closure) : Value() { FromProgramFunction(ast, closure); }

Value::Value(LibraryFunc func, const std::string &str) : Value() { FromLibraryFunction(func, str); }

Value::Value(void *ptr, const std::string &_type) : Value() { FromNativePointer(ptr, _type); }

Value::Value(NilTypeValue) : Value() { FromNil(); }

/****** Operators ******/

const Value &Value::operator=(const Value &val) {
    assert(val.IsValid());

    if (this != &val) {
        this->~Value();
        new (this) Value(val);
        assert(IsValid());
    }
    return *this;
}

Value::operator bool() const {
    assert(IsValid());

    /* True test */
    if (type == Type::UndefType || type == Type::NilType) return false;
    else if (type == Type::ObjectType ||
             type == Type::LibraryFunctionType ||
             type == Type::ProgramFunctionType) return true;
    else if (type == Type::StringType) return data.stringVal[0] != '\0';
    else if (type == Type::NativePtrType) return data.nativePtrVal.ptr != nullptr;
    else if (type == Type::BooleanType) return ToBoolean();
    else if (type == Type::NumberType) return !Utilities::IsZero(data.numVal);
    else assert(false);
}

/****** Verifier ******/

bool Value::IsValid(void) const {
    return (
        type == Type::UndefType ||
        type == Type::NumberType ||
        type == Type::BooleanType ||
        (type == Type::StringType && data.stringVal) ||
        (type == Type::ObjectType && data.objectVal.object) ||
        (type == Type::ProgramFunctionType && data.programFunctionVal.ast && data.programFunctionVal.closure) ||
        (type == Type::LibraryFunctionType && data.libraryFunctionVal.function && data.libraryFunctionVal.id) ||
        (type == Type::NativePtrType && data.nativePtrVal.typeId) ||
        type == Type::NilType);
}

/****** Observers ******/

bool Value::IsUndef(void) const {
    assert(IsValid());
    return type == Type::UndefType;
}

bool Value::IsNumber(void) const {
    assert(IsValid());
    return type == Type::NumberType;
}

bool Value::IsBoolean(void) const {
    assert(IsValid());
    return type == Type::BooleanType;
}

bool Value::IsString(void) const {
    assert(IsValid());
    return type == Type::StringType;
}

bool Value::IsObject(void) const {
    assert(IsValid());
    return type == Type::ObjectType;
}

bool Value::IsProgramFunction(void) const {
    assert(IsValid());
    return type == Type::ProgramFunctionType;
}

bool Value::IsLibraryFunction(void) const {
    assert(IsValid());
    return type == Type::LibraryFunctionType;
}

bool Value::IsNativePtr(void) const {
    assert(IsValid());
    return type == Type::NativePtrType;
}

bool Value::IsNil(void) const {
    assert(IsValid());
    return type == Type::NilType;
}

/****** Setters ******/

void Value::FromUndef(void) {
    assert(IsValid());
    FreeMemory();

    type = Type::UndefType;
    data.Clear();

    assert(IsUndef());
    assert(data.stringVal == nullptr);
    assert(IsValid());
}

void Value::FromNumber(double num) {
    assert(IsValid());
    FreeMemory();

    type = Type::NumberType;
    data.numVal = num;

    assert(IsNumber());
    assert(IsValid());
}

void Value::FromBoolean(bool val) {
    assert(IsValid());
    FreeMemory();

    type = Type::BooleanType;
    data.boolVal = val;

    assert(IsBoolean());
    assert(IsValid());
}

void Value::FromString(const std::string &str) {
    assert(IsValid());
    FreeMemory();

    type = Type::StringType;
    data.stringVal = strdup(str.c_str());

    assert(IsString());
    assert(IsValid());
}

void Value::FromObject(Object *obj, bool closure) {
    assert(obj);
    assert(IsValid());
    FreeMemory();

    type = Type::ObjectType;
    data.objectVal.object = obj;
    data.objectVal.isClosure = closure;

    assert(IsObject());
    assert(IsValid());
}

void Value::FromProgramFunction(Object *ast, Object *closure) {
    assert(ast);
    assert(closure);
    assert(IsValid());
    FreeMemory();

    type = Type::ProgramFunctionType;
    data.programFunctionVal.ast = ast;
    data.programFunctionVal.closure = closure;

    assert(IsProgramFunction());
    assert(IsValid());
}

void Value::FromLibraryFunction(LibraryFunc func, const std::string &str) {
    assert(func);
    assert(!str.empty());
    assert(IsValid());
    FreeMemory();

    type = Type::LibraryFunctionType;
    data.libraryFunctionVal.function = func;
    data.libraryFunctionVal.id = strdup(str.c_str());

    assert(IsLibraryFunction());
    assert(IsValid());
}

void Value::FromNativePointer(void *ptr, const std::string &str) {
    assert(!str.empty());
    assert(IsValid());
    FreeMemory();

    type = Type::NativePtrType;
    data.nativePtrVal.ptr = ptr;
    data.nativePtrVal.typeId = strdup(str.c_str());

    assert(IsNativePtr());
    assert(IsValid());
}

void Value::FromNil(void) {
    assert(IsValid());
    FreeMemory();

    type = Type::NilType;
    data.Clear();

    assert(IsNil());
    assert(data.stringVal == nullptr);
    assert(IsValid());
}

/****** Getters ******/

Value::Type Value::GetType(void) const {
    return type;
}

std::string Value::GetTypeToString(void) const {
    switch(type) {
        case Type::UndefType: return "undef";
        case Type::NumberType: return "number";
        case Type::BooleanType: return "boolean";
        case Type::StringType: return "string";
        case Type::ObjectType: return "object";
        case Type::ProgramFunctionType: return "program function";
        case Type::LibraryFunctionType: return "library function";
        case Type::NativePtrType: return "native pointer";
        case Type::NilType: return "nil";
        default: assert(false);
    }
}

double Value::ToNumber(void) const {
    assert(IsNumber());
    return data.numVal;
}

bool Value::ToBoolean(void) const {
    assert(IsBoolean());
    return data.boolVal;
}

std::string Value::ToString(void) const {
    assert(IsString());
    return std::string(data.stringVal);
}

const Object *Value::ToObject(void) const {
    assert(IsObject());
    return data.objectVal.object;
}

const Object *Value::ToProgramFunctionAST(void) const {
    assert(IsProgramFunction());
    return data.programFunctionVal.ast;
}

const Object *Value::ToProgramFunctionClosure(void) const {
    assert(IsProgramFunction());
    return data.programFunctionVal.closure;
}

Object *Value::ToObject_NoConst(void) const {
    return const_cast<Object *>(ToObject());
}

Object *Value::ToProgramFunctionAST_NoConst(void) const {
    return const_cast<Object *>(ToProgramFunctionAST());
}

Object *Value::ToProgramFunctionClosure_NoConst(void) const {
    return const_cast<Object *>(ToProgramFunctionClosure());
}

LibraryFunc Value::ToLibraryFunction(void) const {
    assert(IsLibraryFunction());
    return data.libraryFunctionVal.function;
}

std::string Value::ToLibraryFunctionId(void) const {
    assert(IsLibraryFunction());
    return std::string(data.libraryFunctionVal.id);
}

void *Value::ToNativePtr(void) const {
    assert(IsNativePtr());
    return data.nativePtrVal.ptr;
}

std::string Value::ToNativeTypeId(void) const {
    assert(IsNativePtr());
    return std::string(data.nativePtrVal.typeId);
}

bool Value::IsObjectClosure(void) const {
    assert(IsObject());
    return data.objectVal.isClosure;
}

/****** Replicator ******/

Value *Value::Clone(void) const {
    Value *val = new Value(*this);
    return val;
}

/****** Destructor ******/

Value::~Value() {
    assert(IsValid());

    FromUndef();
}

/****** Modifier ******/

void Value::FreeMemory(void) {
    assert(IsValid());

    if (type == Type::StringType) {
        free(data.stringVal);
        data.stringVal = nullptr;
    }
    else if (type == Type::LibraryFunctionType) {
        free(data.libraryFunctionVal.id);
        data.libraryFunctionVal.id = nullptr;
    }
    else if (type == Type::NativePtrType) {
        free(data.nativePtrVal.typeId);
        data.nativePtrVal.typeId = nullptr;
    }

    /* Note that free memory violates the invariant of the object. This means
     * that after this method, IsValid() will always fail because the strings
     * are set to NULL (to prevent bugs). This is an acceptable behavior because
     * private methods can violate invariants for a short period */
}