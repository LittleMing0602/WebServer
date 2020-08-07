include_directories(../timer)
aux_source_directory(. log_srcs)
# set(log_srcs Logging.cc LogStream.cc)

add_library(log ${log_srcs})
target_link_libraries(log timer)
