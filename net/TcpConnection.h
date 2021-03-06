#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <memory>
#include "Socket.h"
#include "Channel.h"
#include "InetAddress.h"
#include "Callback.h"
#include "Buffer.h"
#include "../timer/TimeStamp.h"
#include <string>
#include <boost/any.hpp>

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

    void setWriteCompleteCallback(const WriteCompleteCallback& cb)
    { writeCompleteCallback_ = cb; }
    
    const std::string& name() const { return name_; }
    
    bool connected() const { return state_ == kConnected; }
    
    const InetAddress& peerAddr() const { return peerAddr_; }
    
    const InetAddress& localAddr() const { return localAddr_; }
    
    EventLoop* getLoop() const { return loop_; }

    void connectEstablished();  // 连接已建立，注册读事件，对调connectionCallback_。 由TcpServer的newConnection通过runInLoop调用
    
    void connectDestroyed();

    // 可跨线程调用
    void send(const std::string& message);

    void send(Buffer* message);

    // 可跨线程调用
    void shutdown();

    void setTcpNoDelay(bool on);

    void setContext(const boost:: any& context)
    { context_ = context; }

    const boost::any* getContext() const
    { return &context_; }

    boost::any* getMutableContext()
    { return &context_; }

private:
    enum StateE { kConnecting, kConnected, kDisconnecting, kDisconnected, };
    void setState(StateE s) { state_ = s; }
    void handleRead(TimeStamp receiveTime);
    void handleWrite();
    void handleClose();
    void handleError();
    void sendInLoop(const std::string& message);
    void sendInLoop(const void* data, size_t len);
    void shutdownInLoop();
    
    StateE state_;
    EventLoop* loop_; 
    std::unique_ptr<Socket> socket_;
    std::unique_ptr<Channel> channel_;
    InetAddress localAddr_;  // 本地地址
    InetAddress peerAddr_;  // 对端地址
    MessageCallback messageCallback_;  // 绑定的是TcpServer中的messageCallback_
    ConnectionCallback connectionCallback_; // 绑定的是TcpServer中的connectionCallback_, 由connectionEstablished()调用
    CloseCallback closeCallback_;  // 绑定的是TcpServer中的removeConnection
    WriteCompleteCallback writeCompleteCallback_;
    const std::string name_;
    bool connected_;
    Buffer inputBuffer_;
    Buffer outputBuffer_;
    boost::any context_;
};

#endif
