#include "TreeTags.h"
#include "Value.h"
#include "Object.h"

#include <iostream>

int main(void) {
    std::cout << "Hello world!" << std::endl;

    Object numconst;
    numconst.SetValue("type", TYPE_NUMCONST);
    numconst.SetValue("value", 23.0f);

    Object term;
    term.SetValue("type", TYPE_TERM);
    term.SetValue("value", &numconst);

    Object rvalue;
    rvalue.SetValue("type", TYPE_RVALUE);
    rvalue.SetValue("value", &term);

    std::cout << rvalue["value"].GetObject()["value"].GetObject()["value"].ToString() << std::endl;;

    return EXIT_SUCCESS;
}