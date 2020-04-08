#include "LibraryFunctions.h"

#include <cassert>

const Value * GetArgument(Object & env, unsigned argNo, const std::string & optArgName) {
    assert(env.IsValid());

    if (optArgName.empty()) return env[argNo];

    auto arg = env[optArgName];
    if (!arg) arg = env[argNo];

    return arg;
}