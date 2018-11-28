#include <iostream>

/*
 *  简单工厂模式的实质是由一个工厂类根据传入的参数，动态决定应该创建哪一类产品类
 * */
class Product {
public:
    virtual ~Product() {
        std::cout << "Product destruct" << std::endl;
    }
    virtual void func() = 0;
};

class ProductA : public Product {
public:
    virtual ~ProductA() {
        std::cout << "ProductA destruct" << std::endl;
    }
    virtual void func () override {
        std::cout << "I'm ProductA" << std::endl;
    }
};

class ProductB : public Product {
public:
    virtual ~ProductB() {
        std::cout << "ProductB destruct" << std::endl;
    }
    virtual void func () override {
        std::cout << "I'm ProductB" << std::endl;
    }
};

class Factory {
public:
    Product *createproduct (int arg) {
        if (arg) {
            return new ProductA;
        } else {
            return new ProductB;
        }
    }
};

int main () {
    Factory factory;

    Product *p1 = factory.createproduct(1);
    Product *p2 = factory.createproduct(0);

    p1->func();
    p2->func();

    delete p1;
    delete p2;
    return 0;
}
