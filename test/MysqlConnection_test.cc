#include "../mysql/MysqlConnection.h"

int main()
{

    MysqlConnection mysqlConn("192.168.31.80", 3306, "offer", "root", "ming");
    mysqlConn.queryAndStore("select * from student");
    mysqlConn.fetchRows();
    return 0;
}

