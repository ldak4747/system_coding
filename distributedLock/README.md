1、错误方式

    1.1、错误方式1: 依靠expire命令, 设置超时时间点
        IF OK == setnx(lockKey, pid)
            redis.expire(expiretime)
            //process
            redis.del(lockKey)
        ELSE
            pass

    问题: 如果setnx成功, 准备设置超时时间点时, 此时线程崩溃, 则lockKey永远存在造成死锁
    note: setnx无法做到同时设置超时时间
    结论: 需借助value维护超时时间点

    1.2、错误方式2: 未获取分布式锁的线程, 依靠get、set两条命令判断是否超时
        IF OK == setnx(lockKey, expiretimepoint)
            //process
            redis.del(lockKey)
        ELSE
            expiretimepoint = redis.get(lockKey)
            IF currenttime >= expiretimepoint
                redis.set(lockKey, newexpiretimepoint)
        
        问题: get和set之间, 其他线程也执行了get, 也认为可以获取到锁, 这样就都获取到锁
        结论: 在未获取到锁时, 如果当前持有锁的线程发生意外退出, 可能存在多个线程都获取到锁
              get和set需要做到原子

    1.3、错误方式3: 未获取到分布式锁的线程, 依靠getset一条命令判断是否超时
        IF OK == setnx(lockKey, expiretimepoint)
            //process
            redis.del(lockKey)
        ELSE
            expiretimepoint = redis.get(lockKey)
            IF currenttime >= expiretimepoint
                oldexpiretimepoint = redis.getset(lockKey, newexpiretimepoint)
                IF oldexpiretimepoint == expiretimepoint
                    //process
                    redis.del(lockKey)

        问题: 如果锁竞争者较多, 那么最终设置的超时时间点, 很可能不是最初设置的时间点
        结论: 此种情况依然可能出问题, 不过相对不是很大

2、正确方式
    利用lua脚本(或者事务)的多条操作的原子性的特性:

    2.0、初始lockKey不存在, 多个竞争者做setnx, 只有一个可以成功
    2.1、竞锁成功者, 执行相关资源操作
        操作完成后, 执行lua包括如下内容
            1、当前锁还存在?
               如果不存在, 返回2.0阶段
               如果还存在, 但value已不再是自己竞锁成功时设置的超时时间点, 则说明因超时, 已经被其他竞锁者竞争到锁, 返回2.0阶段
               如果还存在, 且还是自己竞锁成功时设置的超时时间点, 则说明锁还在被自己占据, 主动删除锁, 返回2.0阶段

    2.2、竞锁失败者, 执行lua包括如下内容
        0、执行前, 获取当前时间, 和如果自己获取到锁, 需要设置的超时时间点
        1、当前锁还存在?
            如果不存在, 说明被主动删除了, 返回2.0阶段
            如果还存在, 其value是否已经小于当前时间了, 如果小于了, 说明已超时, 将锁更新为自己持有, 设置自己的新的超时时间点, 给应用程序返回竞锁成功, 应用程序进入2.1阶段
            如果还存在, 但value并未小于当前时间, 说明还没有超时, 返回2.0阶段

    总结: 相比错误方式2, 此种方式的竞锁成功者, 其设置的超时时间点, 不会被修改







        
