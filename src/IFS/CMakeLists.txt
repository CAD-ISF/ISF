cmake_minimum_required ( VERSION 2.6 )

include_directories ( ${CMAKE_SOURCE_DIR} )

project ( ifs )

add_library ( ifs ifs.cpp )
target_link_libraries ( ifs circuit )


