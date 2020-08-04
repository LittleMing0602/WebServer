#include "TcpServer.h"
#include <functional>
#include <string.h>
#include <sys/socket.h>
#include "TcpConnection.h"
#include "EventLoop.h"

struct sockaddr_in getLocalAddr(int sockfd)
{
    struct sockaddr_in localAddr;
    bzero(&localAddr, sizeof localAddr);
    socklen_t addrlen = sizeof localAddr;
    if(getsockname(sockfd, static_cast<sockaddr*>((void*)&localAddr), &addrlen) < 0)
    {
        printf("error getsockname\n");
        exit(1);
    }
}

TcpServer::TcpServer(EventLoop* loop, const InetAddress& addr):
    loop_(loop),
    acceptor_(new Acceptor(loop, addr)),
    threadPool_(new EventLoopThreadPool(loop)),
    nextConnId_(1),
    started_(false)
{
    acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this,std::placeholders::_1,std::placeholders:: _2));
}

TcpServer::~TcpServer()
{
    loop_->assertInLoopThread();
    
    for(ConnectionMap::iterator it = connections_.begin(); it != connections_.end(); ++it)
    {
        TcpConnectionPtr conn = it->second;
        it->second.reset();
        conn->getLoop()->runInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
        conn.reset();
    }
}

void TcpServer::start()
{
    if(!started_)
    {
        started_ = true;
        threadPool_->start(threadInitCallback_);
    }
    if(!acceptor_->listening())
    {
        loop_->runInLoop(std::bind(&Acceptor::listen, acceptor_.get()));
    }
}

void TcpServer::newConnection(int sockfd, InetAddress& peerAddr)
{
    char buf[32];
    snprintf(buf, sizeof buf, "#%d", nextConnId_);
    std::string connName = buf;
    ++nextConnId_;
    InetAddress localAddr(getLocalAddr(sockfd));
    EventLoop* ioLoop = threadPool_->getNextLoop();
    TcpConnectionPtr conn(new TcpConnection(connName, ioLoop, sockfd, localAddr, peerAddr));
    connections_[connName] = conn;
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setCloseCallback(std::bind(&TcpServer::removeConnection, this, std::placeholders::_1));
    conn->setWriteCompleteCallback(writeCompleteCallback_);
    ioLoop->runInLoop(std::bind(&TcpConnection::connectEstablished, conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
    /*
    loop_->assertInLoopThread();
    size_t n = connections_.erase(conn->name());
    loop_->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
    */
    loop_->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn)
{
    loop_->assertInLoopThread();
    size_t n = connections_.erase(conn->name());
    assert(n == 1);
    EventLoop* ioLoop = conn->getLoop();
    ioLoop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
}

void TcpServer::setThreadNum(int numThreads)
{
    assert(numThreads >= 0);
    threadPool_->setThreadNum(numThreads);
}