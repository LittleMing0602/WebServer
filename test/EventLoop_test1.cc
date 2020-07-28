#include "../net/EventLoop.h"
#include "../base/Thread.h"

void func()
{
    printf("func() threadid = %d, tid = %d\n", getpid(), tid());
    EventLoop loop;
    loop.loop();
}

int main()
{
	printf("func() threadid = %d, tid = %d\n", getpid(), tid());
    EventLoop loop;
    Thread thread(func);
    thread.start();
    loop.loop();
    
    pthread_exit(NULL);
    return 0;
}

