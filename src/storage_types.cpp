//
// Created by kamil on 30/11/2023.
//

#include "storage_types.hpp"

Package PackageQueue::pop() {
    Package package;

    //zmieniłem pare rzeczy bo jest błąd że na końcu nie ma returna więc w
    // każdym case dałem break i na końcu dałem return, działanie się nie zmieniło (Kamil)
    switch(queue_type_) {
        //Jeśli typ kolejki to LIFO (last in, first out)
        case PackageQueueType::LIFO:
           package = std::move(queue_.back());
           //Pobierz ostatni element z kolejki
           queue_.pop_back();
           break;


        //Jeśli typ kolejki to FIFO (first in, first out)
        case PackageQueueType::FIFO:
           package = std::move(queue_.front());
           //Pobierz pierwszy element z kolejki
           queue_.pop_front();
           break;
    }
    return package;
}
