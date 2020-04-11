#include "LibraryFunctions.h"
#include "Utilities.h"

#include <iostream>
#include <cassert>
#include <ostream>
#include <iomanip>

unsigned whitespace = 0;

#define WHITESPACE_STEP 4

#define RETVAL_RESERVED_FIELD "$retval"

#define RETURN_NIL do { env.Set(RETVAL_RESERVED_FIELD, NilTypeValue::Nil); return; } while(false);

const Value * GetArgument(Object & env, unsigned argNo, const std::string & optArgName = "") {
    assert(env.IsValid());

    if (optArgName.empty()) return env[argNo];

    auto arg = env[optArgName];
    if (!arg) arg = env[argNo];

    return arg;
}

void PrintValue(std::ostream & stream, const Value * value) {
    assert(stream.good());
    assert(value && value->IsValid());

    if (value->IsNumber()) stream << (Utilities::IsInt(value->ToNumber()) ? static_cast<int>(value->ToNumber()) : value->ToNumber()) << std::endl;
    else if (value->IsString()) stream << value->ToString() << std::endl;
    else if (value->IsBoolean()) stream << (value->ToBoolean() ? "true" : "false") << std::endl;
    else if (value->IsUndef()) stream << "undef" << std::endl;
    else if (value->IsNil()) stream << "nil" << std::endl;
    else if (value->IsProgramFunction()) stream << "[Function]" << std::endl;
    else if (value->IsLibraryFunction()) stream << "[Library Function \"" << value->ToLibraryFunctionId() << "\"]" << std::endl;
    else if (value->IsNativePtr()) stream << "[Pointer " << value->ToNativeTypeId() << "]" << std::endl;
    else if (value->IsObject()) {
        stream << "[Object] [" << std::endl;
        Object * o = value->ToObject_NoConst();

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
    else assert(false);
}

void LibFunc::Print(Object & env) {
    assert(env.IsValid());

    for(register unsigned i = 0; i < env.GetNumericSize(); ++i) {
        const Value * value = GetArgument(env, i);
        PrintValue(std::cout, value);
    }

    RETURN_NIL;
}

void LibFunc::Typeof(Object & env) {
    assert(env.IsValid());
    const Value * value = GetArgument(env, 0);
    env.Set(RETVAL_RESERVED_FIELD, value->GetTypeToString());
}

void LibFunc::ObjectKeys(Object & env) {
    assert(env.IsValid());

    const Value * value = GetArgument(env, 0);
    if (!value->IsObject()) RETURN_NIL; /* TODISCUSS: Perhaps a runtime error? */

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
    if (!value->IsObject()) RETURN_NIL; /* TODISCUSS: Perhaps a runtime error? */

    const Object * obj = value->ToObject();
    env.Set(RETVAL_RESERVED_FIELD, static_cast<double>(obj->GetTotal()));
}