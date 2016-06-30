cmake_minimum_required ( VERSION 2.6 )

include_directories ( ${CMAKE_SOURCE_DIR} )
link_directories ( ${CMAKE_SOURCE_DIR}/../abc )

project ( IF_Searcher )

add_executable ( IF_Searcher main.cpp )
target_link_libraries ( IF_Searcher ifs )
target_link_libraries ( IF_Searcher abc -lm -ldl -rdynamic -lreadline -ltermcap -lpthread )

