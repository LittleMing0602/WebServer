#ifndef CHANNEL_H
#define CHANNEL_H

#include <functional>
#include "Callback.h"

class EventLoop;

class Channel
{
public:
    Channel(EventLoop* loop, int fd):
        loop_(loop), 
        fd_(fd), 
        events_(0), 
        revents_(0),
        index_(-1) 
    {  }
    ~Channel() {}

    void setWriteCallback(const EventCallback& cb)
    { writeCallback_ = cb; }

    void setReadCallback(const ReadEventCallback& cb)
    { readCallback_ = cb; }

    void setErrorCallback(const EventCallback& cb)
    { errorCallback_ = cb; }
        
    void setCloseCallback(const EventCallback& cb)
    { closeCallback_ = cb; }
    
    void enableReading() { events_ |= kReadEvent; update();}

    void enableWriting() { events_ |= kWriteEvent; update(); }

    void disableWriting() { events_ &= ~kWriteEvent; update(); }

    void disableAll() { events_ = kNoneEvent; update();}

    void handleEvent(TimeStamp receiveTime);

    void setRevents(int events) { revents_ = events; }

    int index() const { return index_; }
    int fd() const { return fd_; }
    int events() const { return events_; }
    void setIndex(int index) { index_ = index; }
    bool isNoneEvent() const { return events_ == kNoneEvent; }
    void remove();
    bool isWriting() const { return events_ & kWriteEvent; }

private:
    /* 
    * 被enableReading()调用，表示channel状态改变，需要更新。
    * update让所属的EventLoop去更新channel
    * （当然EventLoop又是让Poller去更新的）
    */
    void update();
    
    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;
    const int fd_;
    int events_;
    int revents_;
    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback errorCallback_;
    EventCallback closeCallback_;
    EventLoop* loop_;
    int index_; //标示自己在pollfds数组中的下标，以便快速通过channel找到pollfd
};



#endif
