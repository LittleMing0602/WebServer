#ifndef EPOLL_POLLER_H
#define EPOLL_POLLER_H

#include "Poller.h"
#include <map>
#include <vector>

struct epoll_event;

class EpollPoller : public Poller
{
public:
    EpollPoller(EventLoop* loop);
    virtual ~EpollPoller();

    virtual TimeStamp poll(int timeoutMs, ChannelList *activeChannels);
    virtual void updateChannel(Channel* channel);
    virtual void removeChannel(Channel* channel);

private:
    static const int kInitEventListSize = 16;

    void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;
    void update(int operation, Channel* channel);

    typedef std::vector<struct epoll_event> EventList;
    typedef std::map<int, Channel*> ChannelMap;

    int epollfd_;
    EventList events_;
    ChannelMap channels_;
};

#endif
