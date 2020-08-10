#include "WebServer.h"
#include "../net/http/HttpRequest.h"
#include "../net/http/HttpResponse.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "../log/Logging.h"

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
        int fd = open("./root/welcome.html", O_RDONLY);
        struct stat statbuff;
        unsigned long length = stat("./root/welcome.html", &statbuff);
        char* p = (char *)mmap(0, statbuff.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
        resp->setBody(p);
    }
}
