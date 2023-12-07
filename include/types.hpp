//
// Created by Filip Połatyński on 30/11/2023.
//

#ifndef LAB_NETSIM_TYPES_HPP
#define LAB_NETSIM_TYPES_HPP

#include<functional>

using ElementID = int;
using Time = int;
using TimeOffset = int;

//tego nie jestem pewien
using ProbabilityGenerator = std::function<double(void)>;

#endif //LAB_NETSIM_TYPES_HPP
