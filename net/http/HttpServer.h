#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "../TcpServer.h"

class HttpRequest;
class HttpResponse;

class HttpServer
{
public:
    typedef std::function<void(const HttpRequest&, HttpResponse*)> HttpCallback;
    
    HttpServer(EventLoop* loop, const InetAddress& listenAddr);
    ~HttpServer();

    void setHttpCallback(const HttpCallback& cb)
    {
        httpCallback_ = cb;
    }

    void setThreadNum(int numThreads)
    {
        server_.setThreadNum(numThreads);
    }

    void start();

private:
    void onConnection(const TcpConnectionPtr& conn);
    void onMessage(const TcpConnectionPtr& conn,
                   Buffer* buf,
                   TimeStamp receiveTime);
    void onResquest(const TcpConnectionPtr&, const HttpRequest&);

    
    TcpServer server_;
    HttpCallback httpCallback_;
};

#endif
