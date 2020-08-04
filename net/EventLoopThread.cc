#include "EventLoopThread.h"
#include "EventLoop.h"

EventLoopThread::EventLoopThread(const ThreadInitCallback& cb): 
    loop_(NULL),
    thread_(std::bind(&EventLoopThread::threadFunc, this)),
    mutex_(),
    cond_(mutex_),
    callback_(cb)
{

}

EventLoopThread::~EventLoopThread()
{
    loop_->quit();
    thread_.join();
}

EventLoop* EventLoopThread::startLoop()
{
    assert(!thread_.started());
    thread_.start();

    {
        MutexLockGuard lock(mutex_);
        while(loop_ == NULL)
        {
            cond_.wait();
        }
    }

    return loop_;
}

void EventLoopThread::threadFunc()
{
    EventLoop loop;
    if(callback_)
    {
        callback_(&loop);
    }

    {
        MutexLockGuard lock(mutex_);
        loop_ = &loop; // loop_虽然指向了一个栈上的对象，但是threadFunc函数退出后了，意味着线程也退出了，EventLoopThreadye也就没有存在的价值了，因此没什么大问题
        cond_.notify();
    }

    loop.loop();
}
