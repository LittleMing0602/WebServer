#include "EventLoop.h"
#include "../base/CurrentThread.h"
#include "poll.h"
#include "Channel.h"
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
    poller_(new Poller(this)),
    activeChannels_(),
    callingPendingFunctors_(false),
    wakeupFd_(createEventfd()),
    wakeupChannel_(new Channel(this, wakeupFd_)),
    mutex_(),
    pendingFunctors_()
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
    if(isInLoopThread())
    {
        cb();
    }
    else
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

    if(!isInLoopThread() || callingPendingFunctors_)
    {
        wakeup();
    }
}

void EventLoop::doPendingFunctors()
{
    callingPendingFunctors_ = true;
    std::vector<Functor> functors;

    //缩小临界区，避免死锁
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
        printf("error: EventLoop::wakeup()\n");
    }

}

void EventLoop::handleRead()
{
    uint64_t one = 1;
    ssize_t n = read(wakeupFd_, &one, sizeof one);
    if( n != sizeof one )
    {
        printf("error: EventLoop::handleRead()\n");

    }
}

void EventLoop::removeChannel(Channel* channel)
{
    assertInLoopThread();
    poller_->removeChannel(channel);
}
