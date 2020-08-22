#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include "../net/http/HttpServer.h"
#include "../net/EventLoop.h"
#include "CircularBuffer.h"
#include <functional>
#include <unordered_set>

class WebServer
{
public:
    WebServer(const InetAddress& listenAddr, int idleSeconds);

    void setThreadNum(int numThreads)
    { server_.setThreadNum(numThreads); }

    void start();

    ~WebServer() {}

    void onRequest(const HttpRequest& req, HttpResponse* resp);
    
    void readFile(const char* path, HttpResponse* resp);
    
    // void onWriteComplete(const TcpConnectionPtr& conn);
     
    void onConnection(const TcpConnectionPtr& conn);
    
    void onMessageComplete(const TcpConnectionPtr& conn);
    
    typedef std::weak_ptr<TcpConnection> WeakTcpConnectionPtr;
    struct Entry
    {
        explicit Entry(const WeakTcpConnectionPtr& weakConn):
            weakConn(weakConn_)
            { }
        ~Entry()
        {
            TcpConnectionPtr conn = weakConn_.lock();
            if(conn)
            {
                conn->shutdown();
            }
        }
        
        WeakTcpConnectionPtr weakConn_;
    };
    
    typedef std::shared_ptr<Entry> EntryPtr;
    typedef std::weak_ptr<Entry> WeakEntryPtr;
    typedef std::unordered_set<EntryPtr> Bucket;
    typedef CircularBuffer<Bucket> WeakConnectionList;
    
private:
    EventLoop loop_;
    HttpServer server_;
    static const int kBufSize_ = 64 * 1024;
    
    WeakConnectionList connectionBuckets_;
    
};

#endif
