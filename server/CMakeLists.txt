aux_source_directory(. webserver_srcs)

add_library(webserver ${webserver_srcs})
target_link_libraries(webserver http net)
