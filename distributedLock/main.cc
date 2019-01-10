#include <thread>
#include <string>
#include <vector>
#include <random>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <assert.h>
#include "glog/logging.h"

time_t getTime () {
    std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> now = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    return tmp.count();
}

time_t getExpireTime () {
    return getTime() + 5;
}

std::string parse (char *buf) {
    std::string r = "";
    int len = 0;
    for (int i = 0; i < strlen(buf); ++i) {
        if (buf[i] == '$') {
            ++i;
            len = buf[i] - '0';
            continue;
        }
        if (buf[i] == '\r' || buf[i] == '\n') {
            continue;
        }
        r += buf[i];
    }
    return r;
}

void impl (int rds, int i, time_t expireTime) {
    //get resource
    char buf[1024] = {0};
    std::string getOffset = "get offset\r\n";
    send(rds, getOffset.data(), getOffset.length(), 0);
    recv(rds, buf, 1024, 0);
    int curOffset = std::stoi(parse(buf));
    LOG(INFO) << "index " << i << " curOffset:" << curOffset;

    //pull data from msgqueue, mock by sleep [0, 100]
    std::string incrOffset = "incr offset\r\n";
    send(rds, incrOffset.data(), incrOffset.length(), 0);
    memset(buf, 1024, 0);
    recv(rds, buf, 1024, 0);
    //LOG(INFO) << "index " << i << " work " << sleepTime << " ms, exec positiveDel.";
    //mock the data process
    std::random_device rd;
    int sleepTime = rd() % 10;
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
}

void deleteLock (int rds, int i, time_t expireTime) {
    //after impl, try to delete distributed lock
    char buf[1024] = {0};
    std::string positiveLua = "eval \"local curval = redis.call('get', 'Lock') if curval == ARGV[1] then redis.call('del', 'Lock') return true else return false end\" 0 ";
    positiveLua += std::to_string(expireTime);
    positiveLua += "\r\n";
    send(rds, positiveLua.data(), positiveLua.length(), 0);
    recv(rds, buf, 1024, 0);
    //LOG(INFO) << "index " << i << " positiveDel Lock. " << buf;
    
}

int main () {
    ::google::SetStderrLogging(::google::GLOG_FATAL);
    FLAGS_logbufsecs = 0;

    std::vector<std::thread> workers;
    for (int i = 0; i < 10; ++i) {
        workers.emplace_back([i] () {
            int rds = socket(AF_INET, SOCK_STREAM, 0);
            assert(rds >= 0);

            struct sockaddr_in redis;
            redis.sin_family = AF_INET;
            redis.sin_port = htons(8888);
            redis.sin_addr.s_addr = inet_addr("10.75.29.40");
            assert(0 == connect(rds, (struct sockaddr *)&redis, sizeof(redis)));

            while (1) {
                time_t expireTime = getExpireTime();
                std::string setLock = "setnx Lock ";
                setLock += std::to_string(expireTime);
                setLock += "\r\n";
                int ret = send(rds, setLock.data(), setLock.length(), 0);
                char buf[1024] = {0};
                recv(rds, buf, 1024, 0);
                //LOG(INFO) << "index " << i << " try setnx. expireTime " << expireTime;

                if (buf[1] == '1') {
                    //LOG(INFO) << "index " << i << " setnx success. expireTime " << expireTime;
                    impl(rds, i, expireTime);
                    deleteLock(rds, i, expireTime);

                } else {
                    memset(buf, 1024, 0);
                    time_t curTime = getTime();
                    time_t expireTime = getExpireTime();
                    //LOG(INFO) << "index " << i << " begin to compare with value and curtime " << curTime;
                    std::string negativeLua = "eval \"local curval = redis.call('get', 'Lock') if curval < ARGV[1] then redis.call('set', 'Lock', ARGV[2]) return true else return false end\" 0 ";
                    negativeLua += std::to_string(curTime);
                    negativeLua += " ";
                    negativeLua += std::to_string(expireTime);
                    negativeLua += "\r\n";

                    memset(buf, 1024, 0);
                    send(rds, negativeLua.data(), negativeLua.length(), 0);
                    recv(rds, buf, 1024, 0);
                    if (buf[1] == '1') {
                        //LOG(INFO) << "index " << i << " getset success. expireTime " << expireTime;
                        impl(rds, i, expireTime);
                        deleteLock(rds, i, expireTime);
                    } else {
                        //LOG(INFO) << "index " << i << " try setnx again";
                    }
                }
            }
        });
    }

    for (auto &th: workers) {
        th.join();
    }
}
