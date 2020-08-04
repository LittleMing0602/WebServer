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
    channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
    channel_->setCloseCallback(std::bind(&TcpConnection::handleClose, this));
    channel_->setErrorCallback(std::bind(&TcpConnection::handleError, this));
    socket_->setKeepAlive(true);
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
    loop_->assertInLoopThread();
    if(channel_->isWriting())
    {
        ssize_t n = write(channel_->fd(), outputBuffer_.peek(), outputBuffer_.readableBytes());
        if(n > 0)
        {
            outputBuffer_.retrieve(n);
            if(outputBuffer_.readableBytes() == 0)
            {
                channel_->disableWriting();
                if(state_ == kDisconnecting)
                {
                    shutdownInLoop();
                }
            }
            else
            {
                //LOG_TRACE
                printf("I'm going to write more data\n");
            }
        }
        else
        {
                printf("TcpConnection::handleWrite error\n");
                exit(1);
        }
    }
    else
    {
        printf("connection is down, no more writing\n");
    }
}

void TcpConnection::handleClose()
{
    setState(kDisconnected);
    channel_->disableAll();
    
    connectionCallback_(shared_from_this());
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
    if(state_ == kConnected)
    {
        setState(kDisconnected);
        channel_->disableAll();
        connectionCallback_(shared_from_this()); // ?
    }
    
    channel_->remove();
}

void TcpConnection::shutdown()
{
    if(state_ == kConnected)
    {
        setState(kDisconnecting);
        loop_->runInLoop(std::bind(&TcpConnection::shutdownInLoop, this));
    }
}

void TcpConnection::shutdownInLoop()
{
    loop_->assertInLoopThread();
    if(!channel_->isWriting())
    {
        socket_->shutdownWrite();
    }
}

void TcpConnection::sendInLoop(const std::string& message)
{
    loop_->assertInLoopThread();

    ssize_t nwrote = 0;

    if(!channel_->isWriting() && outputBuffer_.readableBytes() == 0)
    {
        nwrote = write(channel_->fd(), message.data(), message.size());
        if(nwrote >= 0)
        {
            if(static_cast<size_t>(nwrote) < message.size())
            {
                //LOG_TRACE
                printf("I'm going to write more data\n");
            }
        }
        else
        {
            nwrote = 0;
            if(errno != EWOULDBLOCK)
            {
                //LOG_SYSERR
                printf("TcpConnection::sendInLoop error\n");
                exit(1);
            }
        }
    }

    assert(nwrote >= 0);
    if(static_cast<size_t>(nwrote) < message.size())
    {
        outputBuffer_.append(message.data() + nwrote, message.size() - nwrote);
        if(!channel_->isWriting())
        {
            channel_->enableWriting();
        }
    }
}

void TcpConnection::send(const std::string& message)
{
    if(state_ == kConnected)
    {
        if(loop_->isInLoopThread())
        {
            sendInLoop(message);
        }
        else
        {
            loop_->runInLoop(std::bind(&TcpConnection::sendInLoop, this, message));
        }
    }
}

void TcpConnection::setTcpNoDelay(bool on)
{
    socket_->setTcpNoDelay(on);
}
