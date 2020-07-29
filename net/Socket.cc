#include <errno.h>
#include <errno.h>
#include "Socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

void Socket::bind(const struct sockaddr_in &addr)
{
    int ret = ::bind(sockfd_, static_cast<const struct sockaddr*>((const void*)&addr), sizeof addr);
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

int Socket::accept(struct sockaddr_in* addr)
{
    socklen_t addrlen = sizeof *addr;
    int connfd = accept4(sockfd_, static_cast<struct sockaddr*>((void*)addr), &addrlen,
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
            errno = savenErrno;
            break;

        case EBADF:
        case EFAULT:
        case EINVAL:
        case ENFILE:
        case ENOBUFS:
        case ENOMEM:
        case ENOTSOCK:
        case EOPNOTSUPP:
            printf("fatal : unexpected error of ::accept\n");
            exit(1);
            break;
        default:
            printf("fatal : unknown error of ::accept\n");
            break;

        }
    }
    return connfd;
}

void Socket::shutdownWrite()
{
    if(::shutdown(sockfd_, SHUT_WR) < 0)
    {
        printf("error: Sock::shutdownWrite");
        exit(1);
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
