#include "CurrentThread.h"
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <pthread.h>

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
        int n = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
        assert(n == 6); (void)n;
    }
}

bool isMainThread()
{
    return tid() == getpid();
}

void afterFork()
{
    t_cachedTid = 0;
    t_threadName = "main";
    tid();
}

class ThreadNameInitializer
{
public:
    ThreadNameInitializer()
    {
        t_threadName = "main";
        tid();
        pthread_atfork(NULL, NULL, &afterFork);
    }
};

ThreadNameInitializer init;
