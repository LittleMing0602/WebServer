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