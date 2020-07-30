#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "Channel.h"
#include <functional>
#include "Socket.h"
#include "InetAdress.h"

class EventLoop;

class Acceptor{
public:
    typedef std::function<void(int, InetAdress& addr)> NewConnectionCallback;

    Acceptor(EventLoop* loop, const InetAdress& addr);

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
