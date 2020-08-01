#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "Channel.h"
#include <functional>
#include "Socket.h"
#include "InetAddress.h"
#include "Callback.h"

class EventLoop;

class Acceptor{
public:
    Acceptor(EventLoop* loop, const InetAddress& addr);

    void setNewConnectionCallback(const NewConnectionCallback& cb)
    { newConnectionCallback_ = cb; }

    bool listening() const { return listening_; }
    void listen();

private:
    EventLoop* loop_;
    Socket socket_;
    Channel channel_;
    NewConnectionCallback newConnectionCallback_;  // �󶨵���TcpServer��newConnection
    bool listening_;

    void handleRead();  // ����accept��������, ����newConnectionCallback_;
};

#endif
