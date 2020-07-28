#ifndef CHANNEL_H
#define CHANNEL_H

#include <functional>

class EventLoop;

class Channel
{
public:
    typedef std::function<void()> EventCallback;
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

    void setReadCallback(const EventCallback& cb)
    { readCallback_ = cb; }

    void setErrorCallback(const EventCallback& cb)
    {
        errorCallback_ = cb;
    }
    
    void enableReading(){ events_ |= kReadEvent; update();}

    void handleEvent();

    void setRevents(int events) { revents_ = events; }

    int index() const { return index_; }
    int fd() const { return fd_; }
    int events() const { return events_; }
    void setIndex(int index) { index_ = index; }
    bool isNoneEvent() const { return events_ == kNoneEvent; }

private:
    void update();
    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;
    const int fd_;
    int events_;
    int revents_;
    EventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback errorCallback_;
    EventLoop* loop_;
    int index_; //标示自己在pollfds数组中的下标，以便快速通过channel找到pollfd
};



#endif
