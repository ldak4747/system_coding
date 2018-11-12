#include "listimpl.hpp"
#include <list>
#include <queue>
#include "gtest/gtest.h"
#include <chrono>
#include <random>
#include <iostream>
#include <functional>

const int trytime = 10;
const int times = 1000 * 1000 * 100;
struct TestExecutor {
    std::function<int ()> func;
    TestExecutor (std::function<int ()> &&f): func(f){
        int cost = 0;
        for (int i = 0; i < trytime; i++) {
            cost += func();
        }
        std::cout << "average cost: " << double(cost)/trytime << std::endl;
    }
};

TEST (test, List) {
    List<int> list;
    auto lambda = [&list]() {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now(), end;
        for (int i = 0; i < times; i++) {
            list.push_back(i);
            list.front();
            list.pop_front();
        }
        end = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    };

    TestExecutor testexec(lambda);
}

TEST (test, std_list) {
    std::list<int> list;
    auto lambda = [&list]() {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now(), end;
        for (int i = 0; i < times; i++) {
            list.push_back(i);
            list.front();
            list.pop_front();
        }
        end = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    };

    TestExecutor testexec(lambda);
}

TEST (test, std_queue) {
    std::queue<int> q;
    auto lambda = [&q]() {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now(), end;
        for (int i = 0; i < times; i++) {
            q.push(i);
            q.front();
            q.pop();
        }
        end = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    };

    TestExecutor testexec(lambda);
}

int main (int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
