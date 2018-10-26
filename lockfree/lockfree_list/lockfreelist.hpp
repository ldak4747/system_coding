#ifndef __LOCK_FREE_LIST_HPP__
#define __LOCK_FREE_LIST_HPP__
#include <list>
#include <mutex>
#include <thread>
#include <chrono>
#include <atomic>

template<class T> class LockfreeList {
    std::list<T> list;
    bool lock = false;
    const bool withlock = true, withoutlock = false;
    #ifndef USE_ATOMIC
    std::mutex mtx;
    #endif

    void Lock () {
        #ifdef USE_ATOMIC
        while (!__sync_bool_compare_and_swap(&lock, withoutlock, withlock)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        #elif USE_ATOMIC_C11
        while (!std::compare_and_swap(&lock, withoutlock, withlock)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        #endif
    }
    void Unlock () {
        #ifdef USE_ATOMIC
        __sync_bool_compare_and_swap(&lock, withlock, withoutlock);
        #elif USE_ATOMIC_C11
        std::compare_and_swap(&lock, withlock, withoutlock);
        #endif
    }
public:
    void Push(const T &val);
    void Pop();
    T Front();
};

template<class T> void LockfreeList<T>::Push (const T &val) {
    #ifdef USE_ATOMIC
    Lock();
    #else
    std::unique_lock<std::mutex> lock(mtx);
    #endif

    list.push_back(val);
    
    #ifdef USE_ATOMIC
    Unlock();
    #endif
}

template<class T> void LockfreeList<T>::Pop () {
    #ifdef USE_ATOMIC
    Lock();
    #else
    std::unique_lock<std::mutex> lock(mtx);
    #endif

    list.pop_front();

    #ifdef USE_ATOMIC
    Unlock();
    #endif
}

template<class T> T LockfreeList<T>::Front () {
    #ifdef USE_ATOMIC
    Lock();
    #else
    std::unique_lock<std::mutex> lock(mtx);
    #endif

    T res = list.front();

    #ifdef USE_ATOMIC
    Unlock();
    #endif
    return res;
}

#endif
