#ifndef MOCKS_GLOBAL_FUNCTIONS_MOCK_HPP_
#define MOCKS_GLOBAL_FUNCTIONS_MOCK_HPP_


#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "helpers.hpp"

// -------

class IGlobalFunctionsMock {
public:
    virtual double generate_canonical() = 0;

    virtual ~IGlobalFunctionsMock() = default;
};

class GlobalFunctionsMock : public IGlobalFunctionsMock {
public:
    MOCK_METHOD0(generate_canonical, double());
};


// ----

class GlobalFunctionsFixture : public ::testing::Test {
public:
    #ifdef WITH_PROBABILITY_GENERATOR
    GlobalFunctionsFixture() {
        probability_generator = [&]() { return global_functions_mock.generate_canonical(); };
    }

    ~GlobalFunctionsFixture() override {
        probability_generator = default_probability_generator;
    }
    #endif

protected:
    GlobalFunctionsMock global_functions_mock;
};

// ----

#endif /* MOCKS_GLOBAL_FUNCTIONS_MOCK_HPP_ */
