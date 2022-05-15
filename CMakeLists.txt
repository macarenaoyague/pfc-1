cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)

project(pfc1)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Graphs DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_executable(pfc1 main.cpp Algorithm.hpp Dantzig.hpp Testing.hpp Graphs/graph.txt)
target_include_directories(pfc1 PRIVATE Classes)