#ifndef MYSQL_CONNECTION_H
#define MYSQL_CONNECTION_H

#include <mysql/mysql.h>
#include <string>
#include "../log/Logging.h"

class MysqlConnection
{
public:
    MysqlConnection(const char* ip, unsigned short port, const char* dbName,
                    const char* userName, const char* password);
    ~MysqlConnection();
    
    int query(const char* q);
    
    int fetchRows();

private:
    std::string ip_;
    unsigned short port_;
    std::string dbName_;
    std::string userName_;
    std::string password_;
    MYSQL mysql_;
    MYSQL_RES* result_;

};

#endif

