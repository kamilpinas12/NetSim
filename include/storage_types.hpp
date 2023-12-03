//
// Created by kamil on 30/11/2023.
//

#ifndef LAB_NETSIM_STORAGE_TYPES_HPP
#define LAB_NETSIM_STORAGE_TYPES_HPP


#include <list>
#include "package.hpp"

// waydaje mi się że ma być enum class
enum class PackageQueueType{
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
    // przeniosłem implementacje tych iteratorów do PackageQueue bo faktycznie tutaj muszą być tylko wirtualne
    virtual const_iterator begin() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cend() const = 0;

};

class IPackageQueue: public IPackageStockpile{
public:
    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() = 0;

};


class PackageQueue: public IPackageQueue{
public:
    //TODO: Zaimplementować metody, zmienne itp. tej klasy


    //git Kamil (przeniesione z klasy IPackageStockpile)

    //git Maria (CLion czepia się, że używamy metody abstrakcyjnej i jej nie
    //nadpisujemy, to lepiej dodać "override" do każdej z iteracji
    const_iterator begin() const {return queue.cbegin();}
    const_iterator cbegin() const {return queue.cbegin();}
    const_iterator end() const {return queue.end();}
    const_iterator cend() const {return queue.cend();}

    //git Maria (dodanie metod) - potrzebne do storage types. Nadpisanie metod pop()
    // i get_queue_type()
    Package pop() override;
    PackageQueueType get_queue_type() override {return queue_type;}

private:
    std::list<Package> queue;
    //git Maria
    PackageQueueType queue_type;
};





#endif //LAB_NETSIM_STORAGE_TYPES_HPP
