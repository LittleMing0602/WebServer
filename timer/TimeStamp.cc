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
