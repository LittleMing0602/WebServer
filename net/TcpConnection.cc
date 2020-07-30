#include "TcpConnection.h"
#include <memory>
#include <functional>
#include <unistd.h>

TcpConnection::TcpConnection(EventLoop* loop, int sockfd, InetAdress& localAddr, InetAdress& peerAddr):
    loop_(loop),
    socket_(new Socket(sockfd)),
    localAddr_(localAddr),
    peerAddr_(peerAddr),
    channel_(new Channel(loop, sockfd))
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
