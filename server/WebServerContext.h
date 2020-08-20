#ifndef WEB_SERVER_CONTEXT_H
#define WEB_SERVER_CONTEXT_H

#include <stdio.h>

class WebServerContext
{
public:
    WebServerContext():
    fp_(NULL)
    {}
    ~WebServerContext() 
    {
        if(fp_)
        {
            fclose(fp_);
            fp_ = NULL;
        }    
    }
    
    FILE* fp() const
    { return fp_; }
    
    void setFp(FILE* fp)
    { fp_ = fp; }

private:
    FILE* fp_;
};

#endif
