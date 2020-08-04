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

    void connectEstablished();  // �����ѽ�����ע����¼����Ե�connectionCallback_�� ��TcpServer��newConnectionͨ��runInLoop����
    
    void connectDestroyed();

    // �ɿ��̵߳���
    void send(const std::string& message);

    // �ɿ��̵߳���
    void shutdown();

    void setTcpNoDelay(bool on);

private:
    enum StateE { kConnecting, kConnected, kDisconnecting, kDisconnected, };
    void setState(StateE s) { state_ = s; }
    void handleRead(TimeStamp receiveTime);
    void handleWrite();
    void handleClose();
    void handleError();
    void sendInLoop(const std::string& message);
    void shutdownInLoop();
    
    EventLoop* loop_; 
    std::unique_ptr<Channel> channel_;
    std::unique_ptr<Socket> socket_;
    InetAddress localAddr_;  // ���ص�ַ
    InetAddress peerAddr_;  // �Զ˵�ַ
    MessageCallback messageCallback_;  // �󶨵���TcpServer�е�messageCallback_
    ConnectionCallback connectionCallback_; // �󶨵���TcpServer�е�connectionCallback_, ��connectionEstablished()����
    CloseCallback closeCallback_;  // �󶨵���TcpServer�е�removeConnection
    WriteCompleteCallback writeCompleteCallback_;
    const std::string name_;
    bool connected_;
    StateE state_;
    Buffer inputBuffer_;
    Buffer outputBuffer_;
};

#endif
