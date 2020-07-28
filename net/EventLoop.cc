#include "EventLoop.h"
#include "CurrentThread.h"
#include "poll.h"
#include "Channel.h"

const int kPollTimeMs = 10000;

EventLoop::EventLoop(): 
    threadId_(tid()),
    looping_(false),
    quit_(false),
    poller_(new Poller(this))
{

}

void EventLoop::loop()
{
    assertInLoopThread();
    looping_ = true;
    quit_ = false;

    while(!quit_)    
    {
        activeChannels_.clear();
        poller_->poll(kPollTimeMs, &activeChannels_); 
        for(ChannelList::iterator it = activeChannels_.begin(); it != activeChannels_.end(); ++it)
        {
            (*it)->handleEvent();
        }
        
    }
    looping_ = false; 
}


void EventLoop::updateChannel(Channel* channel)
{
    poller_->updateChannel(channel);
}
