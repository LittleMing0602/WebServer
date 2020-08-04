#ifndef EVENTLOOPTHEAD_H
#define EVENTLOOPTHEAD_H

#include "../base/Thread.h"
#include "../lock/Mutex.h"
#include "../lock/Condition.h"
#include <functional>

class EventLoop;

class EventLoopThread
{
public:
    typedef std::function<void(EventLoop*)> ThreadInitCallback;

    EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback());
    ~EventLoopThread();

    EventLoop* startLoop(); // 启动线程，称为io线程

private:
    void threadFunc();

    EventLoop* loop_;
    Thread thread_;
    MutexLock mutex_;
    Condition cond_;
    ThreadInitCallback callback_; // 在loop时间循环之前被调用
};


#endif
