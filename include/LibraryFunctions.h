#ifndef _LIBRARY_FUNCTIONS_H_
#define _LIBRARY_FUNCTIONS_H_

#include "Object.h"
#include "Value.h"

#include <string>

namespace LibFunc {

    void Print(Object & env);

    void Typeof(Object & env);

    void ObjectKeys(Object & env);

    void ObjectSize(Object & env);
}

#endif