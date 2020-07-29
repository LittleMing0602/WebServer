#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "Channel.h"
#include <functional>

class EventLoop;
class Socket;

class Acceptor{
public:
    typedef std::function<void(int, struct sockaddr_in&)> NewConnectionCallback;

    Acceptor(EventLoop* loop, const struct sockaddr_in& addr);

    void setNewConnectionCallback(const NewConnectionCallback& cb)
    { newConnectionCallback_ = cb; }

    bool listening() const { return listening_; }
    void listen();

private:
    EventLoop* loop_;
    Socket socket_;
    Channel channel_;
    NewConnectionCallback newConnectionCallback_;
    bool listening_;

    void handleRead();
};

#endif
