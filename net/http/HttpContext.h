#ifndef HTTPCONTEXT_H
#define HTTPCONTEXT_H

#include "HttpRequest.h"

class HttpContext
{
public:
    enum HttpRequestParseState
    {
        kExpectRequestLine,
        kExpectHeaders,
        kExpectBody,
        kGotAll,
    };

    HttpContext():
        state_(kExpectRequestLine)
    {}

    ~HttpContext() {}

    bool expectRequestLine() const
    { return state_ == kExpectRequestLine; }

    bool expectHeaders() const
    { return state_ == kExpectHeaders; }

    bool expectBody() const
    { return state_ == kExpectBody; }

    bool gotAll() const
    { return state_ == kGotAll; }

    void receiveRequestLine()
    { state_ = kExpectHeaders; }

    void receiveHeaders()
    { state_ = kExpectBody; }
    
    void receiveBody()
    { state_ = kGotAll; }
    
    void receiveAll()
    {
        state_ = kGotAll;
    }

    void reset()
    {
        state_ = kExpectRequestLine;
        HttpRequest dummy;
        request_.swap(dummy);
    }

    const HttpRequest& request() const
    { return request_; }

    HttpRequest& request()
    { return request_; }

private:
    HttpRequestParseState state_;
    HttpRequest request_;
};

#endif
