#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <memory>
#include "Acceptor.h"
#include "TcpConnection.h"
#include <map>
#include "EventLoopThreadPool.h"

class TcpServer
{
public:
    TcpServer(EventLoop* loop, const InetAddress& addr);
    ~TcpServer(); 
    
    //设置线程数量，0表示单线程，1表示出了baseLoop外还有一个线程，N表示有一个N个io线程的线程池，没新建一个连接就会以round-robin的方式放入某个EventLoop线程中去
    void setThreadNum(int numThreads);
    
    void setMessageCallback(const MessageCallback& cb)
    { messageCallback_ = cb; }

    void setConnectionCallback(const ConnectionCallback& cb)
    { connectionCallback_ = cb; }
    
    void setThreadInitCallback(const ThreadInitCallback& cb)
    { threadInitCallback_ = cb; }

    void start();

private:
    typedef std::map<std::string, TcpConnectionPtr> ConnectionMap; 

    void newConnection(int sockfd, InetAddress& addr);  // 与Acceptor的newConnectionCallback绑定，创建TcpConnection对象
    void removeConnection(const TcpConnectionPtr& conn);    // 与TcpConnection的closeCallback绑定
    void removeConnectionInLoop(const TcpConnectionPtr& conn);
    
    bool started_;
    EventLoop* loop_;
    std::unique_ptr<Acceptor> acceptor_;
    std::unique_ptr<EventLoopThreadPool> threadPool_;
    ConnectionMap connections_;
    int nextConnId_;

    MessageCallback messageCallback_;
    ConnectionCallback connectionCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    ThreadInitCallback threadInitCallback_;
};

#endif


