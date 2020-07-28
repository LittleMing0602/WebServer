#include "TimerQueue.h"
#include "../net/EventLoop.h"
#include "functional"
#include "string.h"

void readTimerfd(int timerfd, TimeStamp now)
{
    uint64_t howmany;
    ssize_t n = read(timerfd, &howmany, sizeof howmany);
    //log
    if( n != sizeof howmany )
    {
        printf("error readTimerfd() \n");
        exit(1);
    }
}

struct timespec howLong(TimeStamp when)
{
    int64_t microseconds = when.microSeconds() - TimeStamp::now().microSeconds();
    if(microseconds < 100)
    {
        microseconds = 100;
    }

    struct timespec ts;
    ts.tv_sec = static_cast<time_t>(microseconds / TimeStamp::kMicrosecondsPerSecond);
    ts.tv_nsec = static_cast<long>(microseconds % TimeStamp::kMicrosecondsPerSecond * 1000);

    return ts;
}

void resetTimerfd(int timerfd, TimeStamp expiration)
{
    struct itimerspec newValue;
    struct itimerspec oldValue;
    bzero(&newValue, sizeof newValue);
    bzero(&oldValue, sizeof oldValue);

    newValue.it_value = howLong(expiration);
    int ret = timerfd_settime(timerfd, 0, &newValue, &oldValue);
    if(ret)
    {
        printf("error timerfd_settime()\n");
        exit(1);
    }
}

TimerQueue::TimerQueue(EventLoop* loop): 
    loop_(loop),
    timerfd_(createTimerfd()),
    timerfdChannel_(loop, timerfd_),
    timers_()
{
    timerfdChannel_.setReadCallback(std::bind(&TimerQueue::handleRead, this));
    timerfdChannel_.enableReading();
}


void TimerQueue::addTimer(const TimerCallback& cb, TimeStamp when, double interval)
{
    Timer* timer = new Timer(cb, when, interval);

    loop_->runInLoop(std::bind(&TimerQueue::addTimerInLoop, this, timer));
}


void TimerQueue::addTimerInLoop(Timer* timer)
{
    loop_->assertInLoopThread();
    bool earliestChanged = insert(timer);

    if(earliestChanged)
    {
        resetTimerfd(timerfd_, timer->expiration());
    }
        
}

bool TimerQueue::insert(Timer* timer)
{
    loop_->assertInLoopThread();
    bool earliestChanged = false;

    TimeStamp when = timer->expiration();
    TimerList::iterator it = timers_.begin();
    if(it == timers_.end() || when < it->first)
    {
        earliestChanged = true;
    }
    
    //插入到timers_中
    std::pair<TimerList::iterator, bool> result = timers_.insert(Entry(when, timer));
    return earliestChanged;
}

void TimerQueue::handleRead()
{
    TimeStamp now = TimeStamp::now();
    readTimerfd(timerfd_, now);

    std::vector<Entry> expired = getExpired(now);
    
    for(std::vector<Entry>::iterator it = expired.begin(); it != expired.end(); ++it)
    {
        it->second->run();
    }

    reset(expired, now);
    
}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(TimeStamp now)
{
    std::vector<Entry> expired;
    Entry sentry(now, reinterpret_cast<Timer*>(UINTPTR_MAX));

    //返回第一个>=sentry的元素的迭代器
    TimerList::iterator end = timers_.lower_bound(sentry);

    //将到期的定时器插入到返回结果中
    std::copy(timers_.begin(), end, back_inserter(expired));
    timers_.erase(timers_.begin(), end);

    return expired;
}

void TimerQueue::reset(const std::vector<Entry>& expired, TimeStamp now)
{
    for(std::vector<Entry>::const_iterator it = expired.begin(); it != expired.end(); ++it)
    {
        if(it->second->repeat())
        {
            it->second->restart(now);
            insert(it->second);
        }
        else
        {
            delete it->second;
        }
    }

    TimeStamp nextExpire;

    if(!timers_.empty())
    {
        nextExpire = timers_.begin()->second->expiration();
    }

    if(nextExpire.valid())
    {
        resetTimerfd(timerfd_, nextExpire);
    }
}
