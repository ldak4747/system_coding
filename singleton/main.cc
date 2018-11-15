#include "singleton.h"
#include "gtest/gtest.h"
#include <iostream>
#include <thread>
#include <vector>

class Resource {
    std::mutex mtx;
public:
    Resource() {
        std::cout << "construct" << std::endl;
    }
    ~Resource() {
        std::cout << "destruct" << std::endl;
    }

    void impl (int id) {
        std::unique_lock<std::mutex> lock(mtx);
        std::cout << id << std::endl;
    }
};
TEST (test, template_singleton) {
    Resource onstack;       //could create instance on stack.

    std::vector<std::thread> ths;
    for (int i = 0; i < 10; i++) {
        ths.emplace_back([i]() {
            std::shared_ptr<Resource> instance = Singleton<Resource>::getInstance();
            instance->impl(i);
        });
    }

    for (auto &th: ths) {
        th.join();
    }
}

/*
 *  完美单例模板类的资源类示例
 *  1、构造函数要置为private, 防止在栈上可以创建对象, 不过析构函数没必要设为private;
 *  2、将基类的资源类泛化设置为友元类, 保证单例模板类SingletonArgs可以调用自己的构造函数;
 * */
class ResourceArgs {
    int i;
    float j;
    double k;

    static int default_i;
    static float default_j;
    static double default_k;
    std::mutex mtx;

    explicit ResourceArgs(int _i = default_i, float _j = default_j, double _k = default_k): i(_i), j(_j), k(_k) {
        std::cout << "ResourceArgs construct" << std::endl;
    }

    friend class SingletonArgs<ResourceArgs>;

public:
    ~ResourceArgs() {
        i = default_i;
        j = default_j;
        k = default_k;
        std::cout << "ResourceArgs destruct" << std::endl;
    }
    
    void get () {
        std::cout << "show: i: " << i << ", j: " << j << ", k: " << k << std::endl;
    }

    void set (int id) {
        i = id;
        j = static_cast<float>(id);
        k = static_cast<double>(id);
    }

    void impl (int id) {
        std::unique_lock<std::mutex> lock(mtx);
        get();
        set(id);
    }
};
int ResourceArgs::default_i     = 0;
float ResourceArgs::default_j   = 0.0;
double ResourceArgs::default_k  = 0.0;

TEST (test, withargs_singleton) {
    //ResourceArgs onstack(1, 10.0, 100.0);     //could not create instance on stack.

    std::vector<std::thread> ths;
    for (int i = 0; i < 10; i++) {
        ths.emplace_back([i]() {
            std::shared_ptr<ResourceArgs> instance = SingletonArgs<ResourceArgs>::getInstance(50, 50, 50);
            instance->impl(i);
        });
    }

    for (auto &th: ths) {
        th.join();
    }
}

int main (int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
