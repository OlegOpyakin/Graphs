cmake_minimum_required(VERSION 3.6)
project(Graphs)

set(CMAKE_CXX_FLAGS "-std=c++20 -g -O2")
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++20 -Wall -g -O0 -fsanitize=address -fsanitize=undefined")

add_executable(Graph src/main.cc)

target_include_directories(Graph PUBLIC inc/)