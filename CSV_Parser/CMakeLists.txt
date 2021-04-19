cmake_minimum_required(VERSION 3.17)

project(CSV_Parser)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS -pthread)

add_library(CSV_Parser STATIC csv.h csv.cpp)