#include "TcpServer.h"
#include <functional>
#include <string.h>
#include <sys/socket.h>
#include "TcpConnection.h"

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

TcpServer::TcpServer(EventLoop* loop, const InetAdress& addr):
    loop_(loop),
    acceptor_(new Acceptor(loop, addr)),
    nextConnId_(1)
{
    acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this,std::placeholders::_1,std::placeholders:: _2));
}

void start()
{
    if(acceptor_->listennign())
}

void TcpServer::newConnection(int sockfd, InetAdress& peerAddr)
{
    char buf[32];
    snprintf(buf, sizeof buf, "#%d", nextConnId_);
    std::string connName = buf;

    InetAdress localAddr(getLocalAddr(sockfd));
    TcpConnectionPtr conn(new TcpConnection(loop_, sockfd, localAddr, peerAddr));
    connections_[connName] = conn;
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback_(messagesCallback_);

}
