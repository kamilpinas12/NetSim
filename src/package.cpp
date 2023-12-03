//
// Created by Filip Połatyński on 30/11/2023.
//

#include"package.hpp"

std::set<ElementID> Package::assigned_IDs;
std::set<ElementID> Package::freed_IDs;


Package::Package() {
    //TODO: Jakiś konstruktor  (Kamil)
    //jeżeli coś jest w zwolnionych id to przypisujemy najmniejszą wartość ze zwolniopnych
    if(freed_IDs.size()){
        id_ = *freed_IDs.begin();
        freed_IDs.erase(id_);
    }
    //jeżeli zwolnione id są puste nowe id to o 1 większe od największego do tej
    //przypisanego
    else if(assigned_IDs.size()){
        id_ = *assigned_IDs.end() + 1;
    }
    //pierwszy element dostaje id = 1 (nie wiem czy id zaczynają się od 0 czy od 1
    //dałem 1 jak coś wystarczy zmienić poniżej)
    else{
        id_ = 1;
    }
    assigned_IDs.insert(id_);
}


Package &Package::operator=(Package&& package) {
    //TODO: Nie mam pojęcia co to kurna jest ale trzeba to napisac

}


Package::~Package() {
    //TODO: Destruktor przenoszący z assigned_IDs do freed_IDs  (Kamil)

    assigned_IDs.erase(id_);
    freed_IDs.insert(id_);
}
