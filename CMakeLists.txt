cmake_minimum_required(VERSION 3.13)
project(serial__)

set(CMAKE_CXX_STANDARD 17)

add_executable(serial__ main.cpp PGsql.cpp header.h)
target_link_libraries(serial__ pq pthread)

#target_link_libraries(serial__ pq)