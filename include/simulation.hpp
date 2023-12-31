//
// Created by maria on 28.12.2023.
//

#ifndef LAB_NETSIM_SIMULATION_HPP
#define LAB_NETSIM_SIMULATION_HPP

#include "factory.hpp"

class IntervalReportNotifier {
public:
    explicit IntervalReportNotifier (TimeOffset to) : to_(to) {};
    bool should_generate_report(Time t) {return t == 1 || t % to_ == 1; }

private:
    TimeOffset to_;
};

class SpecificTurnsReportNotifier {
public:
    explicit SpecificTurnsReportNotifier(std::set<Time> turns) : turns_(turns) {};
    bool should_generate_report(Time t) {return turns_.find(t) != turns_.cend(); };
private:
    std::set<Time> turns_;
};

void simulate(Factory& factory, Time t, std::function<void (Factory&, Time)> do_report){
    if (!factory.is_consistent()){
        throw std::logic_error("Funkcja jest niespójna");
    }
    for (Time i = 0; i < t; i++) {
        // Dostawa półproduktów do ramp
        factory.do_delivery(i);

        // Przekazanie półproduktów
        factory.do_package_passing();

        // Przetworzenie półproduktów
        factory.do_work(i);

        //Raportowanie
        do_report(factory, i);
    }
}

#endif //LAB_NETSIM_SIMULATION_HPP
