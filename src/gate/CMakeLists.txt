cmake_minimum_required ( VERSION 2.6 )

project ( gate )

set ( include_path ../../include/gate )
include_directories ( ${include_path} )
set ( HEADER_FILES ${include_path}/gate.h )

add_library ( gate gate.cpp ${HEADER_FILES} )
