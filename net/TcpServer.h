#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <memory>
#include "Acceptor.h"
#include "TcpConnection.h"
#include <map>

class TcpServer
{
public:
    TcpServer(EventLoop* loop, const InetAdress& addr);
    ~TcpServer() {}

    void setMessageCallback(const MessageCallback& cb)
    { messagesCallback_ = cb; }

    void setConnectionCallback(const ConnectionCallback& cb)
    { connectionCallback_ = cb; }

    void start();

private:
    typedef std::map<std::string, TcpConnectionPtr> ConnectionMap; 

    void newConnection(int sockfd, InetAdress& addr);
    std::unique_ptr<Acceptor> acceptor_;
    EventLoop* loop_;
    ConnectionMap connections_;
    int nextConnId_;

    MessageCallback messagesCallback_;
    ConnectionCallback connectionCallback_;
};

#endif


