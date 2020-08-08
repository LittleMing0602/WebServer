aux_source_directory(. mysql_srcs)

add_library(mysql mysql_srcs)
target_link_libraries(mysql log)
