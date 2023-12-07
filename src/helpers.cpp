//
// Created by kamil on 07/12/2023.
//

#include <cstdlib>
#include <random>

#include "helpers.hpp"

std::random_device rd;
std::mt19937 rng(rd());

double default_probability_generator() {
    return std::generate_canonical<double, 10>(rng);
}

std::function<double()> probability_generator = default_probability_generator;