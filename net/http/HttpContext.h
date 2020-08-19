#ifndef HTTPCONTEXT_H
#define HTTPCONTEXT_H

#include "HttpRequest.h"
#include <boost/any.hpp>


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
    
    void setContext(const boost:: any& context)
    { context_ = context; }
    
    const boost::any* getContext() const
    { return &context_; }

    boost::any* getMutableContext()
    { return &context_; }

private:
    HttpRequestParseState state_;
    HttpRequest request_;
    boost::any context_;
};

#endif
