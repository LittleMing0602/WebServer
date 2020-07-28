#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <unistd.h>
#include "CurrentThread.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <memory>
#include "Poller.h"

class Channel;

/*
 * one loop per thread
 */

class EventLoop
{
public:
    EventLoop(); 
    ~EventLoop() {}
    
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
    }

    void updateChannel(Channel* channel);

private:
    typedef std::vector<Channel*> ChannelList;
    std::unique_ptr<Poller> poller_;

    bool quit_;
    const pid_t threadId_;
    ChannelList activeChannels_;
    bool looping_;

};

#endif
