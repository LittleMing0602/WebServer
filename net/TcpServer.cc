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
    nextConnId_(1)
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
    TcpConnectionPtr conn(new TcpConnection(connName, loop_, sockfd, localAddr, peerAddr));
    connections_[connName] = conn;
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setCloseCallback(std::bind(&TcpServer::removeConnection, this, std::placeholders::_1));
    conn->setWriteCompleteCallback(writeCompleteCallback_);
    loop_->runInLoop(std::bind(&TcpConnection::connectEstablished, conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
    loop_->assertInLoopThread();
    size_t n = connections_.erase(conn->name());
    loop_->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
}