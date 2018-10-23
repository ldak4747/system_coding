#ifndef __THREAD_POOL__H_
#define __THREAD_POOL__H_
#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <assert.h>
#include <future>

constexpr int THREAD_NUM= 12;

using Task = std::function<void()>;

class ThreadPool {
    std::vector<std::thread> tpool;
    std::queue<Task> tasks;
    
    std::mutex mtx;
    std::condition_variable cond;
    std::atomic<bool> stoped;
    std::atomic<int> IdleThreadNum;

public:
    ThreadPool(int size = THREAD_NUM);
    ~ThreadPool();
    template<class F, class... Args> auto commit (F f, Args ...args) {
        if (stoped.load()) {
            throw std::runtime_error("commit on ThreadPool is stoped.");
        }

        using ReturnType = decltype(f(args...));
        auto task = std::make_shared<std::packaged_task<ReturnType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        
        std::future<ReturnType> future = task->get_future();
        {
            std::unique_lock<std::mutex> lock(mtx);
            tasks.emplace([task] {
                (*task)();
            });
        }

        cond.notify_one();
        return future;
    }

    int GetIdleThreadNum() const {
        return IdleThreadNum.load();
    };
};


#endif
