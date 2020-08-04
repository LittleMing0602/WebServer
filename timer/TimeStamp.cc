#include "TimeStamp.h"
#include <sys/time.h>

TimeStamp TimeStamp::now()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    time_t seconds = tv.tv_sec;
    return TimeStamp(seconds * kMicrosecondsPerSecond + tv.tv_usec);
}

TimeStamp TimeStamp::invalid()
{
    return TimeStamp();
}

std::string TimeStamp::toString() const
{
    char buf[32] = {0};
    time_t seconds = static_cast<time_t>(microseconds_ / kMicrosecondsPerSecond);
    int microseconds = static_cast<int>(microseconds_ % kMicrosecondsPerSecond);

    struct tm tm_time;
    gmtime_r(&seconds, &tm_time);

    snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d", tm_time.tm_year + 1900,
             tm_time.tm_mon + 1, tm_time.tm_mday, tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
             microseconds);

    return buf;
}
