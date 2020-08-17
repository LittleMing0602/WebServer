#ifndef POLLER_H
#define POLLER_H

#include "../timer/TimeStamp.h"
#include "EventLoop.h"

class Channel;

class Poller
{
public:
    typedef std::vector<Channel*> ChannelList;

    Poller(EventLoop* loop);
    virtual ~Poller();
    
    virtual TimeStamp poll(int timeoutMs, ChannelList* activeChannels) = 0;
    virtual void updateChannel(Channel* channel) = 0;
    virtual void removeChannel(Channel* channel) = 0;
    
    void assertInLoopThread()
    {
        loop_->assertInLoopThread();
    }

    static Poller* newDefaultPoller(EventLoop* loop);

private:
    EventLoop* loop_;
};

#endif
