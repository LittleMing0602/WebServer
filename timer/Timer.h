#ifndef TIMER_H
#define TIMER_H

#include <time.h>
#include <functional>
#include "TimeStamp.h"

class Timer
{
public:
    typedef std::function<void()> TimerCallback;
public:
    Timer(const TimerCallback& cb, TimeStamp when, double interval):
        callback_(cb),
        expiration_(when),
        interval_(interval),
        repeat_(interval>0.0)
    { }

    void run() const
    {
        callback_();
    }

    TimeStamp expiration() const
    {
        return expiration_;
    }

    bool repeat() const 
    {
        return repeat_;
    }

    void restart(TimeStamp now)
    {
        if(repeat_)
        {
            expiration_ = addTime(now, interval_);
        }
        else
        {
            expiration_ = TimeStamp::invalid();
        }
    }
    
private:
    TimeStamp expiration_;
    const TimerCallback callback_;
    const double interval_; // 超时时间间隔，如果是一次性定时器，则设为0
    const bool repeat_; // 是否重复
};

#endif
