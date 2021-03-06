#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "../TcpServer.h"

class HttpRequest;
class HttpResponse;

class HttpServer
{
public:
    typedef std::function<void(const HttpRequest&, HttpResponse*)> HttpCallback;
    typedef std::function<void(const TcpConnectionPtr)> MessageCompleteCallback;
    
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
    
    void setConnectionCallback(const ConnectionCallback& cb)
    { connectionCallback_ = cb; }
    
    void setMessageCompleteCallback(const MessageCompleteCallback& cb)
    { messageCompleteCallback_ = cb; }

    void start();

private:
    void onConnection(const TcpConnectionPtr& conn);  // TcpServer的connectionCallback
    void onMessage(const TcpConnectionPtr& conn,
                   Buffer* buf,
                   TimeStamp receiveTime);  // 绑定为TcpServer的MessageCallback
    void onResquest(const TcpConnectionPtr&, const HttpRequest&);  

    TcpServer server_;
    HttpCallback httpCallback_;  // 在onRequest中调用
    ConnectionCallback connectionCallback_; // 在onConnection调用
    MessageCompleteCallback messageCompleteCallback_; // onMessage处理消息后调用 
};

#endif
