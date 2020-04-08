#ifndef _LIBRARY_FUNCTIONS_H_
#define _LIBRARY_FUNCTIONS_H_

#include "Object.h"
#include "Value.h"

#include <string>

const Value * GetArgument(Object & env, unsigned argNo, const std::string & optArgName = "");

#endif