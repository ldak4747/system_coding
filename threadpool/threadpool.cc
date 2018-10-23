#include "threadpool.h"

ThreadPool::ThreadPool (int size): stoped(false) {
    assert(size >= THREAD_NUM);
    IdleThreadNum = (size < THREAD_NUM)?THREAD_NUM:size;
    
    for (int i = 0; i < size; i++) {
        tpool.emplace_back([this] {
            while (!stoped.load()) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(mtx);
                    cond.wait(lock, [this]{ return stoped.load() || !tasks.empty(); });

                    if (stoped.load() && tasks.empty()) {
                        return;
                    }

                    task = std::move(tasks.front());
                    tasks.pop();
                }

                --IdleThreadNum;
                task();
                ++IdleThreadNum;
            }
        });
    }
}

ThreadPool::~ThreadPool () {
    stoped.store(true);
    cond.notify_all();
    for (auto &thread: tpool) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}


