#include "TimerQueue.h"
#include "../net/EventLoop.h"
#include "../log/Logging.h"
#include "functional"
#include "string.h"
#include <sys/timerfd.h>

int createTimerfd()
{
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC|TFD_NONBLOCK);
    if(timerfd < 0)
    {
        printf("failed in timerfd_create\n");
        exit(1);
    }

    return timerfd;
}

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

// 计算when距离现在的时间间隔
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

// 更改timerfd的超时时间
void resetTimerfd(int timerfd, TimeStamp expiration)
{
    struct itimerspec newValue;
    struct itimerspec oldValue;
    bzero(&newValue, sizeof newValue);
    bzero(&oldValue, sizeof oldValue);

    newValue.it_value = howLong(expiration);
    int ret = timerfd_settime(timerfd, 0, &newValue, &oldValue);  //更改timerfd的超时时间
    if(ret)
    {
        // printf("error timerfd_settime()\n");
        LOG_SYSERR << "timerfd_settime()";
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

// 添加定时器，唯一暴露给外面的接口
void TimerQueue::addTimer(const TimerCallback& cb, TimeStamp when, double interval)
{
    Timer* timer = new Timer(cb, when, interval);

    loop_->runInLoop(std::bind(&TimerQueue::addTimerInLoop, this, timer));  // 放到eventloop中运行，可以不用加锁
}


void TimerQueue::addTimerInLoop(Timer* timer)
{
    loop_->assertInLoopThread();
    bool earliestChanged = insert(timer);

    if(earliestChanged)
    {
        resetTimerfd(timerfd_, timer->expiration());  // 如果队首的定时器变了，说明timerfd的激活时间需要更改
    }
        
}

// 插入定时器
bool TimerQueue::insert(Timer* timer)
{
    loop_->assertInLoopThread();
    bool earliestChanged = false;

    TimeStamp when = timer->expiration();  // 该定时器超时时间
    TimerList::iterator it = timers_.begin();
    if(it == timers_.end() || when < it->first)  // 如果定时器队列是空的或者插入的定时器超时时间比队首还早，说明TimerQueue的超时时间需要变更
    {
        earliestChanged = true;
    }
    
    //插入到timers_中
    std::pair<TimerList::iterator, bool> result = timers_.insert(Entry(when, timer));
    return earliestChanged;
}

// 取出所有的以超时的定时器，执行他们的超时动作
void TimerQueue::handleRead()
{
    TimeStamp now = TimeStamp::now();
    readTimerfd(timerfd_, now);

    std::vector<Entry> expired = getExpired(now);
    
    for(std::vector<Entry>::iterator it = expired.begin(); it != expired.end(); ++it)
    {
        it->second->run();
    }

    reset(expired, now);  // 重置已超时的定时器，如果是周期定时器，则需要restart
    
}

// 返回已经超时的定时器，当前时间由参数now传入
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
        if(it->second->repeat())  // 如果是周期定时器，则重启定时器，然后插入到队列中去
        {
            it->second->restart(now);
            insert(it->second);  // 注意这里可能会改变队首的定时器时间
        }
        else
        {
            delete it->second;  // 如果是一次性定时器，没用了，删除
        }
    }

		// 重置以后可能队首的超时时间会变，所以还是要重新设置timerfd的超时时间
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
