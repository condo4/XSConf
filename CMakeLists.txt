cmake_minimum_required(VERSION 2.8)
project(XSConf)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_library(XSConf STATIC cxx/xsconf.cpp)

set_target_properties(XSConf PROPERTIES PUBLIC_HEADER "cxx/xsconf.h")

install(TARGETS XSConf)
