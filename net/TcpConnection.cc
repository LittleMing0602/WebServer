#include "TcpConnection.h"
#include <memory>
#include <functional>
#include <unistd.h>
#include "EventLoop.h"

TcpConnection::TcpConnection(const std::string& name, EventLoop* loop, int sockfd, InetAddress& localAddr, InetAddress& peerAddr):
    loop_(loop),
    state_(kConnecting),
    socket_(new Socket(sockfd)),
    localAddr_(localAddr),
    peerAddr_(peerAddr),
    channel_(new Channel(loop, sockfd)),
    name_(name)
{
    // connection可读的时候执行的回调函数
    channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this));
}

void TcpConnection::handleRead()
{
    char buf[65535];
    ssize_t n = read(channel_->fd(), buf, sizeof buf);
    messageCallback_(shared_from_this(), buf, n);
}

void TcpConnection::connectEstablished()
{
    loop_->assertInLoopThread();
    setState(kConnected);
    channel_->enableReading();
    connectionCallback_(shared_from_this());
}
