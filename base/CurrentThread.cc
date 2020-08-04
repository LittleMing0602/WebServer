#include "CurrentThread.h"
#include <unistd.h>
#include <sys/syscall.h>

__thread int t_cachedTid = 0;
__thread char t_tidString[32];
__thread const char* t_threadName="unknown";

pid_t gettid()
{
    return static_cast<pid_t>(syscall(SYS_gettid));
}

void cacheTid()
{
    if(t_cachedTid == 0)
    {
        t_cachedTid = gettid();
    }
}

bool isMainThread()
{
    return tid() == getpid();
}
