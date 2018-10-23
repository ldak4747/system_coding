#include "threadpool.h"
#include <sys/sysinfo.h>
#include <iostream>
#include <chrono>
#include <random>
#include <memory>

int add (int a, int b) {
    return a + b;
}

std::string minos (int a, int b) {
    return std::to_string(a - b);
}

int multiple (int a, int b) {
    return a * b;
}

std::string division (int &a, int &b) {
    a = 10;
    b = 100;
    return std::to_string(a / b);
}

int main () {
    std::unique_ptr<ThreadPool> tp(new ThreadPool(get_nprocs() * 2));

    std::future<int> addres = tp->commit(add, 1, 2);
    std::future<std::string> minosres = tp->commit(minos, 1, 2);
    auto multipleres = tp->commit(multiple, 1, 2);
    int a = 1, b = 2;
    std::future<std::string> divisionres = tp->commit(division, std::ref(a), std::ref(b));

    std::cout << addres.get() << std::endl;
    std::cout << minosres.get() << std::endl;
    std::cout << multipleres.get() << std::endl;
    std::cout << divisionres.get() << std::endl;

    std::cout << a << std::endl;
    std::cout << b << std::endl;
    return 0;
}
