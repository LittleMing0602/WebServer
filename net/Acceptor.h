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
    NewConnectionCallback newConnectionCallback_;  // 绑定的是TcpServer的newConnection
    bool listening_;

    void handleRead();  // 调用accept接受连接, 调用newConnectionCallback_;
};

#endif
