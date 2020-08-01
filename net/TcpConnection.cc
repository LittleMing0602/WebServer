#include "TcpConnection.h"
#include <memory>
#include <functional>
#include <unistd.h>
#include "EventLoop.h"

int getSocketError(int sockfd)
{
    int optval;
    socklen_t optlen = sizeof optval;
    if(getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0)
    {
        return errno;
    }
    else
    {
        return optval;
    }
}

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
    if(n > 0)
    {
        messageCallback_(shared_from_this(), buf, n);
    }
    else if(n == 0)
    {
        handleClose();
    }
    else
    {
        handleError();
    }
    
}

void TcpConnection::handleWrite()
{

}

void TcpConnection::handleClose()
{
    channel_->disableAll();
    closeCallback_(shared_from_this());  // 回调TcpServer中的removeConnection
}


void TcpConnection::handleError()
{
    int err = getSocketError(channel_->fd());
    //LOG_ERROR
}

// 连接已建立，设置channel的读事件，回调用户的connectioncallback
void TcpConnection::connectEstablished()
{
    loop_->assertInLoopThread();
    setState(kConnected);
    channel_->enableReading();
    connectionCallback_(shared_from_this());
}

void TcpConnection::connectDestroyed()
{
    setState(kDisconnected);
    channel_->disableAll();
    connectionCallback_(shared_from_this()); // ?
    channel_->remove();
}
