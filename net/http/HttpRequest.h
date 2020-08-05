#ifndef HTTPRESQUEST_H
#define HTTPRESQUEST_H

#include "../../timer/TimeStamp.h"
#include <map>
#include <string>
#include <assert.h>

class HttpRequest
{
public:
    enum Method
    {
        kInvalid, kGet, kPost, kHead, kPut, kDelete
    };

    enum Version
    {
        kUnknown, kHttp10, kHttp11
    };


    HttpRequest():
        method_(kInvalid),
        version_(kUnknown)
    {}

    ~HttpRequest() {}

    void setVersion(Version v)
    { version_ = v; }

    Version getVersion() const 
    { return version_; }

    bool setMethod(const char* start, const char* end)
    {
        assert(method_ == kInvalid);
        std::string m(start, end);
        if(m == "GET")
        {
            method_ = kGet;
        }
        else if (m == "POST")
        {
            method_ = kPost;
        }
        else if (m == "HEAD")
        {
            method_ = kHead;
        }
        else if (m  == "PUT")
        {   
            method_ = kPut;
        }
        else if (m == "DELETE")
        {
            method_ = kDelete;
        }
        else
        {
            method_ = kInvalid;
        }
        return method_ != kInvalid;
    }

    Method method() const
    { return method_; }

    const char* methodString() const
    {
        const char* result = "UNKNOWN";
        switch(method_)
        {
            case kGet:
                result = "GET";
                break;
            case kPost:
                result = "POST";
                break;
            case kHead:
                result = "HEAD";
                break;
            case kPut:
                result = "PUT";
                break;
            case kDelete:
                result = "DELETE";
                break;
            default:
                break;
        }

        return result;
    }

    void setPath(const char* start, const char* end)
    {
        path_.assign(start, end);
    }

    const std::string& path() const
    { return path_; }

    void setReceiveTime(TimeStamp t)
    { receiveTime_ = t; }

    TimeStamp receiveTime() const
    { return receiveTime_; }

    void addHeader(const char* start, const char* colon, const char* end)
    {
        std::string field(start, colon); //header 域
        ++colon; 

        //去除左空格
        while(colon < end && isspace(*colon))
        {
            ++colon;
        }

        std::string value(colon, end); // header值

        // 去除右空格
        while(!value.empty() && isspace(value[value.size() - 1]))
        {
            value.resize(value.size() - 1);
        }

        headers_[field] = value;
    }

    std::string getHeader(const std::string& field) const
    {
        std::string result;
        std::map<std::string, std::string>::const_iterator it = headers_.find(field);
        if(it != headers_.end())
        {
            result = it->second;
        }

        return result;
    }

    const std::map<std::string, std::string>& headers() const
    {
        return headers_;
    }

    void swap(HttpRequest& that)
    {
        std::swap(method_, that.method_);
        path_.swap(that.path_);
        receiveTime_.swap(that.receiveTime_);
        headers_.swap(that.headers_);
    }

private:
    Method method_; //请求方法
    Version version_;   //协议版本号1.0/1.1
    std::string path_;  // 请求路径
    TimeStamp receiveTime_; // 请求时间
    std::map<std::string, std::string> headers_; // header列表
};

#endif
