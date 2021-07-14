cmake_minimum_required(VERSION 2.8)
project(GlobalSettings)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_library(GlobalSettings STATIC src/globalsettings.cpp)

set_target_properties(GlobalSettings PROPERTIES PUBLIC_HEADER "src/globalsettings.h")

install(TARGETS GlobalSettings)
