#include <iostream>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../net/Acceptor.h"
#include "../net/EventLoop.h"

void newConnectionCallback(int connfd, const sockaddr_in& addr)
{
    char host[INET_ADDRSTRLEN] = {0};
    char buf[1024] = {0};
    inet_ntop(AF_INET, &addr.sin_addr, host, 256);
    uint16_t port = htobe16(addr.sin_port);
    snprintf(buf, 1024, "%s:%u", host, port);
    printf("newconnection(): accept a new connection from %s\n", 
           buf);
    ::write(connfd, "how are you?\n", 13);
    close(connfd);
}

int main()
{
    printf("main(): pid = %d\n", getpid());
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9981);
    inet_aton("127.0.0.1", &(addr.sin_addr));
    EventLoop loop;

    Acceptor acceptor(&loop, addr);
    acceptor.setNewConnectionCallback(newConnectionCallback);
    acceptor.listen();
    loop.loop();

    return 0;
}

