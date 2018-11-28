#include <iostream>

/*
 *  简易工程模式, 工厂类根据参数创建不同product对象, low
 *  作为改进的工厂模式, 工厂类也改成基类, 有多少个产品子类, 就有多少个工厂子类, 更加科学了
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
    virtual ~Factory() {
        std::cout << "Factory destruct" << std::endl;
    }
    virtual Product *createproduct() = 0;
};

class FactoryA: public Factory {
public:
    virtual ~FactoryA () {
        std::cout << "FactoryA destruct" << std::endl;
    }
    virtual Product *createproduct() override {
        return new ProductA;
    }
};

class FactoryB: public Factory {
public:
    virtual ~FactoryB () {
        std::cout << "FactoryB destruct" << std::endl;
    }
    virtual Product *createproduct() override {
        return new ProductB;
    }
};

int main () {
    Factory *f1 = new FactoryA, *f2 = new FactoryB;

    Product *p1 = f1->createproduct();
    Product *p2 = f2->createproduct();

    p1->func();
    p2->func();

    delete p1;
    delete p2;
    delete f1;
    delete f2;
    return 0;
}
