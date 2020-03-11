#include <iostream>
#include <memory>

class Foobar {
private:
    int x;

public:
    int y;
};

int main(void) {
    std::shared_ptr<Foobar> ptr = std::make_shared<Foobar>();
    std::shared_ptr<Foobar> p = nullptr;

    ptr->y = 10;

    std::cout << ptr.use_count() << std::endl;
    p = ptr;
    std::cout << ptr.use_count() << std::endl;
    //std::cout << p->y << std::endl;
    ptr.reset();
    std::cout << p.use_count() << std::endl;

    return 0;
}