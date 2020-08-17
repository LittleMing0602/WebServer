#include "EventLoop.h"
#include "../base/CurrentThread.h"
#include "../log/Logging.h"
#include "poll.h"
#include "Channel.h"
#include "Poller.h"
#include <sys/eventfd.h>
#include <signal.h>

const int kPollTimeMs = 10000;

class IgnoreSigPipe{
public:
    IgnoreSigPipe()
    {
        signal(SIGPIPE, SIG_IGN);
    }
};

IgnoreSigPipe initObj;  // 全局对象，用于忽略SIGPIPE信号

int createEventfd()
{
    int fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if(fd < 0)
    {
        printf("error: createEventfd()\n");
        abort();
    }
    return fd;
}

EventLoop::EventLoop(): 
    threadId_(tid()),
    looping_(false),
    quit_(false),
    poller_(Poller::newDefaultPoller(this)),
    activeChannels_(),
    callingPendingFunctors_(false),
    wakeupFd_(createEventfd()),
    wakeupChannel_(new Channel(this, wakeupFd_)),
    mutex_(),
    pendingFunctors_(),
    timerQueue_(new TimerQueue(this))
{
    wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead, this));
    wakeupChannel_->enableReading();
}

EventLoop::~EventLoop()
{
    close(wakeupFd_);
}

void EventLoop::loop()
{
    assertInLoopThread();
    looping_ = true;
    quit_ = false;

    while(!quit_)    
    {
        activeChannels_.clear();
        pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_); 
        for(ChannelList::iterator it = activeChannels_.begin(); it != activeChannels_.end(); ++it)
        {
            (*it)->handleEvent(pollReturnTime_);
        }

        doPendingFunctors();
        
    }
    looping_ = false; 
}


void EventLoop::updateChannel(Channel* channel)
{
    poller_->updateChannel(channel);
}

void EventLoop::runInLoop(const Functor& cb)
{
    if(isInLoopThread())  // 如果在loop自己的线程，那么同步调用即可
    {
        cb();
    }
    else  // 如果是跨线程调用
    {
        queueInLoop(cb);
    }
}

void EventLoop::queueInLoop(const Functor& cb)
{
    {
        MutexLockGuard lock(mutex_);
        pendingFunctors_.push_back(cb);
    }

    // 如果是跨线程调用，或者在doingPendingFunctors函数中，需要唤醒eventloop
    if(!isInLoopThread() || callingPendingFunctors_)  
    {
        wakeup();
    }
}

void EventLoop::doPendingFunctors()
{
    callingPendingFunctors_ = true;
    std::vector<Functor> functors;

    //将functors交换出来，缩小临界区，避免死锁
    {
        MutexLockGuard lock(mutex_);
        functors.swap(pendingFunctors_);
    }

    for(size_t i = 0; i < functors.size(); ++i)
    {
        functors[i]();
    }

    callingPendingFunctors_ = false;
    
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    ssize_t n = write(wakeupFd_, &one, sizeof(one));
    if(n != sizeof one)
    {
        // printf("error: EventLoop::wakeup()\n");
        LOG_ERROR << "EventLoop::wakeup() writes " << n << " bytes instead of 8";
    }

}

// 读掉wakeupFd_的东西，避免忙询
void EventLoop::handleRead()
{
    uint64_t one = 1;
    ssize_t n = read(wakeupFd_, &one, sizeof one);
    if( n != sizeof one )
    {
        //printf("error: EventLoop::handleRead()\n");
        LOG_ERROR << "EventLoop::handleRead() reads " << n << " bytes instead of 8";
    }
}

void EventLoop::removeChannel(Channel* channel)
{
    assertInLoopThread();
    poller_->removeChannel(channel);
}

void EventLoop::runAt(const TimeStamp& time, const TimerCallback& cb)
{
    timerQueue_->addTimer(cb, time, 0.0);
}

void EventLoop::runAfter(double delay, const TimerCallback& cb)
{
    TimeStamp time(addTime(TimeStamp::now(), delay));
    runAt(time, cb);
}

void EventLoop::runEvery(double interval, const TimerCallback& cb)
{
    TimeStamp time(addTime(TimeStamp::now(), interval));
    timerQueue_->addTimer(cb, time, interval);
}

