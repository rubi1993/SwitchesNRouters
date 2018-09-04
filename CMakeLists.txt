cmake_minimum_required(VERSION 3.8)
project(SRA)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp data_structures.cpp rules.cpp)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread")
add_executable(SRA ${SOURCE_FILES})