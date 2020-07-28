#include "../net/EventLoop.h"
#include "../base/Thread.h"

EventLoop* g_loop;

void threadFunc()
{
    printf("threadFunc(): pid = %d, tid = %d\n", 
           getpid(), tid());
    g_loop->loop();
}

int main()
{
    EventLoop loop;
    g_loop = &loop;
    Thread t(threadFunc);
    t.start();
    t.join();
}
