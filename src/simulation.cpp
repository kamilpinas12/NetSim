//
// Created by Filip Połatyński on 31/12/2023.
//
#include "factory.hpp"

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
