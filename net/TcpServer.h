#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <memory>
#include "Acceptor.h"
#include "TcpConnection.h"
#include <map>

class TcpServer
{
public:
    TcpServer(EventLoop* loop, const InetAddress& addr);
    ~TcpServer(); 

    void setMessageCallback(const MessageCallback& cb)
    { messageCallback_ = cb; }

    void setConnectionCallback(const ConnectionCallback& cb)
    { connectionCallback_ = cb; }

    void start();

private:
    typedef std::map<std::string, TcpConnectionPtr> ConnectionMap; 

    void newConnection(int sockfd, InetAddress& addr);  // ��Acceptor��newConnectionCallback�󶨣�����TcpConnection����
    void removeConnection(const TcpConnectionPtr& conn);    // ��TcpConnection��closeCallback��
    
    std::unique_ptr<Acceptor> acceptor_;
    EventLoop* loop_;
    ConnectionMap connections_;
    int nextConnId_;

    MessageCallback messageCallback_;
    ConnectionCallback connectionCallback_;
    WriteCompleteCallback writeCompleteCallback_;
};

#endif


