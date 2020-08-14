#include "EpollPoller.h"
#include "../log/Logging.h"
#include "Channel.h"
#include <sys/epoll.h>

static const int kNew = -1; // channel不在epoll树中，也不在ChannelMap中
static const int kAdded = 1; // chanel在epoll树中，也在ChannelMap中
static const int kDeleted = 2; // channel不在epoll中，但在ChannelMap中


EpollPoller::EpollPoller(EventLoop* loop)
    :Poller(loop),
     epollfd_(epoll_create1(EPOLL_CLOEXEC)),
     events_(kInitEventListSize)
{
    if(epollfd_ < 0)
    {
        LOG_SYSERR << "EpollPoller::EPollPoller";
    }
}

EpollPoller::~EpollPoller()
{
    close(epollfd_);
}

TimeStamp EpollPoller::poll(int timeoutMs, ChannelList* activeChannels)
{
    int numEvents = epoll_wait(epollfd_, &*events_.begin(), static_cast<int>(events_.size()), timeoutMs);

    TimeStamp now(TimeStamp::now());

    if(numEvents > 0)
    {
        fillActiveChannels(numEvents, activeChannels);
        if(static_cast<size_t>(numEvents) == events_.size())
        {
            events_.resize(events_.size() * 2);
        }
    }
    else if(numEvents == 0)
    {
        LOG_TRACE << "nothing happended";
    }
    else
    {
        LOG_SYSERR << "EpollPoller::poll()";
    }
    return now;
}

void EpollPoller::fillActiveChannels(int numEvents, ChannelList* activeChannels) const
{
    for(int i = 0; i < numEvents; ++i)
    {
        Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
        channel->setRevents(events_[i].events);
        activeChannels->push_back(channel);
    }
}

//添加或更新poller中的channel
void EpollPoller::updateChannel(Channel* channel)
{
    Poller::assertInLoopThread();
    const int index = channel->index();

    if(index == kNew || index == kDeleted)
    {
        int fd = channel->fd();
        if(index == kNew)
        {
            channels_[fd] = channel;
        }
        else // 因为标记为kDelete的channel还在channel_中，所以不许要添加
        {

        }
        channel->setIndex(kAdded); // 转态改为kAdded
        update(EPOLL_CTL_ADD, channel);
    }
    else // 如果channel为kAdded, 则修改
    {
        if(channel->isNoneEvent())
        {
            update(EPOLL_CTL_DEL, channel);
            channel->setIndex(kDeleted); // 标记为kDelete，从epoll树中删除，但是还在channels_中
        }
        else
        {
            update(EPOLL_CTL_MOD, channel);
        }
    }
}

// 删除channel，有两个地方要删除，channels_ 和 epoll树中
void EpollPoller::removeChannel(Channel* channel)
{
    int fd = channel->fd();
    int index = channel->index();
    size_t n = channels_.erase(fd);
    (void)n;

    if(index == kAdded)
    {
        update(EPOLL_CTL_DEL, channel);

    }
    channel->setIndex(kNew);
}

void EpollPoller::update(int operation, Channel* channel)
{
    struct epoll_event event;
    bzero(&event, sizeof event);
    event.events = channel->events();
    event.data.ptr = channel;
    int fd = channel->fd();
    if(epoll_ctl(epollfd_, operation, fd, &event) < 0)
    {
        if(operation == EPOLL_CTL_DEL)
        {
            LOG_SYSERR << "epoll_ctl op=" << operation << " fd=" << fd;
        }
        else
        {
            LOG_SYSFATAL << "epoll_ctl op=" << operation << " fd=" << fd;
        }
    }
}
