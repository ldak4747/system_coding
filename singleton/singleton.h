#pragma
#include <memory>
#include <mutex>

/*
 *  常规单例模板类
 *
 *  描述 : 这样的一个单例类, 实现了:
 *  1、全局唯一实例, 只能在堆上创建;
 *  2、资源由shared_ptr封装, 可以自动释放
 *  3、由call_once控制实例创建, 多线程安全
 *
 *  不足 : 
 *  1、如果资源类的构造函数是public, 依然可以自行创建资源
 *  2、无法传入构造参数创建实例
 *
 * */
template<class T> class Singleton {
    static std::shared_ptr<T> instance;

public:
    Singleton() = default;
    ~Singleton() = default;
    Singleton(const Singleton &other) = delete;
    Singleton &operator= (const Singleton &other) = delete;

    static std::shared_ptr<T> &getInstance() {
        static std::once_flag flag;
        std::call_once(flag, [&]() {
            instance.reset(new T());
        });

        return instance;
    }
};

template<class T> std::shared_ptr<T> Singleton<T>::instance;

/*
 *  完美单例模板类
 *
 *  1、只能在堆上创建唯一实例, 资源类的基类构造函数要置为private, 不可以在栈上创建
 *  2、可以传入任意参数创建实例
 *
 *  实现的思路 : 
 *  1、资源类构造函数设置为private, 防止在栈上创建实例; 同时将SingletonArgs<ResourceArgs>设为友元类; 使单例模板类可以创建资源类的实例; 这就解决了问题1
 *  2、单例模板类的getInstance方法, 做成可变参数模板函数, 实现任意参数创建实例
 * */
template<class T> class SingletonArgs {
    static std::shared_ptr<T> instance;

    SingletonArgs() = default;
    ~SingletonArgs() = default;
    SingletonArgs(const SingletonArgs &other) = delete;
    SingletonArgs &operator= (const SingletonArgs &other) = delete;
    
public:
    template<class ...Args> static std::shared_ptr<T> &getInstance (Args &&... args) {
        static std::once_flag flag;
        std::call_once(flag, [&]() {
            instance.reset(new T(std::forward<Args>(args)...));
        });

        return instance;
    }
};

template<class T> std::shared_ptr<T> SingletonArgs<T>::instance;
