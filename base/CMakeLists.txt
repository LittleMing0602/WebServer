# set(thread Thread.cc)
aux_source_directory(. src)
add_library(thread ${src})
target_link_libraries(thread pthread)
