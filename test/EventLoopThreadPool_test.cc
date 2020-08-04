#include "../net/TcpServer.h"
#include "../net/EventLoop.h"
#include "../net/InetAddress.h"

#include <stdio.h>

void onConnection(const TcpConnectionPtr& conn)
{
  if (conn->connected())
  {
    printf("onConnection(): tid=%d new connection [%s] from %s\n",
           tid(),
           conn->name().c_str(),
           conn->peerAddr().toIpPort().c_str());
  }
  else
  {
    printf("onConnection(): tid=%d connection [%s] is down\n",
           tid(),
           conn->name().c_str());
  }
}

void onMessage(const TcpConnectionPtr& conn,
               Buffer* buf,
               TimeStamp receiveTime)
{
  printf("onMessage(): tid=%d received %zd bytes from connection [%s] at %s\n",
         tid(),
         buf->readableBytes(),
         conn->name().c_str(),
         receiveTime.toString().c_str());

  printf("onMessage(): [%s]\n", buf->retrieveAllAsString().c_str());
}

int main(int argc, char* argv[])
{
  printf("main(): pid = %d\n", getpid());

  InetAddress listenAddr(9981);
  EventLoop loop;

  TcpServer server(&loop, listenAddr);
  server.setConnectionCallback(onConnection);
  server.setMessageCallback(onMessage);
  if (argc > 1) {
    server.setThreadNum(atoi(argv[1]));
  }
  server.start();

  loop.loop();
}
