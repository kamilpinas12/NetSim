cmake_minimum_required(VERSION 3.24)
project(lab_netsim)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab_netsim main.cpp include/storage_types.hpp src/storage_types.cpp)
