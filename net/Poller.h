#ifndef POLLER_H
#define POLLER_H

#include <vector>
#include <map>
#include <poll.h>

class Channel;
class EventLoop;

class Poller
{
public:
    typedef std::vector<Channel*> ChannelList;

    Poller(EventLoop* loop);
    ~Poller() {}

    // 调用poll事件循环，然后调用fillActiveChannels将激活的channel传出去
    void poll(int timeoutMs, ChannelList* activeChannels);
    
    //维护poll的套接字数组，添加和修改或删除channel代表的fd，由EventLoop调用
    void updateChannel(Channel* channel);
    
    void removeChannel(Channel* channel);

private:
    //将激活的套接字添加到activeChannels中
    void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;
    typedef std::vector<struct pollfd> PollFdList;
    typedef std::map<int, Channel*> ChannelMap;

    EventLoop* loop_;  // 所属的EventLoop
    PollFdList pollfds_;
    ChannelMap channels_;
};

#endif
