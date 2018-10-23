#include "threadpool.h"
#include <sys/sysinfo.h>
#include <iostream>
#include <chrono>
#include <random>
#include <memory>

int add (int a, int b) {
    return a + b;
}

int minos (int a, int b) {
    return a - b;
}

int multiple (int a, int b) {
    return a * b;
}

int division (int a, int b) {
    return a / b;
}

int main () {
    std::unique_ptr<ThreadPool> tp(new ThreadPool(get_nprocs() * 2));

    std::future<int> addres = tp->commit(add, 1, 2);
    std::future<int> minosres = tp->commit(minos, 1, 2);
    std::future<int> multipleres = tp->commit(multiple, 1, 2);
    std::future<int> divisionres = tp->commit(division, 1, 2);

    std::cout << addres.get() << std::endl;
    std::cout << minosres.get() << std::endl;
    std::cout << multipleres.get() << std::endl;
    std::cout << divisionres.get() << std::endl;
    return 0;
}
