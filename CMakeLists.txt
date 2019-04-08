cmake_minimum_required(VERSION 3.13)
project(serial__)

set(CMAKE_CXX_STANDARD 17)
set(Boost_INCLUDE_DIR /usr/local/include/)
find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(serial__ main.cpp PGsql.cpp header.h config.cpp)
    target_link_libraries(serial__ pq pthread ${Boost_LIBRARIES})
endif()

#target_link_libraries(serial__ pq)