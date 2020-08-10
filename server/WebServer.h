#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include "../net/http/HttpServer.h"
#include "../net/EventLoop.h"
#include <functional>

class WebServer
{
public:
    WebServer(const InetAddress& listenAddr):
        loop_(),
        server_(&loop_, listenAddr)
    {
        server_.setHttpCallback(std::bind(&WebServer::onRequest, this, 
                                          std::placeholders::_1,
                                          std::placeholders::_2));
    }

    void setThreadNum(int numThreads)
    { server_.setThreadNum(numThreads); }

    void start();

    ~WebServer() {}

    void onRequest(const HttpRequest& req, HttpResponse* resp);

private:
    EventLoop loop_;
    HttpServer server_;
};

#endif
