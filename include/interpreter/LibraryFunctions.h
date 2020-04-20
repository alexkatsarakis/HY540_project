#ifndef _LIBRARY_FUNCTIONS_H_
#define _LIBRARY_FUNCTIONS_H_

#include "Object.h"
#include "Value.h"

#include <string>

namespace LibFunc {

    void Print(Object & env);

    void Typeof(Object & env);

    void Assert(Object & env);

    void Sqrt(Object & env);

    void Pow(Object & env);

    void GetTime(Object & env);

    void Input(Object & env);

    void ObjectKeys(Object & env);

    void ObjectSize(Object & env);

    void Sleep(Object & env);
}

#endif