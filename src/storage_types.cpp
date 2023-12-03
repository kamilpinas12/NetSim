//
// Created by kamil on 30/11/2023.
//

#include "storage_types.hpp"

Package PackageQueue::pop() {
    //Implementuję zmienną package
    Package package;

    switch(queue_type) {
        //Jeśli typ kolejki to LIFO (last in, first out)
        case PackageQueueType::LIFO:
           package = std::move(queue.back());
           //Pobierz ostatni element z kolejki
           queue.pop_back();
           //Zwróć zmienną
           return package;

        //Jeśli typ kolejki to FIFO (first in, first out)
        case PackageQueueType::FIFO:
           package = std::move(queue.front());
           //Pobierz pierwszy element z kolejki
           queue.pop_front();
           //Zwróć zmienną
           return package;
    }
}
