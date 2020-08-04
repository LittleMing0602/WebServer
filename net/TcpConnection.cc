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
    channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this, std::placeholders::_1));
}

void TcpConnection::handleRead(TimeStamp receiveTime)
{
    int savedError = 0;
    ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedError);
    if(n > 0)
    {
        messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
    }
    else if(n == 0)
    {
        handleClose();
    }
    else
    {
        errno = savedError;
        // LOG_SYSERR
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
