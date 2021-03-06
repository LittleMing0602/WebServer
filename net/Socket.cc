#include <errno.h>
#include <errno.h>
#include "Socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/tcp.h>
#include "../log/Logging.h"

int createNonblockingSocket()
{
    int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK |SOCK_CLOEXEC, IPPROTO_TCP);
    if(sockfd < 0)
    {
        printf("error : createNonblockingSocket()\n");
        exit(1);
    }
}

// Socket负责管理，一定要在析构函数中关闭所管理socket描述符
Socket::~Socket()
{
    close(sockfd_);
}

void Socket::bind(const InetAddress& addr)
{
    const struct sockaddr_in& addr_in = addr.getAddrInet();
    int ret = ::bind(sockfd_, static_cast<const struct sockaddr*>((const void*)&addr_in), sizeof addr_in);
    if(ret < 0)
    {
        printf("bind error\n");
        exit(1);
    }
}

void Socket::listen()
{
    int ret = ::listen(sockfd_, SOMAXCONN);
    if(ret < 0)
    {
        printf("listen error\n");
        exit(1);
    }
}

int Socket::accept(InetAddress* addr)
{
    struct sockaddr_in addr_in;
    bzero(&addr_in, sizeof addr_in);
    socklen_t addrlen = sizeof addr_in;
    int connfd = accept4(sockfd_, static_cast<struct sockaddr*>((void*)&addr_in), &addrlen,
                         SOCK_NONBLOCK | SOCK_CLOEXEC);

    if(connfd < 0)
    {
        int savedErrno = errno;
        switch(savedErrno)
        {
        case EAGAIN:
        case ECONNABORTED:
        case EINTR:
        case EPROTO:
        case EPERM:
        case EMFILE:
            errno = savedErrno;
            break;

        case EBADF:
        case EFAULT:
        case EINVAL:
        case ENFILE:
        case ENOBUFS:
        case ENOMEM:
        case ENOTSOCK:
        case EOPNOTSUPP:
            LOG_FATAL << "unexpected error of ::accept " << savedErrno;
            // printf("fatal : unexpected error of ::accept\n");
            // exit(1);
            break;
        default:
            LOG_FATAL << "unknown error of ::accept " << savedErrno;
            // printf("fatal : unknown error of ::accept\n");
            break;

        }
    }
    addr->setAddrInet(addr_in);
    return connfd;
}

void Socket::shutdownWrite()
{
    if(::shutdown(sockfd_, SHUT_WR) < 0)
    {
        LOG_SYSERR << "Sock::shutdownWrite error";
        // printf("error: Sock::shutdownWrite");
        // exit(1);
    }
}

void Socket::setTcpNoDelay(bool on)
{
    int opt = on ? 1 : 0;
    ::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof opt);
}

void Socket::setReuseAddr(bool on)
{
    int opt = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt);
}

void Socket::setKeepAlive(bool on)
{
    int opt = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof opt);
}
