#include <iostream>
#include <string>

int main(void) {
    std::cout << "Hello world" << std::endl;

    char * ptr = (char *) malloc(32 * sizeof(char));

    std::memset(ptr, 0, 32 * sizeof(char));

    return 0;
}