#include <stdio.h>
#include "../net/EventLoop.h"
#include "../net/Channel.h"
#include <sys/timerfd.h>
#include <string.h>
#include "../net/Poller.h"


EventLoop* g_loop;
void timeout(TimeStamp receiveTime)
{
    printf("%s Timeout!\n", receiveTime.toString().c_str());
    g_loop->quit();
}

int main()
{
    printf("%s started\n", TimeStamp::now().toString().c_str());    
    EventLoop loop;
    g_loop = &loop;
    int timerfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK|TFD_CLOEXEC);

    Channel channel(&loop, timerfd);
    channel.setReadCallback(timeout);
    channel.enableReading();

    struct itimerspec howlong;
    bzero(&howlong, sizeof howlong);
    howlong.it_value.tv_sec = 5;
    timerfd_settime(timerfd, 0, &howlong, NULL);

    loop.loop();
    ::close(timerfd);
}
