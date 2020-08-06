# set(thread Thread.cc)
aux_source_directory(. src)
add_library(Thread ${src})
target_link_libraries(Thread pthread)
