#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <string>
#include <map>

class Buffer;

class HttpResponse
{
public:
    enum HttpStatusCode
    {
        kUnKnown,
        k200Ok = 200, // 成功
        k301MovePermanently = 301,  // 301重定向，请求的页面永久性移植到另一个地址
        k400BadRequest = 400,  // 错误的请求，语法格式有错，服务器无法处理此请求
        k404NotFound = 404, // 网页不存在
    };
    explicit HttpResponse(bool toClose):
        statusCode_(kUnKnown),
        closeConnection_(toClose)
    {}

    void setStatusCode(HttpStatusCode code)
    { statusCode_ = code; }

    void setStatusMessage(const std::string& message)
    { statusMessage_ = message; }

    void setCloseConnection(bool on)
    { closeConnection_ = on; };

    bool closeConnection() const
    { return closeConnection_; }

    void setContentType(const std::string& contentType)
    { addHeader("Content-Type", contentType); }

    void addHeader(const std::string& key, const std::string& value)
    { headers_[key] = value; }

    void setBody(const std::string& body)
    { body_ = body; }

    void appendToBuffer(Buffer* output) const;

    ~HttpResponse() {}

private:
    std::map<std::string, std::string> headers_;  // 头部
    HttpStatusCode statusCode_;  // 状态码
 
    std::string statusMessage_;  // 状态说明
    bool closeConnection_;  // 是否关闭连接
    std::string body_;  // 响应报文体
};

#endif
