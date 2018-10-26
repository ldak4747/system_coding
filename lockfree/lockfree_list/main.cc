#include "lockfreelist.hpp"
#include <iostream>
#include <vector>

int main () {
    LockfreeList<int> lflist;
    std::vector<std::thread> pushers, popers;

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    for (int i = 0; i < 64; i++) {
        pushers.emplace_back([&lflist]() {
            for (int i = 0; i < 1000 * 1000; i++) {
                lflist.Push(i);
            }
        });
    }
    for (auto &th: pushers) {
        th.join();
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "push cost " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

    start = std::chrono::steady_clock::now();
    for (int i = 0; i < 64; i++) {
        popers.emplace_back([&lflist]() {
            for (int i = 0; i < 1000 * 1000; i++) {
                lflist.Pop();
            }
        });
    }
    for (auto &th: popers) {
        th.join();
    }
    end = std::chrono::steady_clock::now();
    std::cout << "pop cost " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
    
    return 0;
}
