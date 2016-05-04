cmake_minimum_required ( VERSION 2.6 )

include_directories ( ${CMAKE_SOURCE_DIR} )

project ( IFS )

add_executable ( IFS ifs.cpp )
target_link_libraries ( IFS gate )

