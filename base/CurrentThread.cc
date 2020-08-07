#include "CurrentThread.h"
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <pthread.h>

__thread int t_cachedTid = 0;  // 线程tid 是一个线程全局变量，每个线程有一份
__thread char t_tidString[32]; // 线程tid字符串，调用cacheTid后填充
__thread const char* t_threadName="unknown";  // 线程名

// 获取线程号
pid_t gettid()
{
    return static_cast<pid_t>(syscall(SYS_gettid));
}

// 获取线程号并将线程号格式化保存在t_cacheTid 和 t_tidString
void cacheTid()
{
    if(t_cachedTid == 0)
    {
        t_cachedTid = gettid();
        int n = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
        assert(n == 6); (void)n;
    }
}

// 判断是否处于主线程 
bool isMainThread()
{
    return tid() == getpid();  // 判断线程id是否等于tgid（线程组id）
}

// 重置线程私有全局变量
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
        pthread_atfork(NULL, NULL, &afterFork);  // 如果调用fork，则fork完后子进程执行afterFork函数
    }
};

// 通过全局对象init来初始化 t_cachedId, t_tidString, t_threadName
ThreadNameInitializer init; 
