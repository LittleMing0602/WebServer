#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <memory>
#include "Socket.h"
#include "Channel.h"
#include "InetAdress.h"
#include "Callback.h"

class EventLoop;

class TcpConnection: public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(EventLoop* loop, int sockfd, InetAdress& localAddr, InetAdress& peerAddr_);
    ~TcpConnection() {}

    void handleRead();
    void setMessageCallback_(const MessageCallback& cb)
    { messageCallback_ = cb; }
    void setConnectionCallback(const ConnectionCallback& cb)
    { connectionCallback_ = cb; }


private:
    EventLoop* loop_; 
    std::unique_ptr<Channel> channel_;
    std::unique_ptr<Socket> socket_;
    InetAdress localAddr_;
    InetAdress peerAddr_;
    MessageCallback messageCallback_;
    ConnectionCallback connectionCallback_;

};

#endif
