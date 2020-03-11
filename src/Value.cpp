#include "Value.h"

#include <string>
#include <cassert>
#include <cstring> /* For memset*/
#include <sstream>

Value::ValueData::ValueData() { this->Clear(); }

void Value::ValueData::Clear(void) {
    std::memset(this, 0, sizeof(ValueData));
    assert(this->nativePointerVal == nullptr);
}

Value::ValueData::~ValueData() {}

Value::Value() { }

Value::Value(const Value & val) {
    this->type = val.type;
    /* TODO: Is this the correct way to do it ? */
    std::memcpy(&data, &val.data, sizeof(ValueData));
    assert(this->data.nativePointerVal == val.data.nativePointerVal);
    assert(IsValid());
}

const Value & Value::operator = (const Value & val) {
    if (this != &val) {
        this->~Value();
        new (this) Value(val);
        assert(IsValid());
    }
    return *this;
}

Value::Value(double num) { FromNumber(num); }

Value::Value(const char * str) { FromString(str); }

Value::Value(const std::string & str) { FromString(str); }

Value::Value(bool val) { FromBool(val); }

Value::Value(Object * obj) { FromObject(obj); }

Value::Value(void * ptr) { FromNativePointer(ptr); }

bool Value::IsValid(void) const {
    return (
        type == ValueType::TypeNumber ||
        type == ValueType::TypeString ||
        type == ValueType::TypeBool ||
        type == ValueType::TypeObject ||
        type == ValueType::TypeNativePtr ||
        type == ValueType::TypeUserFunc ||
        type == ValueType::TypeLibFunc ||
        type == ValueType::TypeNil ||
        type == ValueType::TypeUndef
    );
}

bool Value::IsNumber(void) const { return type == Value::ValueType::TypeNumber; }

bool Value::IsString(void) const { return type == Value::ValueType::TypeString; }

bool Value::IsBool(void) const { return type == Value::ValueType::TypeBool; }

bool Value::IsObject(void) const { return type == Value::ValueType::TypeObject; }

bool Value::IsNativePointer(void) const { return type == Value::ValueType::TypeNativePtr; }

bool Value::IsUserFunc(void) const { return type == Value::ValueType::TypeUserFunc; }

bool Value::IsLibFunc(void) const { return type == Value::ValueType::TypeLibFunc; }

bool Value::IsNil(void) const { return type == Value::ValueType::TypeNil; }

bool Value::IsUndef(void) const { return type == Value::ValueType::TypeUndef; }

void Value::FromNumber(double num) {
    type = Value::ValueType::TypeNumber;
    data.numVal = num;
    assert(IsNumber());
    assert(IsValid());
}

void Value::FromString(const std::string & str) {
    type = Value::ValueType::TypeString;
    data.stringVal = str;
    assert(IsString());
    assert(IsValid());
}

void Value::FromBool(bool val) {
    type = Value::ValueType::TypeBool;
    data.boolVal = val;
    assert(IsBool());
    assert(IsValid());
}

void Value::FromObject(Object * obj) {
    assert(obj);
    type = Value::ValueType::TypeObject;
    data.objectVal = obj;
    assert(IsObject());
    assert(IsValid());
}

void Value::FromNativePointer(void * ptr) {
    assert(ptr);
    type = Value::ValueType::TypeNativePtr;
    data.nativePointerVal = ptr;
    assert(IsNativePointer());
    assert(IsValid());
}

void Value::FromUserFunc(unsigned index) {
    type = Value::ValueType::TypeUserFunc;
    data.userFuncVal = index;
    assert(IsUserFunc());
    assert(IsValid());
}

void Value::FromLibFunc(const std::string & str) {
    assert(!str.empty());
    type = Value::ValueType::TypeString;
    data.libFuncVal = str;
    assert(IsLibFunc());
    assert(IsValid());
}

void Value::FromNil(void) {
    type = Value::ValueType::TypeNil;
    data.Clear();
    assert(IsNil());
    assert(data.nativePointerVal == nullptr);
    assert(IsValid());
}

void Value::FromUndef(void) {
    type = Value::ValueType::TypeUndef;
    data.Clear();
    assert(IsUndef());
    assert(data.nativePointerVal == nullptr);
    assert(IsValid());
}

std::string Value::ToString(void) const {
    std::stringstream ss;
    ss << "[ ";

    /* TODO: There has to be a better way to do this! */
    if (type == ValueType::TypeNumber) { ss << "Number ] [ " << std::to_string(data.numVal); }
    else if (type == ValueType::TypeString) { ss << "String ] [ \"" << data.stringVal << '"'; }
    else if (type == ValueType::TypeBool) { ss << "Bool ] [ " << std::to_string(data.boolVal); }
    else if (type == ValueType::TypeObject) ss << "Object";
    else if (type == ValueType::TypeNativePtr) ss << "NativePointer";
    else if (type == ValueType::TypeUserFunc) ss << "UserFunction";
    else if (type == ValueType::TypeLibFunc) ss << "LibFunction";
    else if (type == ValueType::TypeNil) ss << "Nil";
    else if (type == ValueType::TypeUndef) ss << "Undef";
    else assert(false);

    ss << " ]";
    return ss.str();
}

double Value::GetNumber(void) const {
    assert(IsNumber());
    return data.numVal;
}

std::string Value::GetString(void) const {
    assert(IsString());
    return data.stringVal;
}

bool Value::GetBool(void) const {
    assert(IsBool());
    return data.boolVal;
}

Object * Value::GetObject(void) const {
    assert(IsObject());
    return data.objectVal;
}

void * Value::GetNativePointer(void) const {
    assert(IsNativePointer());
    return data.nativePointerVal;
}

unsigned Value::GetUserFunc(void) const {
    assert(IsUserFunc());
    return data.userFuncVal;
}

std::string Value::GetLibFunc(void) const {
    assert(IsLibFunc());
    return data.libFuncVal;
}

Value::~Value() { }