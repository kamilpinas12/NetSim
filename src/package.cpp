//
// Created by Filip Połatyński on 30/11/2023.
//

#include"package.hpp"

// Inicializowanie statycznych zmiennych jako puste zbiory
std::set<ElementID> Package::assigned_IDs = {};
std::set<ElementID> Package::freed_IDs = {};


Package::Package() {
    //Kamil zrobil dobrą robotę  (Kamil)

    //jeżeli coś jest w zwolnionych id to przypisujemy najmniejszą wartość ze zwolniopnych
    // Zamieniłem metode size() na metodę empty() bo jest adekwatniejsza w tym przypadku (Filip)
    if(!freed_IDs.empty()){
        id_ = *freed_IDs.begin();
        freed_IDs.erase(id_);
    }
    //jeżeli zwolnione id są puste nowe id to o 1 większe od największego do tej
    //przypisanego
    else if(!assigned_IDs.empty()){
        id_ = *assigned_IDs.end() + 1;
    }
    //pierwszy element dostaje id = 1 (nie wiem czy id zaczynają się od 0 czy od 1
    //dałem 1 jak coś wystarczy zmienić poniżej)
    else{
        id_ = 1;
    }
    assigned_IDs.insert(id_);
}

// Przenoszący operator przypisania (zamienia referencje do r-wartości na referencje do l-wartości)(move assignment operator)
Package &Package::operator=(Package &&package) {
    //TODO: (Filip)
    if (this != &package){
        // Przeniesienie z assigned_IDs do freed_IDs
        assigned_IDs.erase(this->id_);
        freed_IDs.insert(this->id_);
        // Przypisanie nowych wartości
        this->id_ = package.id_;
        assigned_IDs.insert(package.id_);
        return *this;
    }
    return *this;
}


Package::~Package() {
    //git  (Kamil)
    assigned_IDs.erase(id_);
    freed_IDs.insert(id_);
}
