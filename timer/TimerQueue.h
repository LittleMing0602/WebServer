#ifndef TIMER_QUEUE_H
#define TIMER_QUEUE_H

#include "Timer.h"
#include "TimeStamp.h"
#include "../net/Channel.h"
#include <set>
#include <functional>

#include <vector>

class EventLoop;

typedef std::function<void()> TimerCallback;



class TimerQueue
{
public:
    TimerQueue(EventLoop* loop);
    ~TimerQueue() {}

    void addTimer(const TimerCallback& cb, TimeStamp when, double interval);
    

private:
    typedef std::pair<TimeStamp, Timer*> Entry;
    typedef std::set<Entry> TimerList;

    void handleRead();
    void addTimerInLoop(Timer* timer);
    bool insert(Timer* timer);
    std::vector<Entry> getExpired(TimeStamp now);
    void reset(const std::vector<Entry>& expired, TimeStamp now);
    
    EventLoop* loop_;
    const int timerfd_;  // 使用timerfd作为定时器
    Channel timerfdChannel_;
    TimerList timers_;  // 用红黑树管理，以时间排序



};


#endif

