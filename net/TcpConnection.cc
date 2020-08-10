#include <functional>
#include <memory>
#include <unistd.h>
#include "EventLoop.h"
#include "TcpConnection.h"
#include "../log/Logging.h"

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
    state_(kConnecting),
    loop_(loop),
    socket_(new Socket(sockfd)),
    channel_(new Channel(loop, sockfd)),
    localAddr_(localAddr),
    peerAddr_(peerAddr),
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

// 处理写事件
void TcpConnection::handleWrite()
{
    loop_->assertInLoopThread();
    if(channel_->isWriting())
    {
        // 发送outPut_中保存的数据
        ssize_t n = write(channel_->fd(), outputBuffer_.peek(), outputBuffer_.readableBytes());
        if(n > 0)
        {
            outputBuffer_.retrieve(n);
            // 如果数据全发完了，要取消读事件，否则一直忙询
            if(outputBuffer_.readableBytes() == 0)
            {
                channel_->disableWriting(); // 取消写事件
                
                if(writeCompleteCallback_) // 回调writeCompleteCallback_
                {
                    loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
                }

                if(state_ == kDisconnecting) // 如果想要关闭连接却还有内容发完，那么发完后就关闭连接
                {
                    shutdownInLoop();
                }
            }
            else // 如果没发完，那么不用取消读事件，等待下一次loop来执行handleWrite_
            {
                LOG_TRACE << "I'm going to write more data";
                // printf("I'm going to write more data\n");
            }
        }
        else
        {
            LOG_SYSERR << "TcpConnection::handleWrite error";
            // printf("TcpConnection::handleWrite error\n");
        }
    }
    else
    {
        LOG_TRACE << "connection is down, no more writing";
        // printf("connection is down, no more writing\n");
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
            loop_->runInLoop(std::bind((void(TcpConnection::*)(const std::string&))&TcpConnection::sendInLoop, this, message));
        }
    }
}

void TcpConnection::send(Buffer* buf)
{
    if(state_ == kConnected)
    {
        if(loop_->isInLoopThread())
        {
            sendInLoop(buf->peek(), buf->readableBytes());
            buf->retrieveAll();
        }

        else
        {
            loop_->runInLoop(std::bind((void(TcpConnection::*)(const std::string&))&TcpConnection::sendInLoop, 
                                       this, 
                                       buf->retrieveAllAsString()));
        }
    }
}


void TcpConnection::sendInLoop(const std::string& message)
{
    sendInLoop(message.data(), message.size());
}

// 首先尝试直接发送数据，如果一次发送完毕就不会启用writeCallback_
// 如果只发送了部分数据，则把剩余的数据放入outputBuffer_, 并开始关注writable_事件，在handleWrite中发送剩余数据
void TcpConnection::sendInLoop(const void* data, size_t len)
{
    loop_->assertInLoopThread();
    
    const char* buf = static_cast<const char*>(data);
    ssize_t nwrote = 0;

    // 此时还没有关注写事件，表示首次发送数据，先尝试一次发送全部数据
    if(!channel_->isWriting() && outputBuffer_.readableBytes() == 0)
    {
        nwrote = write(channel_->fd(), buf, len);
        if(nwrote >= 0)
        {
            if(static_cast<size_t>(nwrote) < len)  // 数据没发完
            {
                LOG_TRACE << "I'm going to write more data";
                //printf("I'm going to write more data\n");
            }
            else if(writeCompleteCallback_)  // 数据一次发完了，回调writeCompleteCallback_
            {
                loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
            }
        }
        else
        {
            nwrote = 0;
            if(errno != EWOULDBLOCK)
            {
                LOG_SYSERR << "TcpConnection::sendInLoop error";
                // printf("TcpConnection::sendInLoop error\n");
            }
        }
    }

    assert(nwrote >= 0);
    // 一次性没有发送完，就将剩余的数据保存在outPut_中，并注册写事件，留给handleWrite_来做
    if(static_cast<size_t>(nwrote) < len)
    {
        outputBuffer_.append(buf + nwrote, len - nwrote);
        if(!channel_->isWriting())
        {
            channel_->enableWriting();
        }
    }
}

void TcpConnection::setTcpNoDelay(bool on)
{
    socket_->setTcpNoDelay(on);
}
