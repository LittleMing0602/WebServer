#include "Acceptor.h"
#include "Socket.h"
#include <functional>
#include "EventLoop.h"
#include <unistd.h>
#include <arpa/inet.h>
#include "../log/Logging.h"

Acceptor::Acceptor(EventLoop* loop, const InetAddress& addr):
    loop_(loop),
    socket_(createNonblockingSocket()),
    channel_(loop, socket_.fd()),
    listening_(false)
{
    socket_.setReuseAddr(true);
    socket_.bind(addr);
    channel_.setReadCallback(std::bind(&Acceptor::handleRead, this, std::placeholders::_1));
}

void Acceptor::listen()
{
    LOG_TRACE << "Acceptor starts listening";
    loop_->assertInLoopThread();
    listening_ = true;
    socket_.listen();
    channel_.enableReading(); // 将channel_加入到EventLoop中去
}


void Acceptor::handleRead(TimeStamp receiveTime)
{
    loop_->assertInLoopThread();
    InetAddress addr(0);
    int connfd = socket_.accept(&addr);
    if(connfd >= 0)
    {
        if(newConnectionCallback_)
        {
            newConnectionCallback_(connfd, addr);
        }
        else
        {
            close(connfd);
        }
    }        
}
