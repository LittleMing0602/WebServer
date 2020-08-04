#include <unistd.h>
#include <stdio.h>
#include "../net/InetAddress.h"
#include "../net/EventLoop.h"
#include "../net/TcpServer.h"
#include "../net/TcpConnection.h"

void onConnection(const TcpConnectionPtr& conn)
{
    if(conn->connected())
    {
        printf("onConnection() : new connection [%s] from %s\n", 
               conn->name().c_str(), 
               conn->peerAddr().toIp().c_str());
    }
    else
    {
        printf("onConnection(): connection [%s] is down\n",
               conn->name().c_str());
    }
}

void onMessage(const TcpConnectionPtr& conn, Buffer* buf, TimeStamp receiveTime)
{
    printf("onMessage(): receive %zd bytes from connection [%s] at %s\n",
           buf->readableBytes(),
           conn->name().c_str(),
           receiveTime.toString().c_str());
    conn->send(buf->retrieveAllAsString());
}

int main()
{
    printf("main(): pid = %d\n", getpid());
    InetAddress listenAddr(9981);
    EventLoop loop;

    TcpServer server(&loop, listenAddr);
    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);
    server.start();

    loop.loop();
}
