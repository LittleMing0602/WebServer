#include "Channel.h"
#include <poll.h>
#include "EventLoop.h"

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

void Channel::handleEvent()
{
    if(revents_ & POLLNVAL)
    {
        // 输出日志        
    }
    if(revents_ & (POLLERR | POLLNVAL))
    {
        if(errorCallback_) errorCallback_();
    }
    if(revents_ & (POLLIN | POLLPRI | POLLRDHUP))
    {
        if(readCallback_) readCallback_();
    }
    if(revents_ & POLLOUT)
    {
        if(writeCallback_) writeCallback_();
    }
}

void Channel::update()
{
    loop_->updateChannel(this);
}

