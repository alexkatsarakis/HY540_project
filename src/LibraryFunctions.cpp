#include "LibraryFunctions.h"
#include "Utilities.h"

#include <iostream>
#include <cassert>
#include <ostream>
#include <iomanip>

unsigned whitespace = 0;

#define WHITESPACE_STEP 4

#define RETURN_NULL env.Set("$retval", NilTypeValue::Nil); return

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
        stream << "[Object] [ " << std::endl;
        Object * o = value->ToObject_NoConst();

        whitespace += WHITESPACE_STEP;
        o->Visit([&stream](const Value &key, const Value &val) {
            if (key.IsString()) stream << std::setw(whitespace) << "\"" << key.ToString() << "\" : ";
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

    RETURN_NULL;
}