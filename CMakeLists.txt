cmake_minimum_required(VERSION 3.14)
project(GrammerAnalysis)

set(CMAKE_CXX_STANDARD 14)

add_executable(GrammerAnalysis main.cpp grammer.cpp grammer.h parse.h parse.cpp)