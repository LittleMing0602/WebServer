#ifndef ASYNC_LOGGING_H
#define ASYNC_LOGGING_H

#include "../lock/Mutex.h"
#include "../lock/Condition.h"
#include "../lock/CountDownLatch.h"
#include "../base/Thread.h"
#include "LogStream.h"
#include <vector>
#include <memory>

class AsyncLogging
{
public:
    AsyncLogging(const string& basename, size_t rollSize, int flushInterval = 3);
    ~AsyncLogging() 
    {
        if(running_)
        {
            stop();
        }
    }

    void append(const char* logline, int len);

    void start()
    {
        running_ = true;
        thread_.start();
        latch_.wait();  // thread_启动以后才继续运行
    }

    void stop()
    {
        running_ = false;
        cond_.notify();
        thread_.join();
    }

private:
    AsyncLogging(const AsyncLogging&);
    void operator=(const AsyncLogging&);

    void threadFunc();

    typedef FixedBuffer<kLargeBuffer> Buffer;
    typedef std::vector<std::unique_ptr<Buffer>> BufferVector;
    typedef std::unique_ptr<Buffer> BufferPtr;

    const int flushInterval_;
    bool running_;
    string basename_;
    size_t rollSize_;
    Thread thread_;
    MutexLock mutex_;
    Condition cond_;
    CountDownLatch latch_; // 用于等待线程启动
    BufferPtr currentBuffer_;
    BufferPtr nextBuffer_;
    BufferVector buffers_;
};

#endif
