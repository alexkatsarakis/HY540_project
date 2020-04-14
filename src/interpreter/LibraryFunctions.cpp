#include "Interpreter.h"
#include "LibraryFunctions.h"
#include "TreeTags.h"
#include "Utilities.h"

#include <iostream>
#include <cassert>
#include <ostream>
#include <iomanip>

unsigned whitespace = 0;

#define WHITESPACE_STEP 4

void PrintNumber(std::ostream & stream, const Value * value);
void PrintString(std::ostream & stream, const Value * value);
void PrintBoolean(std::ostream & stream, const Value * value);
void PrintUndef(std::ostream & stream, const Value * value);
void PrintNil(std::ostream & stream, const Value * value);
void PrintLibraryFunction(std::ostream & stream, const Value * value);
void PrintNativePointer(std::ostream & stream, const Value * value);
void PrintProgramFunction(std::ostream & stream, const Value * value);
void PrintObject(std::ostream & stream, const Value * value);
void PrintValue(std::ostream & stream, const Value * value);

const Value * GetArgument(Object & env, unsigned argNo, const std::string & optArgName = "") {
    assert(env.IsValid());

    if (optArgName.empty()) return env[argNo];

    auto arg = env[optArgName];
    if (!arg) arg = env[argNo];

    return arg;
}

void PrintNumber(std::ostream & stream, const Value * value) {
    assert(stream.good());
    assert(value && value->IsNumber());
    stream << (Utilities::IsInt(value->ToNumber()) ? static_cast<int>(value->ToNumber()) : value->ToNumber()) << std::endl;
}

void PrintString(std::ostream & stream, const Value * value) {
    assert(stream.good());
    assert(value && value->IsString());
    stream << value->ToString() << std::endl;
}

void PrintBoolean(std::ostream & stream, const Value * value) {
    assert(stream.good());
    assert(value && value->IsBoolean());
    stream << (value->ToBoolean() ? "true" : "false") << std::endl;
}

void PrintUndef(std::ostream & stream, const Value * value) {
    assert(stream.good());
    assert(value && value->IsUndef());
    stream << "undef" << std::endl;
}

void PrintNil(std::ostream & stream, const Value * value) {
    assert(stream.good());
    assert(value && value->IsNil());
    stream << "nil" << std::endl;
}

void PrintLibraryFunction(std::ostream & stream, const Value * value) {
    assert(stream.good());
    assert(value && value->IsLibraryFunction());
    stream << "[ Library Function \"" << value->ToLibraryFunctionId() << "\" ]" << std::endl;
}

void PrintNativePointer(std::ostream & stream, const Value * value) {
    assert(stream.good());
    assert(value && value->IsNativePtr());
    stream << "[ Pointer " << value->ToNativeTypeId() << " ]" << std::endl;
}

void PrintProgramFunction(std::ostream & stream, const Value * value) {
    assert(stream.good());
    assert(value && value->IsProgramFunction());

    const Object * funcdef = value->ToProgramFunctionAST();
    assert((*funcdef)[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_FUNCTION_DEF);

    const Object * child = (*funcdef)[AST_TAG_FUNCTION_ID]->ToObject();
    assert((*child)[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_ID);

    std::string name = (*child)[AST_TAG_ID]->ToString();

    if(name[0] == '$') stream << "[ Anonymous Function ]" << std::endl;
    else stream << "[ Function \"" + name + "\" ]" << std::endl;
}

void PrintObject(std::ostream & stream, const Value * value) {
    assert(stream.good());
    assert(value && value->IsObject());

    Object * o = value->ToObject_NoConst();

    if(o->GetTotal() == 0) {
        stream << "[Object] [ ]" << std::endl;
        return;
    }

    stream << "[Object] [" << std::endl;

    whitespace += WHITESPACE_STEP;
    o->Visit([&stream](const Value &key, const Value &val) {
        if (key.IsString()) stream << std::setw(whitespace) << '"' << key.ToString() << "\" : ";
        else if (key.IsNumber()) stream << std::setw(whitespace) << key.ToNumber() << " : ";
        else assert(false);

        PrintValue(stream, &val);
    });
    whitespace -= WHITESPACE_STEP;

    stream << std::setw(whitespace) << "]" << std::endl;
}

void PrintValue(std::ostream & stream, const Value * value) {
    assert(stream.good());
    assert(value && value->IsValid());

    switch(value->GetType()) {
        case Value::Type::NumberType: return PrintNumber(stream, value);
        case Value::Type::StringType: return PrintString(stream, value);
        case Value::Type::BooleanType: return PrintBoolean(stream, value);
        case Value::Type::UndefType: return PrintUndef(stream, value);
        case Value::Type::NilType: return PrintNil(stream, value);
        case Value::Type::LibraryFunctionType: return PrintLibraryFunction(stream, value);
        case Value::Type::NativePtrType: return PrintNativePointer(stream, value);
        case Value::Type::ProgramFunctionType: return PrintProgramFunction(stream, value);
        case Value::Type::ObjectType: return PrintObject(stream, value);
        default: assert(false);
    }
}

void LibFunc::Print(Object & env) {
    assert(env.IsValid());

    for(register unsigned i = 0; i < env.GetNumericSize(); ++i) {
        const Value * value = GetArgument(env, i);
        PrintValue(std::cout, value);
    }

    env.Set(RETVAL_RESERVED_FIELD, Value());
}

void LibFunc::Typeof(Object & env) {
    assert(env.IsValid());
    const Value * value = GetArgument(env, 0);
    env.Set(RETVAL_RESERVED_FIELD, value->GetTypeToString());
}

void LibFunc::ObjectKeys(Object & env) {
    assert(env.IsValid());

    const Value * value = GetArgument(env, 0);
    if (!value->IsObject()) return Interpreter::RuntimeError("Library function \"object_keys\" did not receive an object.");

    const Object * obj = value->ToObject();
    Object * table = new Object();
    table->IncreaseRefCounter();
    unsigned i = 0;

    obj->Visit([table, &i](const Value &key, const Value &val) {
        table->Set(i++, key);
    });

    env.Set(RETVAL_RESERVED_FIELD, table);
}

void LibFunc::ObjectSize(Object & env) {
    assert(env.IsValid());

    const Value * value = GetArgument(env, 0);
    if (!value->IsObject()) return Interpreter::RuntimeError("Library function \"object_size\" did not receive an object.");

    const Object * obj = value->ToObject();
    env.Set(RETVAL_RESERVED_FIELD, static_cast<double>(obj->GetTotal()));
}