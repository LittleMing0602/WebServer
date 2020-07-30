#ifndef  SOCKET_H
#define SOCKET_H

#include "InetAdress.h"

int createNonblockingSocket();

class Socket
{
public:
    Socket(int fd):sockfd_(fd) {}
    ~Socket() {}

    int fd() const { return sockfd_; }
    void bind(const InetAdress& addr);
    void listen();
    void shutdownWrite();
    void setTcpNoDelay(bool on); // 禁用Nagle算法
    void setReuseAddr(bool on);
    // TCP keepalive 是指定期探测连接是否存在，如果应用层有心跳的话，这个选项是不需设置的
    void setKeepAlive(bool on);
    int accept(InetAdress* addr);

private:
    const int sockfd_;
};


#endif
