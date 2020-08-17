#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <unistd.h>
#include "../base/CurrentThread.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <memory>
// #include "Poller.h"
#include "../lock/Mutex.h"
#include "Channel.h"
#include <functional>
#include "../timer/TimeStamp.h"

class Poller;

/*
 * one loop per thread
 */

class EventLoop
{
public:
    typedef std::function<void()> Functor;

    EventLoop(); 
    ~EventLoop();
    
    void loop();
    void assertInLoopThread()
    {
        if(!isInLoopThread())
        {
            //log error
            printf("error: is not in loop thread\n");
            exit(1);
        }

    }

    bool isInLoopThread() const
    {
        return threadId_ == tid();
    }

    void quit()
    {
        quit_ = true;
        if(!isInLoopThread())
        {
            wakeup();
        }
    }

    void updateChannel(Channel* channel);

    void runInLoop(const Functor& cb);
    
    void queueInLoop(const Functor& cb);
    
    void removeChannel(Channel* channel);

private:
    typedef std::vector<Channel*> ChannelList;

    std::unique_ptr<Poller> poller_;
    bool quit_;
    const pid_t threadId_;
    ChannelList activeChannels_;
    bool looping_;
    bool callingPendingFunctors_;
    int wakeupFd_;
    std::unique_ptr<Channel> wakeupChannel_;
    MutexLock mutex_;
    std::vector<Functor> pendingFunctors_;
    TimeStamp pollReturnTime_;

    void handleRead();
    void doPendingFunctors();
    void wakeup();
    
};

#endif
