cmake_minimum_required(VERSION 3.24)
project(lab_netsim)

set(CMAKE_CXX_STANDARD 17)



add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors -Wconversion -Wno-error=pessimizing-move)

add_compile_definitions(EXERCISE_ID=5)

include_directories(include)
set(SOURCE_FILES
        src/storage_types.cpp
        src/package.cpp
        src/nodes.cpp
        src/helpers.cpp
        src/factory.cpp
        src/reports.cpp
        src/simulation.cpp
        )



add_executable(lab_netsim ${SOURCE_FILES} main.cpp src/reports.cpp)
# == Unit testing using Google Testing Framework ==

set(SOURCES_FILES_TESTS
        test/main_gtest.cpp
        test/test_Factory.cpp
        test/test_factory_io.cpp
        test/test_nodes.cpp
        test/test_package.cpp
        test/test_reports.cpp
        test/test_simulate.cpp
        test/test_storage_types.cpp
        test/test_Factory.cpp
        test/test_factory_io.cpp
        )

set(EXEC_TEST ${PROJECT_ID}__test)
add_executable(${EXEC_TEST} ${SOURCE_FILES} ${SOURCES_FILES_TESTS} test/main_gtest.cpp)


set(GTEST_ROOT ../googletest-master)
target_include_directories(${EXEC_TEST} PUBLIC
        ${GTEST_ROOT}/googlemock/include
        ${GTEST_ROOT}/googletest/include
        mocks
        )

add_subdirectory(${GTEST_ROOT} googletest-master)

target_link_libraries(${EXEC_TEST} gmock)
