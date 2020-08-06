#include "HttpServer.h"
#include "HttpRequest.h"
#include "HttpContext.h"
#include "HttpResponse.h"
#include <functional>
#include <boost/any.hpp>


// 处理请求行  Get / HTTP/1.1
bool processRequestLine(const char* begin, const char* end, HttpContext* constext)
{
    bool succeed = false;
    const char* start = begin;
    const char* space = std::find(start, end, ' ');
    HttpRequest& request = constext->request();
    
    if(space != end && request.setMethod(start, space))
    {
       start = space + 1;
       space = std::find(start, end, ' ');
       if(space != end)
       {
           request.setPath(start, space);  // 设置路径
           start = space + 1;
           succeed = end - start == 8 && std::equal(start, end - 1, "HTTP/1.");
           // 设置版本号
           if(succeed)
           {
               if(*(end - 1) == '1')
               {
                   request.setVersion(HttpRequest::kHttp11);
               }
               else if(*(end -1) == '0')
               {
                   request.setVersion(HttpRequest::kHttp10);
               }
               else
               {
                   succeed = false;
               }
           }
       }
    }
    return succeed;
}

// 解析请求报文
bool parseRequest(Buffer* buf, HttpContext* context, TimeStamp receiveTime)
{
    bool ok = true;
    bool hasMore = true;

    while(hasMore)
    {
        if(context->expectRequestLine())
        {
            const char* crlf = buf->findCRLF();
            if(crlf)
            {
                ok = processRequestLine(buf->peek(), crlf, context);  // 解析请求行
                if(ok)
                {
                    context->request().setReceiveTime(receiveTime);
                    buf->retrieveUntil(crlf + 2);
                    context->receiveRequestLine();
                }
                else
                {
                    hasMore = false;
                }
            }
            else
            {
                hasMore = false;
            }
        }
        else if(context->expectHeaders())
        {
            const char* crlf = buf->findCRLF();
            if(crlf)
            {
                const char* colon = std::find(buf->peek(), crlf, ':');
                if(colon != crlf)
                {
                    context->request().addHeader(buf->peek(), colon, crlf);
                }
                else
                {
                    context->receiveHeaders();
                    hasMore = !context->gotAll();
                }
                buf->retrieveUntil(crlf + 2);
            }
            else
            {
                hasMore = false;
            }
        }
        else if(context->expectBody())
        {

        }
        
    }
    return ok;
}

HttpServer::HttpServer(EventLoop* loop, const InetAddress& listenAddr):
    server_(loop, listenAddr),
    httpCallback_()
{
    server_.setMessageCallback(std::bind(&HttpServer::onMessage, this, 
                                         std::placeholders::_1, 
                                         std::placeholders::_2,
                                         std::placeholders::_3));

    server_.setConnectionCallback(std::bind(&HttpServer::onConnection, this, 
                                            std::placeholders::_1));
        

}

HttpServer::~HttpServer()
{

}

void HttpServer::start()
{
    printf("HttpServer starts listenning\n");
    server_.start();
}

void HttpServer::onConnection(const TcpConnectionPtr& conn)
{
    if(conn->connected())
    {
        conn->setContext(HttpContext());  // 建立连接，设置连接上下文，包含解析请求处于的状态
    }
}

void HttpServer::onMessage(const TcpConnectionPtr& conn,
                           Buffer* buf, 
                           TimeStamp receiveTime)
{
    HttpContext* context = boost::any_cast<HttpContext>(conn->getMutableContext());

    if(!parseRequest(buf, context, receiveTime))
    {
        conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
        conn->shutdown();
    }

    if(context->gotAll())
    {
        onResquest(conn, context->request());
        context->reset(); // 本次请求处理完毕，重置HttpContext，适用于长连接
    }
}

void HttpServer::onResquest(const TcpConnectionPtr& conn, const HttpRequest& req)
{
    const std::string& connection = req.getHeader("Connection");
    bool isClose = connection == "close" || (req.getVersion() == HttpRequest::kHttp10 &&
                                             connection != "Keep-Alive");  // 判断是否需要断开连接

    HttpResponse response(isClose);
    httpCallback_(req, &response);  // 回调httpCallback_
    Buffer buf;
    response.appendToBuffer(&buf);  // 将相应报文添加到buf中
    conn->send(&buf);
    if(response.closeConnection())
    {
        conn->shutdown();  // 断开连接
    }

}
      
