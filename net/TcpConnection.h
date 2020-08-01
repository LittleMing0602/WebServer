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

    void setMessageCallback(const MessageCallback& cb)
    { messageCallback_ = cb; }
    
    void setConnectionCallback(const ConnectionCallback& cb)
    { connectionCallback_ = cb; }
    
    void setCloseCallback(const CloseCallback& cb)
    { closeCallback_ = cb; }
    
    const std::string& name() const { return name_; }
    
    bool connected() const { return state_ == kConnected; }
    
    const InetAddress& peerAddr() const { return peerAddr_; }
    
    const InetAddress& localAddr() const { return localAddr_; }
    
    EventLoop* getLoop() const { return loop_; }

    void connectEstablished();  // 连接已建立，注册读事件，对调connectionCallback_。 由TcpServer的newConnection通过runInLoop调用
    
    void connectDestroyed();

private:
    enum StateE { kConnecting, kConnected, kDisconnected, };
    void setState(StateE s) { state_ = s; }
    void handleRead();
    void handleWrite();
    void handleClose();
    void handleError();
    
    EventLoop* loop_; 
    std::unique_ptr<Channel> channel_;
    std::unique_ptr<Socket> socket_;
    InetAddress localAddr_;  // 本地地址
    InetAddress peerAddr_;  // 对端地址
    MessageCallback messageCallback_;  // 绑定的是TcpServer中的messageCallback_
    ConnectionCallback connectionCallback_; // 绑定的是TcpServer中的connectionCallback_, 由connectionEstablished()调用
    CloseCallback closeCallback_;  // 绑定的是TcpServer中的removeConnection
    const std::string name_;
    bool connected_;
    StateE state_;
};

#endif
