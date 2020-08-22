#include "WebServer.h"
#include "../net/http/HttpRequest.h"
#include "../net/http/HttpResponse.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "../log/Logging.h"
#include "../net/http/HttpContext.h"
#include "WebServerContext.h"

WebServer::WebServer(const InetAddress& listenAddr, int idleSeconds):
        loop_(),
        server_(&loop_, listenAddr),
        connectionBuckets_(idleSeconds)
    {
        server_.setHttpCallback(std::bind(&WebServer::onRequest, this, 
                                          std::placeholders::_1,
                                          std::placeholders::_2));
        
        server_.setConnectionCallback(std::bind(&WebServer::onConnection, this, 
                                                 std::placeholders::_1));
                                                 
        server_.setMessageCompleteCallback(std::bind(&WebServer::onMessageComplete, this, 
                                                 std::placeholders::_1));
    }

void WebServer::start()
{
    LOG_TRACE << "WebServer starts";
    server_.start();
    loop_.loop();
}

void WebServer::onRequest(const HttpRequest& req, HttpResponse* resp)
{
    
    if(req.path() == "/")
    {
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        resp->setContentType("text/html");
        resp->addHeader("Server", "WebServer");
        readFile("../root/welcome.html", resp);
    }
    else if(req.path() == "/5")
    {
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        resp->setContentType("image/jpg");
        resp->addHeader("Server", "WebServer");
        readFile("../root/xxx.jpg", resp);
    }
    else if(req.path() == "/6")
    {
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        resp->setContentType("video/mpeg4");
        resp->addHeader("Server", "WebServer");
        readFile("../root/xxx.mp4", resp);
    }
    else
    {
        resp->setStatusCode(HttpResponse::k404NotFound);
        resp->setStatusMessage("Not Found");
        resp->setCloseConnection(true);
    }
}

void WebServer::readFile(const char* path, HttpResponse* resp)
{
    int fd = open(path, O_RDONLY);
    if(fd == -1)
    {
        char buf[128] = {0};
        LOG_TRACE << getcwd(buf, sizeof buf);
        LOG_SYSFATAL << "open error";
    }
    struct stat statbuff;
    int ret = stat(path, &statbuff);
    if(ret == -1)
    {
        LOG_SYSFATAL << "stat error";
    }
    
    char* p = (char *)mmap(0, statbuff.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    resp->setBody(p, statbuff.st_size);
    munmap(p, statbuff.st_size);
    close(fd);
}

// 将conn添加到timeing wheel中区
void WebServer::onConnection(const TcpConnectionPtr& conn)
{
    if(conn->connected()) 
    {
        EntryPtr entry(std::make_shared<Entry>(conn));
        connectionBuckets_.back().insert(entry);
        WeakEntryPtr weakEntry(entry);
        HttpContext* context = boost::any_cast<HttpContext>(conn->getMutableContext());
        context->setContext(weakEntry);
    }
    else
    { 
    }

}

void WebServer::onMessageComplete(const TcpConnectionPtr& conn)
{
    HttpContext* context = boost::any_cast<HttpContext>(conn->getMutableContext());
    WeakEntryPtr weakEntry(*boost::any_cast<WeakEntryPtr>(context->getMutableContext()));
    EntryPtr entry = weakEntry.lock();
    if(entry)
    {
        connectionBuckets_.back().insert(entry);
    }
}
