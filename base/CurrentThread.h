#ifndef CURRENT_THREAD_H
#define CURRENT_THREAD_H

extern __thread int t_cachedTid;  // 线程tid 是一个线程全局变量，每个线程有一份
extern __thread char t_tidString[32];  // 线程tid字符串，调用cacheTid后填充
extern __thread const char* t_threadName;  // 线程名

void cacheTid();

// 调用cacheTid，初始化线程全局变量t_cachedTid 和 t_tidString
inline int tid()
{
    if(t_cachedTid == 0)
    {
        cacheTid();
    }
    return t_cachedTid;
}

// 获取线程号字符串
inline const char* tidString()
{
    return t_tidString;
}

// 获取线程名字符串
inline const char* name()
{
    return t_threadName;
}

// 判断是否是主线程
bool isMainThread();

#endif
