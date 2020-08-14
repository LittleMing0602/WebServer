#ifndef POLL_POLLER_H
#define POLL_POLLER_H

#include "Poller.h"
#include <vector>
#include <map>
#include <poll.h>
#include "../timer/TimeStamp.h"

class Channel;
class EventLoop;

class PollPoller : public Poller
{
public:
    typedef std::vector<Channel*> ChannelList;

    PollPoller(EventLoop* loop);
    virtual ~PollPoller() {}

    // 调用poll事件循环，然后调用fillActiveChannels将激活的channel传出去
    virtual TimeStamp poll(int timeoutMs, ChannelList* activeChannels);
    
    //维护poll的套接字数组，添加和修改或删除channel代表的fd，由EventLoop调用
    virtual void updateChannel(Channel* channel);
    
    virtual void removeChannel(Channel* channel);

private:
    //将激活的套接字添加到activeChannels中
    void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;

    typedef std::vector<struct pollfd> PollFdList;
    typedef std::map<int, Channel*> ChannelMap;

    PollFdList pollfds_;
    ChannelMap channels_;
};

#endif
