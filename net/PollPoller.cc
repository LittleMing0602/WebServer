#include "PollPoller.h"
#include <poll.h>
#include <assert.h>
#include "Channel.h"
#include "../log/Logging.h"

PollPoller::PollPoller(EventLoop* loop)
    : Poller(loop)
{

}

TimeStamp PollPoller::poll(int timeoutMs, ChannelList *activeChannels)
{
    int numEvents = ::poll(&*pollfds_.begin(), pollfds_.size(), timeoutMs);

    TimeStamp now(TimeStamp::now());
    
    if(numEvents > 0)
    {
        fillActiveChannels(numEvents, activeChannels);
    }
    else if(numEvents == 0)
    {
        // printf("nothing happend\n");
        LOG_TRACE << "nothing happended";
    }
    else
    {
        // printf("error: Poller::poll()\n");
        LOG_SYSERR << "PollPoller::poll()";
    }
    return now;
}


void PollPoller::fillActiveChannels(int numEvents, ChannelList* activeChannels) const
{
    for(PollFdList::const_iterator it = pollfds_.begin(); it != pollfds_.end(); ++it)
    {
        if(it->revents > 0)
        {
            --numEvents;
            ChannelMap::const_iterator ch = channels_.find(it->fd);
            assert(ch != channels_.end());
            
            Channel* channel = ch->second;

            channel->setRevents(it->revents);
            activeChannels->push_back(channel);
        }

    }
}

/*
 * 负责维护和更新pollfds_数组,
 * 添加新的Channel的事件复杂度为O(logN)
 * 更新已有的Channel的复杂度为O(1)
*/
void PollPoller::updateChannel(Channel* channel)
{
    if(channel->index() < 0)
    {
        //  新的channel，需要添加到pollfds_中去
        struct pollfd pfd;
        pfd.fd = channel->fd();
        pfd.events = static_cast<short>(channel->events());
        pfd.revents = 0;
        pollfds_.push_back(pfd);
        int index = static_cast<int>(pollfds_.size()) - 1;
        channel->setIndex(index);
        channels_[pfd.fd] = channel;
    }
    else
    {
        // 更新存在的channel
        int index = channel->index();
        struct pollfd& pfd = pollfds_[index];
        pfd.events = static_cast<short>(channel->events());
        pfd.revents = 0;
        if(channel->isNoneEvent())
        {
            pfd.fd = -channel->fd()-1; // 如果channel不关心任何事件，那么将fd设为负数，让poll忽略这项 
        }
    }
}


void PollPoller::removeChannel(Channel* channel)
{
    int index = channel->index();
    const struct pollfd& pfd = pollfds_[index];
    ssize_t n = channels_.erase(channel->fd());
    if(index == pollfds_.size() - 1)
    {
        pollfds_.pop_back();
    }
    else
    {
        int fdAtEnd = pollfds_.back().fd;
        iter_swap(pollfds_.begin() + index, pollfds_.end() - 1);
        if(fdAtEnd < 0)
        {
            fdAtEnd = -fdAtEnd - 1;
        }
        channels_[fdAtEnd]->setIndex(index);
        pollfds_.pop_back();
    }
}
