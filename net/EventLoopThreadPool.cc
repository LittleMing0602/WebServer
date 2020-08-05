#include "EventLoopThreadPool.h"
#include "EventLoop.h"

EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop):
    baseLoop_(baseLoop),
    started_(false),
    numThreads_(0),
    next_(0)
{
}

EventLoopThreadPool::~EventLoopThreadPool()
{

}

void EventLoopThreadPool::start(const ThreadInitCallback& cb)
{
    assert(!started_);
    baseLoop_->assertInLoopThread();

    started_ = true;

    for(int i = 0; i < numThreads_; ++i)
    {
        EventLoopThread* t = new EventLoopThread(cb);
        threads_.push_back(std::unique_ptr<EventLoopThread>(t));
        // 启动EventLoop线程，在这个EventLoop进入事件循环之前，调用cb
        loops_.push_back(t->startLoop());

    }

    if(numThreads_ == 0 && cb)
    {
        cb(baseLoop_);
    }

}

EventLoop* EventLoopThreadPool::getNextLoop()
{
    baseLoop_->assertInLoopThread();
    EventLoop* loop = baseLoop_;

    // 如果loops_为空，则loop指向baseLoop_
    // 否则，按照round-robin的调度方式选择一个EventLoop
    if(!loops_.empty())
    {
        loop = loops_[next_];
        ++next_;
        if(static_cast<size_t>(next_) >= loops_.size())
        {
            next_ = 0;
        }
    }

    return loop;
}