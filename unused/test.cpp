#include <iostream>

#include "../src/ValueStack.h"

int main(void) {
    ValueStack stack;

    stack.Push(10.0f);
    stack.Push("String");
    stack.Push(false);
    stack.Push(true);
    stack.Push(NilTypeValue::Nil);

    stack.Pop();
    std::cout << stack.Top().ToBoolean() << std::endl;
    std::cout << stack.GetTopAndPop().ToBoolean() << std::endl;
    std::cout << stack.GetTopAndPop().ToBoolean() << std::endl;
    std::cout << stack.GetTopAndPop().ToString() << std::endl;
    std::cout << stack.GetTopAndPop().ToNumber() << std::endl;

    Value v1(0.0f);
    Value v2(0.1f);

    std::cout << "\nTrue test" << std::endl;

    std::cout << v1.operator bool() << std::endl;
    std::cout << v2.operator bool() << std::endl;

    return EXIT_SUCCESS;
}