#include "Acceptor.h"
#include "Socket.h"
#include <functional>
#include "EventLoop.h"
#include <unistd.h>
#include <arpa/inet.h>

Acceptor::Acceptor(EventLoop* loop, const struct sockaddr_in& addr):
    loop_(loop),
    socket_(createNonblockingSocket()),
    channel_(loop, socket_),
    newConnectionCallback_(),
    listening_(false)
{
    socket_.setReuseAddr(true);
    socket_.bind(addr);
    channel_.setReadCallback(std::bind(&Acceptor::handleRead, this));
}

void Acceptor::listen()
{
    loop_->assertInLoopThread();
    listening_ = true;
    socket_.listen();
    channel_.enableReading();
}


void Acceptor::handleRead()
{
    loop_->assertInLoopThread();
    struct sockaddr_in addr;
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