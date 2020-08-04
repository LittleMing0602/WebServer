#include <stdio.h>
#include <unistd.h>
#include "../net/EventLoop.h"
#include "../net/TcpConnection.h"
#include "../net/TcpServer.h"
#include "../net/Buffer.h"
#include "../timer/TimeStamp.h"

void onConnection(const TcpConnectionPtr& conn)
{
    if(conn->connected())
    {
        printf("onConnection(): new connection %s form %s\n", 
               conn->name().c_str(), conn->peerAddr().toIp().c_str());
    }
    else
    {
        printf("onConnection() : connection %s is down\n",
               conn->name().c_str());
    }
}


void onMessage(const TcpConnectionPtr& conn, Buffer* buf, TimeStamp receiveTime)
{
    printf("onMessage(): received %zd bytes from connection %s at %s\n", 
            buf->readableBytes(), conn->name().c_str(), receiveTime.toString().c_str());
    printf("onMessage(): %s\n", buf->retrieveAllAsString().c_str());
    
}

int main()
{   
    printf("main() : pid = %d\n", getpid());
    InetAddress listenAddr(9981);
    EventLoop loop;
    TcpServer server(&loop, listenAddr);
    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);
    server.start();
    loop.loop();
    return 0;
}

