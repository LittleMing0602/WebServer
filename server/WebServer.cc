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

void WebServer::start()
{
    LOG_TRACE << "WebServer starts";
    server_.start();
    loop_.loop();
}

void WebServer::onRequest(const HttpRequest& req, HttpResponse* resp, const TcpConnectionPtr& conn)
{
    HttpContext* httpContext = boost::any_cast<HttpContext>(conn->getMutableContext());
    WebServerContext* context = boost::any_cast<WebServerContext>(httpContext->getMutableContext()); 
    if(req.path() == "/")
    {
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        resp->setContentType("text/html");
        resp->addHeader("Server", "WebServer");
        int fd = open("../root/welcome.html", O_RDONLY);
        if(fd == -1)
        {
            char buf[128] = {0};
            LOG_TRACE << getcwd(buf, sizeof buf);
            LOG_SYSFATAL << "open error";
        }
        struct stat statbuff;
        int ret = stat("../root/welcome.html", &statbuff);
        if(ret == -1)
        {
            LOG_SYSFATAL << "stat error";
        }
        
        char* p = (char *)mmap(0, statbuff.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
        resp->setBody(p, statbuff.st_size);
        munmap(p, statbuff.st_size);
        close(fd);
    }
    else if(req.path() == "/5")
    {
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        resp->setContentType("image/*");
        resp->addHeader("Server", "WebServer");

        assert(context->fp() == NULL);
        FILE* fp = fopen("../root/xxx.jpg", "rb");
        if(fp)
        {
            context->setFp(fp);
            char buf[kBufSize_];
            size_t nread = fread(buf, 1, sizeof buf, fp);
            resp->setBody(buf, nread);
        }
        else
        {
            LOG_SYSERR << "WebServer::onRequest()";
        }
    }
    else
    {
        resp->setStatusCode(HttpResponse::k404NotFound);
        resp->setStatusMessage("Not Found");
        resp->setCloseConnection(true);
    }
}


void WebServer::onWriteComplete(const TcpConnectionPtr& conn)
{
    HttpContext* httpContext = boost::any_cast<HttpContext>(conn->getMutableContext());
    WebServerContext* context = boost::any_cast<WebServerContext>(httpContext->getMutableContext()); 
    FILE* fp = context->fp();
    char buf[kBufSize_];
    size_t nread = fread(buf, 1, sizeof buf, fp);
    if(nread > 0)
    {
        resp->setBody(buf, nread);
    }
    else
    {
        fclose(fp_);
        fp_ = NULL;
        LOG_INFO << "file send over";
    }
}


void WebServer::onConnection(const TcpConnectionPtr& conn)
{
    assert(conn->connected());
    HttpContext* context = boost::any_cast<HttpContext>(conn->getMutableContext());
    context->setContext(WebServerContext());
}

