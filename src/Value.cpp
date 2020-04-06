#include "Value.h"

#include <cassert>
#include <cstring> /* For memset and strdup */
#include <string>

Value::Data::Data() { this->Clear(); }

void Value::Data::Clear(void) {
    std::memset(this, 0, sizeof(Data));
    assert(this->objectVal == nullptr);
}

Value::Data::~Data() { this->Clear(); }

/****** Constructors ******/

Value::Value() { FromUndef(); }

Value::Value(const Value &val) {
    assert(val.IsValid());

    this->type = val.type;
    if (val.type == Type::StringType)
        data.stringVal = strdup(val.data.stringVal);
    else if (val.type == Type::LibraryFunctionType)
        data.libraryFunctionVal.id = strdup(val.data.libraryFunctionVal.id);
    else if (val.type == Type::NativePtrType)
        data.nativePtrVal.typeId = strdup(val.data.nativePtrVal.typeId);
    else
        std::memcpy(&data, &val.data, sizeof(Data));

    assert(IsValid());
}

Value::Value(double num) { FromNumber(num); }

Value::Value(bool val) { FromBoolean(val); }

Value::Value(const char *str) { FromString(str); }

Value::Value(const std::string &str) { FromString(str); }

Value::Value(Object *obj) { FromObject(obj); }

Value::Value(Object *ast, Object *closure) { FromProgramFunction(ast, closure); }

Value::Value(LibraryFunc func, const std::string &str) { FromLibraryFunction(func, str); }

Value::Value(void *ptr, const std::string &_type) { FromNativePointer(ptr, _type); }

Value::Value(NilTypeValue) { FromNil(); }

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

/****** Verifier ******/

bool Value::IsValid(void) const {
    return (
        type == Type::UndefType ||
        type == Type::NumberType ||
        type == Type::BooleanType ||
        (type == Type::StringType && data.stringVal) ||
        (type == Type::ObjectType && data.objectVal) ||
        (type == Type::ProgramFunctionType && data.programFunctionVal.ast) ||
        (type == Type::LibraryFunctionType && data.libraryFunctionVal.function && data.libraryFunctionVal.id) ||
        (type == Type::NativePtrType && data.nativePtrVal.ptr && data.nativePtrVal.typeId) ||
        type == Type::NilType);
}

/****** Observers ******/

bool Value::IsUndef(void) const { return type == Type::UndefType; }

bool Value::IsNumber(void) const { return type == Type::NumberType; }

bool Value::IsBoolean(void) const { return type == Type::BooleanType; }

bool Value::IsString(void) const { return type == Type::StringType; }

bool Value::IsObject(void) const { return type == Type::ObjectType; }

bool Value::IsProgramFunction(void) const { return type == Type::ProgramFunctionType; }

bool Value::IsLibraryFunction(void) const { return type == Type::LibraryFunctionType; }

bool Value::IsNativePtr(void) const { return type == Type::NativePtrType; }

bool Value::IsNil(void) const { return type == Type::NilType; }

/****** Setters ******/

void Value::FromUndef(void) {
    type = Type::UndefType;
    data.Clear();

    assert(IsUndef());
    assert(data.objectVal == nullptr);
    assert(IsValid());
}

void Value::FromNumber(double num) {
    type = Type::NumberType;
    data.numVal = num;

    assert(IsNumber());
    assert(IsValid());
}

void Value::FromBoolean(bool val) {
    type = Type::BooleanType;
    data.boolVal = val;

    assert(IsBoolean());
    assert(IsValid());
}

void Value::FromString(const std::string &str) {
    type = Type::StringType;
    data.stringVal = strdup(str.c_str());

    assert(IsString());
    assert(IsValid());
}

void Value::FromObject(Object *obj) {
    assert(obj);

    type = Type::ObjectType;
    data.objectVal = obj;

    assert(IsObject());
    assert(IsValid());
}

void Value::FromProgramFunction(Object *ast, Object *closure) {
    assert(ast);

    type = Type::ProgramFunctionType;
    data.programFunctionVal.ast = ast;
    data.programFunctionVal.closure = closure;

    assert(IsProgramFunction());
    assert(IsValid());
}

void Value::FromLibraryFunction(LibraryFunc func, const std::string &str) {
    assert(!str.empty());
    assert(func);

    type = Type::LibraryFunctionType;
    data.libraryFunctionVal.function = func;
    data.libraryFunctionVal.id = strdup(str.c_str());

    assert(IsLibraryFunction());
    assert(IsValid());
}

void Value::FromNativePointer(void *ptr, const std::string &str) {
    assert(ptr);
    assert(!str.empty());

    type = Type::NativePtrType;
    data.nativePtrVal.ptr = ptr;
    data.nativePtrVal.typeId = strdup(str.c_str());

    assert(IsNativePtr());
    assert(IsValid());
}

void Value::FromNil(void) {
    type = Type::NilType;
    data.Clear();

    assert(IsNil());
    assert(data.objectVal == nullptr);
    assert(IsValid());
}

/****** Getters ******/

Value::Type Value::GetType(void) const {
    return type;
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
    return data.objectVal;
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

/****** Replicator ******/

Value *Value::Clone(void) const {
    Value *val = new Value(*this);
    return val;
}

/****** Destructor ******/

Value::~Value() {
    assert(IsValid());

    if (type == Type::StringType)
        free(data.stringVal);
    else if (type == Type::LibraryFunctionType)
        free(data.libraryFunctionVal.id);
    else if (type == Type::NativePtrType)
        free(data.nativePtrVal.typeId);

    FromUndef();
}
