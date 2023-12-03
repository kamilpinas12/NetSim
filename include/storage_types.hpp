//
// Created by kamil on 30/11/2023.
//

#ifndef LAB_NETSIM_STORAGE_TYPES_HPP
#define LAB_NETSIM_STORAGE_TYPES_HPP


#include <list>
#include "package.hpp"


enum PackageQueueType{
    FIFO,
    LIFO
};


class IPackageStockpile{
public:
    virtual void push(Package&&) = 0;
    virtual bool empty() = 0;
    virtual  std::size_t size() = 0;
    virtual ~IPackageStockpile() = 0;
    using const_iterator = std::list<Package>::const_iterator;
    //TODO: Napisać iteratorytej klasy

    //iteratory  (nie jestem pewien czy o to chodzi)
    // Wydaje mi się że te iteratory muszą być wirtualne bo ta klasa jest w pełni wirtualna
    const_iterator begin() const {return queue.cbegin();}
    const_iterator cbegin() const {return queue.cbegin();}
    const_iterator end() const {return queue.end();}
    const_iterator cend() const {return queue.cend();}
// Coś tu nie gra bo interfejs nie przechowuje danych. Fajnie to jest opisane w książce kłęczka do cpp na stronie 68
private:
    std::list<Package> queue;

};

class IPackageQueue: public IPackageStockpile{
public:
    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() = 0;

};


class PackageQueue: public IPackageQueue{
public:
    //TODO: Zaimplementować metody, zmienne itp. tej klasy


};





#endif //LAB_NETSIM_STORAGE_TYPES_HPP
