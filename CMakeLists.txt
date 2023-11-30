cmake_minimum_required(VERSION 3.24)
project(lab_netsim)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab_netsim main.cpp include/storage_types.hpp src/storage_types.cpp)

set(GTEST_ROOT ../googletest-master)
target_include_directories(${EXEC_TEST} PUBLIC
        ${GTEST_ROOT}/googlemock/include
        ${GTEST_ROOT}/googletest/include
        )

add_subdirectory(${GTEST_ROOT} googletest-master)

target_link_libraries(${EXEC_TEST} gmock)
