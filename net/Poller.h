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

    void poll(int timeoutMs, ChannelList* activeChannels);
    void updateChannel(Channel* channel);

private:
    void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;
    typedef std::vector<struct pollfd> PollFdList;
    typedef std::map<int, Channel*> ChannelMap;

    EventLoop* loop_;  // 所属的EventLoop
    PollFdList pollfds_;
    ChannelMap channels_;
};

#endif