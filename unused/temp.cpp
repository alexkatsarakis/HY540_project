#include <iostream>
#include "../src/Object.h"
#include "../src/Value.h"

int main(void) {

    Object obj;

    obj.Set("type", "table");
    obj.Set("child", true);
    obj.Set(10, false);
    obj.Set(20, 30.0f);

    obj.Visit([](const Value & key, const Value & val) {
        if (key.IsString()) std::cout << key.ToString();
        else std::cout << key.ToNumber();

        std::cout << " = ";

        if(val.IsNumber()) std::cout << val.ToNumber();
        else if (val.IsString()) std::cout << val.ToString();
        else if (val.IsBoolean()) std::cout << val.ToBoolean();

        std::cout << std::endl;
    });

    std::cout << obj["type"]->ToString() << std::endl;

    std::cout << obj.GetNumericSize() << std::endl;
    obj.Remove(10);
    obj.Remove(101);
    std::cout << obj.GetNumericSize() << std::endl;

    return EXIT_SUCCESS;
}