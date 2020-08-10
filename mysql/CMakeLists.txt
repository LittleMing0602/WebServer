aux_source_directory(. mysql_srcs)

find_library(mysqlclient libmysqlclient.so /usr/lib64/mysql)
add_library(mysql ${mysql_srcs})
target_link_libraries(mysql log ${mysqlclient})
# target_link_libraries(mysql log)
