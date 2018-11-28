#include <iostream>

/*
 *  每个product有多种变种对象的话, 那么可以使用抽象工厂模式
 *  product1-----product1_A
 *          |
 *          |
 *          |----product1_B
 *
 *  product2-----product2_A
 *          |
 *          |
 *          |----product2_B
 *
 *  factory-----create1
 *         -----create2
 *
 *  factoryA-----create1
 *          -----creat2
 *
 *  factoryB-----create1
 *          -----create2
 *
 *  简单的理解, 方便创建每个产品的不同形态的对象
 *
 *  product可能分为不同的形态, 如下例, 每一种水果都有"汁"和"派"两种形态, 
 *  现在希望苹果工厂能够同时制作苹果汁和苹果派, 香蕉工厂也能如此, 
 *
 *  那么思路是:
 *  1、既然汁和派, 是更低级别抽象级别(这句话最重要), 那么, 汁和派作为最基类, 由苹果汁/香蕉汁, 继承汁, 由苹果派/香蕉派, 继承派
 *  2、然后, 工厂的基类规定好这样的方法, 也是创建汁的方法和创建派的方法(而不是返回苹果或香蕉, 因为基类是汁和派), 返回的是汁和派
 *  3、然后, 派生出, 苹果工厂和香蕉工厂, 它们都可以生产汁和派, 苹果工厂生产苹果汁和苹果派, 香蕉工厂生产香蕉汁和香蕉派
 *
 *  事实上, 如果苹果、香蕉作为最低级别的抽象, 那么产品基类应该是苹果、香蕉, 而不是汁和派, 工厂也应该是生产"苹果制品"、"香蕉制品", 而不是汁制品、派制品, 
 *  派生出的工厂, 也应该是"造汁工厂"、"造派工厂", 而不是"苹果工厂"、"香蕉工厂"; 这是根据业务需求决定的
 * */
class Juice {
public:
    virtual void func() = 0;
};

class Pie {
public:
    virtual void func() = 0;
};

class AppleJuice : public Juice {
public:
    virtual void func() override {
        std::cout << "Drink Apple Juice" << std::endl;
    }
};

class BananaJuice : public Juice {
public:
    virtual void func() override {
        std::cout << "Drink Banana Juice" << std::endl;
    }
};

class ApplePie : public Pie {
public:
    virtual void func() override {
        std::cout << "Eat Apple Pie" << std::endl;
    }
};

class BananaPie : public Pie {
public:
    virtual void func() override {
        std::cout << "Eat Banana Pie" << std::endl;
    }
};

class Factory {
public:
    virtual Juice *makeJuice() = 0;
    virtual Pie *makePie() = 0;
};

class AppleFactory : public Factory {
public:
    virtual Juice *makeJuice () override {
        return new AppleJuice;
    }
    virtual Pie *makePie () override {
        return new ApplePie;
    }
};

class BananaFactory : public Factory {
public:
    virtual Juice *makeJuice () override {
        return new BananaJuice;
    }
    virtual Pie *makePie () override {
        return new BananaPie;
    }
};

int main () {
    AppleFactory factory_use_apple;
    BananaFactory factory_use_banana;

    Juice *applejuice = factory_use_apple.makeJuice(), *bananajuice = factory_use_banana.makeJuice();
    Pie *applepie = factory_use_apple.makePie(), *bananapie = factory_use_banana.makePie();

    applejuice->func();
    bananajuice->func();
    applepie->func();
    bananapie->func();

    delete applejuice;
    delete bananajuice;
    delete applepie;
    delete bananapie;
    return 0;
}
