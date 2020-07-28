#ifndef CURRENT_THREAD_H
#define CURRENT_THREAD_H


extern __thread int t_cachedTid;

void cacheTid();

inline int tid()
{
    if(t_cachedTid == 0)
    {
        cacheTid();
    }
    return t_cachedTid;
}

#endif
