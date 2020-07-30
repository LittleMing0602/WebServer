#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <memory>
#include "Acceptor.h"

class TcpServer
{
public:
    TcpServer() {}
    ~TcpServer() {}

private:
    void newConection(int sockfd, )
    std::unique_ptr<Acceptor> acceptor_;

}

#endif

;

