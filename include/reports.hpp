//
// Created by kamil on 27/12/2023.
//

#ifndef LAB_NETSIM_REPORTS_HPP
#define LAB_NETSIM_REPORTS_HPP

#include "factory.hpp"

// Raporty (Kamil)
void generate_structure_report(const Factory& f, std::ostream& os);
void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t);


#endif //LAB_NETSIM_REPORTS_HPP
