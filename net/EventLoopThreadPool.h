#ifndef EVENTLOOPTHREADPOOL_H
#define EVENTLOOPTHREADPOOL_H

#include <memory>
#include <vector>
#include "EventLoopThread.h"
#include <functional>

class EventLoopThreadPool
{
public:
    typedef std::function<void(EventLoop*)> ThreadInitCallback;
    
    EventLoopThreadPool(EventLoop* baseLoop);
    ~EventLoopThreadPool();

    void setThreadNum(int numThreads) { numThreads_ = numThreads; }
    void start(const ThreadInitCallback& cb);
    EventLoop* getNextLoop();

private:
    EventLoop* baseLoop_;
    bool started_;
    int numThreads_;
    int next_;
    std::vector<std::unique_ptr<EventLoopThread>> threads_;
    std::vector<EventLoop*> loops_;

};

#endif
