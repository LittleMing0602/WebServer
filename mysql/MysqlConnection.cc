#include "MysqlConnection.h"

MysqlConnection::MysqlConnection(const char* ip, unsigned short port, const char* dbName, 
                                 const char* userName, const char* password):
    ip_(ip),
    port_(port),
    dbName_(dbName),
    userName_(userName),
    password_(password),
    result_(NULL)
{
    if(NULL == mysql_init(&mysql_))
    {
        LOG_FATAL << "mysql init failed";
    } 

    //连接
    if(mysql_real_connect(&mysql_, ip_.c_str(), userName_.c_str(), password_.c_str(), 
                          dbName_.c_str(), port_, NULL, 0))
    {
        LOG_FATAL << "mysql connected failed";
    }
}

MysqlConnection::~MysqlConnection()
{
    if(result_ != NULL)
    {
        mysql_free_result(result_);
    }
    result_ = NULL;
    mysql_close(&mysql_);
}

// 执行sql语句，成功返回0， 失败返回-1
int MysqlConnection::query(const char* q)
    {
        if(mysql_query(&mysql_, q) != 0)
        {
            LOG_ERROR << "mysql_errno: " << mysql_errno(&mysql_) 
                << ", " << mysql_error(&mysql_);
            return -1;
        }
        
        if(result_ != NULL)
        {
            mysql_free_result(result_);
        }

        if(result_ = mysql_store_result(&mysql_))
        {
            LOG_DEBUG << "query successfully and there are rows";
        }
        else // 如果返回集是空的，可能是类似insert的语句或者是出错了
        {
            if(mysql_field_count(&mysql_) == 0)
            {
                // query does not return data (it was not a SELECT)
                // num_rows = mysql_affected_rows(&msyql);
                LOG_DEBUG << "mysql_query successfully";
            }
            else // mysql_store_result should have returned data
            {
                LOG_ERROR << "mysql_errno: " << mysql_errno(&mysql_) 
                    << ", " << mysql_error(&mysql_);
                return -1;
            }

        }
        return 0;
        
    }
