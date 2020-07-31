#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <memory>
#include "Socket.h"
#include "Channel.h"
#include "InetAddress.h"
#include "Callback.h"
#include <string>

class EventLoop;

class TcpConnection: public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(const std::string& name, EventLoop* loop, int sockfd, InetAddress& localAddr, InetAddress& peerAddr_);
    ~TcpConnection() {}

    void handleRead();
    void setMessageCallback_(const MessageCallback& cb)
    { messageCallback_ = cb; }
    void setConnectionCallback(const ConnectionCallback& cb)
    { connectionCallback_ = cb; }
    const std::string& name() const { return name_; }
    bool connected() const { return state_ == kConnected; }
    const InetAddress& peerAddr() const { return peerAddr_; }
    const InetAddress& localAddr() const { return localAddr_; }

    void connectEstablished();

private:
    enum StateE { kConnecting, kConnected, };
    void setState(StateE s) { state_ = s; }
    EventLoop* loop_; 
    std::unique_ptr<Channel> channel_;
    std::unique_ptr<Socket> socket_;
    InetAddress localAddr_;
    InetAddress peerAddr_;
    MessageCallback messageCallback_;
    ConnectionCallback connectionCallback_;
    const std::string name_;
    bool connected_;
    StateE state_;
};

#endif
