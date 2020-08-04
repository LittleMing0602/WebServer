#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <stdlib.h>
#include <string>

/*
 * 为了实现让定时器精确到微妙级，需要实现一个TimeStamp类完成微秒和秒的转换
 */

#include <time.h>

class TimeStamp
{
public:
    TimeStamp():microseconds_(0) {}

    ~TimeStamp() {}

    explicit TimeStamp(int64_t t):microseconds_(t){}

    bool valid() const {return microseconds_ > 0;}
    
    int64_t microSeconds() {return microseconds_;}

    time_t seconds() { return microseconds_ / kMicrosecondsPerSecond; }

    static TimeStamp now();

    static TimeStamp invalid();

    std::string toString() const;
    
    static const int kMicrosecondsPerSecond = 1000 * 1000;
private:
    int64_t microseconds_; // 微秒时间戳
    

};

// 不需要引用传递，TimeStamp只有一个数据成员，相当于是64位整数，可以放到寄存器中，效率高一些
inline bool operator<(TimeStamp l, TimeStamp r)
{
    return l.microSeconds() < r.microSeconds();
}

inline bool operator==(TimeStamp l, TimeStamp r)
{
    return l.microSeconds() == r.microSeconds();
}

inline double timeDifference(TimeStamp high, TimeStamp low)
{
    int64_t diff = high.microSeconds() - low.microSeconds();
    return static_cast<double>(diff) / TimeStamp::kMicrosecondsPerSecond;
}

inline TimeStamp addTime(TimeStamp t, double seconds)
{
    int64_t delta = static_cast<int64_t>(seconds * TimeStamp::kMicrosecondsPerSecond);
    return TimeStamp(t.microSeconds() + delta);
}

#endif

