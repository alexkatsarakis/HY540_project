#include "LibraryFunctions.h"
#include "Utilities.h"

#include <iostream>
#include <cassert>

const Value * GetArgument(Object & env, unsigned argNo, const std::string & optArgName = "") {
    assert(env.IsValid());

    if (optArgName.empty()) return env[argNo];

    auto arg = env[optArgName];
    if (!arg) arg = env[argNo];

    return arg;
}

void LibFunc::Print(Object & env) {
    assert(env.IsValid());

    /* TODO: Make it work for variadic number of arguments */
    auto value = GetArgument(env, 0);
    assert(value);

    /* TODO: Please refactor!
             Recursive call for objects */
    if (value->IsNumber()) std::cout << (Utilities::IsInt(value->ToNumber()) ? static_cast<int>(value->ToNumber()) : value->ToNumber()) << std::endl;
    else if (value->IsString()) std::cout << value->ToString() << std::endl;
    else if (value->IsBoolean()) std::cout << (value->ToBoolean() ? "true" : "false") << std::endl;
    else if (value->IsUndef()) std::cout << "undef" << std::endl;
    else if (value->IsNil()) std::cout << "nil" << std::endl;
    else if (value->IsObject()) std::cout << "[Object]" << std::endl;
    else if (value->IsProgramFunction()) std::cout << "[Function]" << std::endl;
    else if (value->IsLibraryFunction()) std::cout << "[Library Function \"" << value->ToLibraryFunctionId() << "\"]" << std::endl;
    else if (value->IsNativePtr()) std::cout << "[Pointer " << value->ToNativeTypeId() << "]" << std::endl;
    else assert(false);
}