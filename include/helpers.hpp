//
// Created by kamil on 07/12/2023.
//

#ifndef LAB_NETSIM_HELPERS_HPP
#define LAB_NETSIM_HELPERS_HPP


#include <functional>
#include <random>

#include "types.hpp"


extern std::random_device rd;
extern std::mt19937 rng;

extern double default_probability_generator();

extern ProbabilityGenerator probability_generator;


#endif //LAB_NETSIM_HELPERS_HPP
