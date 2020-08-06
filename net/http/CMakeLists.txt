set(http_srcs HttpServer.cc HttpResponse.cc)

add_library(http ${http_srcs})
target_link_libraries(http net)


